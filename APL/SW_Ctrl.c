/**
 * @file  SwCtrl.c
 * @brief スイッチ制御機能.
 * 
 * スイッチ制御機能である.
 * 
 * @author      Teru
 * @date        2019/09/27
 * @version     Rev0.10
 * @copyright   2019 Emb-se.com.
 * 
 * @par 変更履歴:
 * - Rev0.10: 2019/09/27: AplTemplateを元に新規作成.
 * 
 * @copyright   2019 Emb-se.com.
 */
/**
 * @addtogroup SwCtrl_GROUP アプリテンプレート機能.
 * @{
 */
#include <string.h>
#include <stdbool.h>
#include "SW_Ctrl.h"
#include "os.h"
#include "ExtSW.h"
#include "ExtLED.h"

#undef  MUTEX_SAMPLE
#define QUEUE_SAMPLE
#define TASK_SAMPLE


/**
 * @brief 制御デバイス番号.
 * シーケンシャルな制御デバイス番号を定義する.
 */
typedef enum CTRL_DEV {
    DEV_SW1         = 0,
    DEV_SW2,
    DEV_SW3,
    MAX_CTRL_DEV
}CTRL_DEV;

/**
 * @brief 制御イベント番号.
 * シーケンシャルな制御イベント番号を定義する.
 */
typedef enum CTRL_EVENT {
    EVENT_OFF       = 0,
    EVENT_ON,
    MAX_CTRL_EVENT
}CTRL_EVENT;

/**
 * @brief SwCtrlメッセージデータ型.
 */
typedef struct SwCtrl_MSG_t {
    /// スイッチデバイス.
    CTRL_DEV    dev_no;
    /// スイッチ状態.
    CTRL_EVENT  dev_event;
}SwCtrl_MSG_t;


/**
 * @brief 制御状態番号.
 * シーケンシャルな制御ステート番号を定義する.
 */
typedef enum CTRL_STATE {
    STATE_OFF       = 0,         /**< OFF状態 */
    STATE_ON,                    /**< ON状態 */
    STATE_ERR,
    MAX_CTRL_STATE
}CTRL_STATE;


/**
 * @brief アクション関数型.
 */
typedef bool(*FUNC_p)( CTRL_STATE, CTRL_STATE* );

/**
 * @brief 制御アクションテーブル型.
 */
typedef struct {
    FUNC_p  p_func;
}ActionTbl_t;

/**
 * @brief 制御ステートテーブル型.
 */
typedef struct {
    ActionTbl_t     *p_act;
}StateTbl_t;

/**
 * @brief ステートアクション関数群.
 */
static bool EVENT_OFF_Func( CTRL_STATE now_s, CTRL_STATE *next_s );
static bool EVENT_ON_Func( CTRL_STATE now_s, CTRL_STATE *next_s );
static bool EVENT_OFF_ignore_Func( CTRL_STATE now_s, CTRL_STATE *next_s );
static bool EVENT_ON_ignore_Func( CTRL_STATE now_s, CTRL_STATE *next_s );


/**
 * @brief アクションテーブル.
 */
ActionTbl_t STATE_OFF_ActTbl[ MAX_CTRL_EVENT ] = {
    EVENT_OFF_ignore_Func,
    EVENT_ON_Func,
};
ActionTbl_t STATE_ON_ActTbl[ MAX_CTRL_EVENT ] = {
    EVENT_OFF_Func,
    EVENT_ON_ignore_Func,
};

/**
 * @brief 制御ステートテーブル.
 */
StateTbl_t  SwCtrl_StateTbl[ MAX_CTRL_STATE ] = {
    STATE_OFF_ActTbl,
    STATE_ON_ActTbl,
    NULL
};

/**
 * @brief スイッチ制御機能の管理データ
 */
typedef struct SwCtrl_MNGR_t {
	/// 現在の各SW制御ステート状態番号.
    CTRL_STATE		NowState[ MAX_CTRL_DEV ];
	uint8_t			SW1Counter;
}SwCtrl_MNGR_t;

