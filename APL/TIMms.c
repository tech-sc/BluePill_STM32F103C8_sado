/**
 * @file  TIMms.c
 * @brief ミリ秒オーダータイマ機能.
 * 
 * ミリ秒単位で要求可能なソフトウェアタイマをAPLに提供する.
 * 
 * @author      Teru
 * @date        2019.06.27
 * @version     Rev0.01
 * @copyright   2019 Emb-se.com.
 * 
 * @par 変更履歴:
 * - Rev0.01: 2019.06.27: 新規作成
 * 
 * @copyright   2019 Emb-se.com.
 */
/**
 * @page TIMER_title タイマライブラリ
 *
 * @section TIMER_api API一覧
 * | API名          | 機能                             |
 * |:-------------- |:------------- |
 * | TIMms_initTimer()  | 初期設定   |
 * | TIMms_reqTimer()   | タイマ要求 |
 * | TIMms_cancelTimer()| タイマ取消 |
 *
 * @section TIMER_usage 使用方法
 * - 予め一度だけ TIMms_initTimer() を呼び出す。
 */
#include "TIMER.h"
#include "os.h"
#include "TIMms.h"
#include "ExtLED.h"		//計測用

/** 内部管理データ */
TIMms_t     *TIMms_TopReq;
TIMms_t     *TIMms_TailReq;
TIMms_t     *TIMms_TopExpire;

TIMCNT_t    TIMms_NextExpire;

osMutexHandle_t     TIMms_ExpireMutex;


/**
 * @brief 初期設定API.
 */
void TIMms_initTimer( void )
{
    osTaskHandle_t  handle;
	int		retv;

	/* タイマ起動 */
	TIMER_init();

	/* 内部管理データ初期化 */
    TIMms_TopReq     = NULL;
    TIMms_TailReq    = NULL;
    TIMms_TopExpire  = NULL;
    TIMms_NextExpire = (TIMCNT_t)-1;

	/* OSリソース生成 */
    osMutex_create( TIMms_ExpireMutex );
    osMutex_take( TIMms_ExpireMutex, portMAX_DELAY );

    /* 発火コールバック用タスクを生成する. */
    retv = osTask_create( &TIMms_task, "TIMms", TIMmsTASK_STACKSZ/4,
                        NULL, TIMmsTASK_PRI | portPRIVILEGE_BIT, &handle );
    configASSERT( retv == pdPASS );
}

/**
 * @brief 次回発火TICKセット.
 * @param[in] tick  次発火TICK値.
 */
static void TIMms_setNextExpire( TIMCNT_t tick )
{
    /* コンペアマッチレジスタを変更する. */
    TIMER_setCompReg( tick );

    TIMms_NextExpire = tick;
}

/**
 * @brief タイマ要求API.
 *
 * ミリ秒単位のタイマ要求を提供するAPI関数である.
 * 発火はコールバックにて通知する.
 *
 * @param[in] time  タイマ時間。ミリ秒で指定する.
 * @param[in] expire_cb  発火時のコールバック関数を指定する.
 * @param[in] p_req  タイマ要求データ領域を渡す.
 * @retval  handle  タイマ要求が成功すると、そのタイマハンドルを返す.
 * @retval  NULL    パラメータエラー.
 * @pre     timeは1ms以上であること.
 *          expire_cbはNULLでないこと.
 *          p_reqはNULLでないこと.
 * @post    タイマ停止は返したハンドルで停止可能である. また発火時に該当ハンドルを通知する.
 */
