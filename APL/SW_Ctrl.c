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
#include "os.h"
#include "SwCtrl.h"

#undef  MUTEX_SAMPLE
#define QUEUE_SAMPLE
#define TASK_SAMPLE

/**
 * @brief スイッチデバイス.
 */
typedef enum SW_DEV {
    DEV_ExtSW1      = 0,        /**< ExtSW1 */
    DEV_ExtSW2,                 /**< ExtSW2 */
    MAX_SW_DEV
}SW_DEV;

/**
 * @brief スイッチ状態.
 */
typedef enum SW_STATE {
    DEV1_OFF        = 0,        /**< OFF状態 */
    DEV1_ON,                    /**< ON状態 */
    MAX_SW_STATE
}SW_STATE;

/**
 * @brief SwCtrlメッセージデータ型.
 */
typedef struct SwCtrl_MSG_t {
    /// スイッチデバイス.
    SW_DEV          dev_no;
    /// スイッチ状態.
    SW_STATE        dev_state;
}SwCtrl_MSG_t;


/**
 * @brief 制御ステータス.
 */
typedef enum CTRL_STATE {
    STATE_OFF        = 0,       /**< OFFステート */
    STATE_ON,                   /**< ONステート */
    STATE_ERR,
    MAX_CTRL_STATE
}CTRL_STATE;

/**
 * @brief 制御イベント.
 */
typedef enum CTRL_EVENT {
    EVENT_OFF       = 0,
    EVENT_ON,
}CTRL_EVENT;

typedef struct {
    int (*func)( CTRL_STATE old_s, CTRL_STATE *new_s );
}ActionTbl_t;
typedef struct {
    ActionTbl_t     *action;
}StateTbl_t;

StateTbl_t  ExtSw1_StateTbl[] = {
    StateOffFunc,

};


/**
 * @brief SwCtrl管理データデータ型.
 */
typedef struct SwCtrl_MNGR_t {
    /// ステート
    SwCtrl_STATE        state;
}SwCtrl_MNGR_t;

/// SwCtrl管理データ.
static SwCtrl_MNGR_t    SwCtrl_MNGR;


#ifdef MUTEX_SAMPLE
    /// MUTEXハンドル
    static osMutexHandle_t  SwCtrl_hMutex;
#endif
#ifdef QUEUE_SAMPLE
    // QUEUEハンドル
    static osQueHandle_t    SwCtrl_hQue;
#endif

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

    /* 内部管理データ初期化 */

    /* OSリソース生成 */
#ifdef MUTEX_SAMPLE
    // MUTEX生成
    SwCtrl_hMutex = osMutex_create();
    configASSERT( SwCtrl_hMutex == 0 );
#endif
#ifdef QUEUE_SAMPLE
    SwCtrl_hQue = osQue_create( 3, sizeof(SwCtrl_MNGR_t) );
    configASSERT( SwCtrl_hQue == 0 );
#endif
#ifdef TASK_SAMPLE
    // タスク生成
    retv = osTask_create( &SwCtrl_Task, "SwCtrl", SwCtrl_STACKSZ/4,
                        NULL, SwCtrlTASK_PRI | portPRIVILEGE_BIT, &handle );
    configASSERT( retv == pdPASS );
#endif
}


/**
 * @brief ExtSW-ON割込みハンドラ処理.
 */
void ExtSW_activeHandler( ExtSW_e dev )
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
        msg.dev_no    = SwCtrl_DEV_ExtSW1;
        msg.dev_state = SwCtrl_DEV_ON;
        osQue_sendISR( SwCtrl_hQue, &msg, &dispatch );
        break;
    case ExtSW2:
        msg.dev_no    = SwCtrl_DEV_ExtSW2;
        msg.dev_state = SwCtrl_DEV_ON;
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
 */
void ExtSW_inactiveHandler( ExtSW_e dev )
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
        msg.dev_no    = SwCtrl_DEV_ExtSW1;
        msg.dev_state = SwCtrl_DEV_OFF;
        osQue_sendISR( SwCtrl_hQue, &msg, &dispatch );
        break;
    case ExtSW2:
        msg.dev_no    = SwCtrl_DEV_ExtSW2;
        msg.dev_state = SwCtrl_DEV_OFF;
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