static SwCtrl_MNGR_t	SwCtrl_MngData;



#ifdef MUTEX_SAMPLE
    /// MUTEXハンドル
    static osMutexHandle_t  SwCtrl_hMutex;
#endif
#ifdef QUEUE_SAMPLE
    // QUEUEハンドル
    static osQueHandle_t    SwCtrl_hQue;
#endif


/**
 * @brief ステートアクション関数群.
 * @param[in]   now_s   現在状態（遷移番号）.
 * @param[out]  next_s  次状態（遷移番号）.
 * @retval  true    正常終了.
 * @retval  false   エラー発生.
 */
static bool EVENT_OFF_Func( CTRL_STATE now_s, CTRL_STATE *next_s )
{
    *next_s = STATE_OFF;
    return true;
}

static bool EVENT_ON_Func( CTRL_STATE now_s, CTRL_STATE *next_s )
{
    *next_s = STATE_ON;
    return true;
}

static bool EVENT_OFF_ignore_Func( CTRL_STATE now_s, CTRL_STATE *next_s )
{
    *next_s = now_s;
    return false;
}

static bool EVENT_ON_ignore_Func( CTRL_STATE now_s, CTRL_STATE *next_s )
{
    *next_s = now_s;
    return false;
}

typedef struct {
    GPIO_TypeDef    *port;
    uint32_t        pin;
}LedCtrl_Info;
static const LedCtrl_Info   led[ 4 ] = {{ExtLED1_POPT, ExtLED1_PIN},
                                        {ExtLED2_POPT, ExtLED2_PIN},
                                        {ExtLED3_POPT, ExtLED3_PIN},
                                        {ExtLED4_POPT, ExtLED4_PIN}};

// スイッチアクション
static void SwCtrl_action( CTRL_DEV dev, CTRL_STATE next_s )
{
    int     i= 0;
    switch( dev ) {
    case DEV_SW1:
		if( next_s == STATE_ON ) {
			SwCtrl_MngData.SW1Counter++;
			// LED表示
			for( i=0; i<4; i++ ) {
				if( SwCtrl_MngData.SW1Counter & (1 << i) ) {
					ExtLED_turnON( led[i].port, led[i].pin );
				} else {
					ExtLED_turnOFF( led[i].port, led[i].pin );
				}
			}
		}
        break;
    default:
        break;
    }
}

/**
 * @brief ステートマシン.
 * @param[in]   ext_event   外部イベントコード.
 * @retval  true    正常終了.
 * @retval  false   エラー発生.
 */
static bool StateMachine( CTRL_EVENT ctrl_event, CTRL_STATE now_s, CTRL_STATE *next_s )
{
    StateTbl_t  *p_state = NULL;
    ActionTbl_t *p_act   = NULL;
    bool       retv      = false;

    p_state = &SwCtrl_StateTbl[ now_s ];
    if( p_state == NULL ) {
        return false;
    }

    /**< アクション */
    p_act = p_state->p_act;
    if( p_act == NULL ) {
        return false;
    }
	p_act = &p_act[ ctrl_event ];
    retv = p_act->p_func( now_s, next_s );

    return retv;
}


/**
 * @brief ミリ秒タイマ発火処理タスク.
 * @param[in]   arg  未使用.
 */
static void SwCtrl_Task( void *arg )
{
#ifdef QUEUE_SAMPLE
    SwCtrl_MSG_t    msg;
#endif
#if defined(QUEUE_SAMPLE)||defined(MUTEX_SAMPLE)
    osRetVal_t      retv;
#endif
    CTRL_STATE      next_s;

    while( 1 ){
#ifdef MUTEX_SAMPLE
        // MUTEX-take
        retv = osMutex_take( SwCtrl_hMutex, osMAX_TIME );
        if( retv == pdFALSE ) {
            continue;
        }
#endif
#ifdef QUEUE_SAMPLE
        retv = osQue_receive( SwCtrl_hQue, &msg, osMAX_TIME );
        if( retv == pdFALSE ) {
            continue;
        }
#endif
        if( StateMachine( msg.dev_event, SwCtrl_MngData.NowState[msg.dev_no], &next_s ) ) {
			// 次状態遷移
			SwCtrl_MngData.NowState[ msg.dev_no ] = next_s;
			// スイッチアクション
			SwCtrl_action( msg.dev_no, next_s );
        }
    }
}