void *TIMms_reqTimer( int32_t time, void (*expire_cb)(void *handle), TIMms_t *p_req )
{
    TIMCNT_t  tmp_tick;

    configASSERT(time > 0);
    configASSERT(expire_cb != NULL);
    configASSERT(p_req != NULL);

    p_req->old_tick    = TIMms_getTick();
    p_req->remain_tick = MSEC_TO_TICK(time);
    p_req->expire_cb   = expire_cb;
    p_req->next_list   = NULL;
    p_req->feature     = TIMms_FEATURE;

    osEnterCritical();
    if( TIMms_TailReq != NULL ){
        TIMms_TailReq->next_list = p_req;
        TIMms_TailReq = p_req;
    }else{
        TIMms_TailReq = p_req;
        TIMms_TopReq  = p_req;
    }
    osExitCritical();

    tmp_tick  = p_req->old_tick;
    tmp_tick += p_req->remain_tick;
    if( tmp_tick < TIMms_NextExpire ){
        TIMms_setNextExpire( (TIMCNT_t)tmp_tick );
    }
    return p_req;
}


/**
 * @brief タイマ取消API.
 *
 * タイマ要求を取消すAPI関数である.
 * APIの使い方によっては取消と発火がすれ違う可能性があるので呼び出し側で対応すること.
 *
 * @param[in] handle  タイマ要求を取消すタイマハンドル.
 * @retval  handle  取消が成功すると、取消したタイマハンドルを返す.
 * @retval  NULL    パラメータエラー.
 *                  またはエントリされてない（既に発火した）
 * @pre     handleはNULLでないこと.
 */
void *TIMms_cancelTimer( TIMms_t *handle )
{
    TIMms_t     **pre, *p;

    osEnterCritical();
    pre = &TIMms_TopReq;
    p = TIMms_TopReq;
    while(p != NULL) {
        if( p == handle ) {
            if( TIMms_TailReq == handle ) {
                TIMms_TailReq = (*pre)->next_list;
            }
            *pre = p->next_list;
            break;
        }
        pre = &p->next_list;
        p = p->next_list;
    }
    osExitCritical();
    return p;
}


void TIMms_expire( int over )
{
    portBASE_TYPE   dispatch;
    TIMms_t     **pre, *p;
    TIMms_t     **exp;
    TIMCNT_t    now_tick, min_tick;
	int64_t		temp_tick;

	if( over ){
		// 計測用
		ExtLED1_toggle();
	}

    dispatch  = pdFALSE;
	min_tick = (TIMCNT_t)-1;	//set MAX
    now_tick = TIMms_getTick();

    osEnterCritical();
    exp = &TIMms_TopExpire;
    while( *exp != NULL ) {
        *exp = (*exp)->next_list;
    }
    pre = &TIMms_TopReq;
    p = TIMms_TopReq;
    while(p != NULL) {
		if( now_tick > p->old_tick ){
			temp_tick = 0;
		}else{
			temp_tick = (TIMCNT_t)-1;
			temp_tick ++;
		}
        temp_tick  += now_tick;
        temp_tick  -= p->old_tick;

		p->remain_tick -= temp_tick;
        if( p->remain_tick <= 0 ) {
             *pre = p->next_list;
             p->next_list = NULL;
             *exp = p;
             exp = &p->next_list;
             p = *pre;
        } else {
			if( p->remain_tick < min_tick ) {
				min_tick = (TIMCNT_t)p->remain_tick;
			}
			p->old_tick = now_tick;
            p = p->next_list;
        }
    }
	if( TIMms_TopReq == NULL ) {
		TIMms_TailReq = NULL;
	}
	TIMms_setNextExpire( min_tick );
    osExitCritical();

    if( TIMms_TopExpire != NULL ){
        /* タスクを起床する */
        osMutex_giveISR( TIMms_ExpireMutex, &dispatch );
        portEND_SWITCHING_ISR( dispatch );
    }
}

void TIMms_task( void *arg )
{
    TIMms_t     **exp, *p;

    while( 1 ){
        osMutex_take( TIMms_ExpireMutex, portMAX_DELAY );

        if( TIMms_TopExpire != NULL ){
            exp = &TIMms_TopExpire;
            while( *exp != NULL ) {
                p = *exp;
                *exp = p->next_list;
                (p->expire_cb)( p );
            }
        }
    }
}
