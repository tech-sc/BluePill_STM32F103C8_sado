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
#include <stdbool.h>
#include "os.h"
#include "SW_Ctrl.h"
#include "ExtLED.h"

#undef  MUTEX_SAMPLE
#define QUEUE_SAMPLE
#define TASK_SAMPLE

/**
 * @brief スイッチデバイス.
 */
typedef enum EXT_DEV {
    DEV_ExtSW1    = 0x0101,     /**< Ext SW1 */
    DEV_ExtSW2    = 0x0102,     /**< Ext SW2 */
    DEV_BSPSW1    = 0x0001,     /**< BSP SW1 */
    MAX_EXT_DEV
}EXT_DEV;

/**
 * @brief 外部イベントコード.
 * 処理する外部からのイベントコードを定義する.
 */
typedef enum EXT_EVENT {
    DEV_EVENT_OFF = 0x0100,     /**< OFF通知イベント */
    DEV_EVENT_ON  = 0x0101,     /**< ON通知イベント */
}EXT_EVENT;

/**
 * @brief SwCtrlメッセージデータ型.
 */
typedef struct SwCtrl_MSG_t {
    /// スイッチデバイス.
    EXT_DEV         dev_no;
    /// スイッチ状態.
    EXT_EVENT       dev_event;
}SwCtrl_MSG_t;


/**
 * @brief 制御デバイス番号.
 * シーケンシャルな制御デバイス番号を定義する.
 */
typedef enum CTRL_DEV {
    DEV_SW1       = 0,
    DEV_SW2,
    DEV_SW3,
    MAX_CTRL_DEV
}CTRL_DEV;

/**
 * @brief 制御イベント番号.
 * シーケンシャルな制御イベント番号を定義する.
 */
typedef enum CTRL_EVENT {
    EVENT_OFF     = 0,
    EVENT_ON,
    MAX_CTRL_EVENT
}CTRL_EVENT;

/**
 * @brief 制御状態番号.
 * シーケンシャルな制御ステート番号を定義する.
 */
typedef enum CTRL_STATE {
    STATE_OFF      = 0,         /**< OFF状態 */
    STATE_ON,                   /**< ON状態 */
    STATE_ERR,
    MAX_CTRL_STATE
}CTRL_STATE;

/**
 * @brief 制御スイッチ状態
 * 制御するスイッチの状態
 */
CTRL_STATE  SwCtrl_State[MAX_CTRL_DEV];


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
 * @brief 現在の制御状態番号.
 */
typedef struct SwCtrl_MNGR_t {
    CTRL_STATE          state;
}SwCtrl_MNGR_t;

static CTRL_STATE   SwCtrl_NowState;

uint8_t SwCtrl_counter;

/**
 * @brief SwCtrl管理データデータ型.
 */


#ifdef MUTEX_SAMPLE
    /// MUTEXハンドル
    static osMutexHandle_t  SwCtrl_hMutex;
#endif
#ifdef QUEUE_SAMPLE
    // QUEUEハンドル
    static osQueHandle_t    SwCtrl_hQue;
#endif


/**
 * @brief 外部イベントから制御イベント番号への変換.
 * @param[in]   ext_event   外部イベントコード.
 * @return  変換したシーケンシャルな制御イベント番号.
 * @retval  EVENT_ERR   エラー.
 */
static CTRL_EVENT convExtEventToCtrlEvent( EXT_EVENT ext_event )
{
    CTRL_EVENT  retv = MAX_CTRL_EVENT;

    switch( ext_event ) {
    case DEV_EVENT_OFF:
        retv = EVENT_OFF;
        break;
    case DEV_EVENT_ON:
        retv = EVENT_ON;
        break;
    default:
        break;
    }
    return retv;
}

CTRL_DEV convExtDevToCtrlDev( EXT_DEV dev )
{
    CTRL_DEV  retv = MAX_CTRL_DEV;

    switch( dev ) {
    case DEV_ExtSW1:
        retv = DEV_SW1;
        break;
    case DEV_ExtSW2:
        retv = DEV_SW2;
        break;
    default:
        break;
    }
    return retv;
}

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
                                        {ExtLED1_POPT, ExtLED1_PIN},
                                        {ExtLED1_POPT, ExtLED1_PIN}};

// スイッチアクション
static void SwCtrl_action( CTRL_DEV dev, CTRL_STATE next_s )
{
    int     i= 0;
    switch( dev ) {
    case DEV_SW1:
        SwCtrl_counter++;
        // LED表示
        for( i=0; i<4; i++ ) {
            if( SwCtrl_counter & (1 << i) ) {
                ExtLED_turnON( led[i].port, led[i].pin );
            } else {
                ExtLED_turnOFF( led[i].port, led[i].pin );
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
static bool StateMachine( EXT_EVENT ext_event, CTRL_STATE now_s, CTRL_STATE *next_s )
{
    CTRL_EVENT  ctrl_event = MAX_CTRL_EVENT;
    StateTbl_t  *p_state = NULL;
    ActionTbl_t *p_act   = NULL;
    bool       retv      = false;

    /**< 外部イベントを制御イベント番号への変換 */
    ctrl_event = convExtEventToCtrlEvent( ext_event );
    if( ctrl_event == MAX_CTRL_EVENT ) {
        return false;
    }

    p_state = &SwCtrl_StateTbl[ ctrl_event ];
    if( p_state == NULL ) {
        return false;
    }

    /**< アクション */
    p_act = p_state->p_act;
    if( p_act == NULL ) {
        return false;
    }
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
    CTRL_DEV        dev;
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
        if( StateMachine( msg.dev_event, SwCtrl_NowState, &next_s ) ) {
            // 物理デバイス番号を論理シーケンシャルデバイス番号に変換
            dev = convExtDevToCtrlDev( msg.dev_no );
            if( dev != MAX_CTRL_DEV ) {
                // 次状態遷移
                SwCtrl_State[ dev ] = next_s;
                // スイッチアクション
                SwCtrl_action( dev, next_s );
            }
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

    /* 内部管理データ初期化 */

    /* OSリソース生成 */
#ifdef MUTEX_SAMPLE
    // MUTEX生成
    SwCtrl_hMutex = osMutex_create();
    configASSERT( SwCtrl_hMutex == 0 );
#endif
#ifdef QUEUE_SAMPLE
    SwCtrl_hQue = osQue_create( 3, sizeof(SwCtrl_MSG_t) );
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
void ExtSW_activeHandler( EXT_DEV dev )
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
    case DEV_ExtSW1:
        msg.dev_no    = DEV_ExtSW1;
        msg.dev_event = DEV_EVENT_ON;
        osQue_sendISR( SwCtrl_hQue, &msg, &dispatch );
        break;
    case DEV_ExtSW2:
        msg.dev_no    = DEV_ExtSW2;
        msg.dev_event = DEV_EVENT_ON;
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
void ExtSW_inactiveHandler( EXT_DEV dev )
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
    case DEV_ExtSW1:
        msg.dev_no    = DEV_ExtSW1;
        msg.dev_event = DEV_EVENT_OFF;
        osQue_sendISR( SwCtrl_hQue, &msg, &dispatch );
        break;
    case DEV_ExtSW2:
        msg.dev_no    = DEV_ExtSW2;
        msg.dev_event = DEV_EVENT_OFF;
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