/**
 * @brief 初期設定API.
 *
 * 初期設定及びOSリソースの生成.
 */
void SwCtrl_init( void )
{
    osTaskHandle_t  handle;
    int     retv;

	printf("%s\n", __FUNCTION__);
    /* 内部管理データ初期化 */
	memset( &SwCtrl_MngData, 0, sizeof(SwCtrl_MngData) );

    /* OSリソース生成 */
#ifdef MUTEX_SAMPLE
    // MUTEX生成.
    SwCtrl_hMutex = osMutex_create();
    configASSERT( SwCtrl_hMutex != 0 );
#endif
#ifdef QUEUE_SAMPLE
    SwCtrl_hQue = osQue_create( 3, sizeof(SwCtrl_MSG_t) );
    configASSERT( SwCtrl_hQue != 0 );
#endif
#ifdef TASK_SAMPLE
    // タスク生成.
    retv = osTask_create( &SwCtrl_Task, "SwCtrl", SwCtrl_STACKSZ/4,
                        NULL, SwCtrlTASK_PRI | portPRIVILEGE_BIT, &handle );
    configASSERT( retv == pdPASS );
#endif
}


/**
 * @brief ExtSW-ON割込みハンドラ処理.
 *	ExtSWの割込みハンドラからコールされる.
 */
void ExtSW_activeHandler( ExtSW dev )
{
    portBASE_TYPE   dispatch = pdFALSE;
#ifdef QUEUE_SAMPLE
    SwCtrl_MSG_t    msg;
#endif

    /* タスクを起床する */
#ifdef MUTEX_SAMPLE
    // MUTEX-give
    osMutex_giveISR( SwCtrl_hMutex, &dispatch );
#endif
#ifdef QUEUE_SAMPLE
    switch( dev ) {
    case ExtSW1:
        msg.dev_no    = DEV_SW1;
        msg.dev_event = EVENT_ON;
        osQue_sendISR( SwCtrl_hQue, &msg, &dispatch );
        break;
    case ExtSW2:
        msg.dev_no    = DEV_SW2;
        msg.dev_event = EVENT_ON;
        osQue_sendISR( SwCtrl_hQue, &msg, &dispatch );
        break;
    default:
        break;
    }
#endif

    portEND_SWITCHING_ISR( dispatch );
}

/**
 * @brief ExtSW-OFF割込みハンドラ処理.
 *	ExtSWの割込みハンドラからコールされる.
 */
void ExtSW_inactiveHandler( ExtSW dev )
{
    portBASE_TYPE   dispatch = pdFALSE;
#ifdef QUEUE_SAMPLE
    SwCtrl_MSG_t    msg;
#endif

    /* タスクを起床する */
#ifdef MUTEX_SAMPLE
    // MUTEX-give
    osMutex_giveISR( SwCtrl_hMutex, &dispatch );
#endif
#ifdef QUEUE_SAMPLE
    switch( dev ) {
    case ExtSW1:
        msg.dev_no    = DEV_SW1;
        msg.dev_event = EVENT_OFF;
        osQue_sendISR( SwCtrl_hQue, &msg, &dispatch );
        break;
    case ExtSW2:
        msg.dev_no    = DEV_SW2;
        msg.dev_event = EVENT_OFF;
        osQue_sendISR( SwCtrl_hQue, &msg, &dispatch );
        break;
    default:
        break;
    }
#endif

    portEND_SWITCHING_ISR( dispatch );
}

/**
 * @} end of addtogroup
 */
