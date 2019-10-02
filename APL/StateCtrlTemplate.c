/**
 * @file  StateCtrlTemplate.c
 * @brief 状態制御テンプレート.
 * 
 * 状態制御テンプレートである.
 * 
 * @author      Teru
 * @date        2019/09/28
 * @version     Rev0.01
 * @copyright   2019 Emb-se.com.
 * 
 * @par 変更履歴:
 * - Rev0.01: 2019/09/28: 新規作成.
 * 
 * @copyright   2019 Emb-se.com.
 */
/**
 * @addtogroup StateCtrlTemp_GROUP アプリテンプレート機能.
 * @{
 */
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief 外部イベントコード.
 * 処理する外部からのイベントコードを定義する.
 */
typedef enum EXT_EVENT {
    DEV_EVENT_OFF = 0x0100,
    DEV_EVENT_ON  = 0x0101,
}EXT_EVENT;


/**
 * @brief 制御イベント番号.
 * シーケンシャルな制御イベント番号を定義する.
 */
typedef enum CTRL_EVENT {
    EVENT_OFF       = 0,
    EVENT_ON,
    EVENT_ERR,
    MAX_CTRL_EVENT
}CTRL_EVENT;

/**
 * @brief 制御状態番号.
 * シーケンシャルな制御ステート番号を定義する.
 */
typedef enum CTRL_STATE {
    STATE_IDLE      = 0,        /**< IDLE状態 */
    STATE_NORMAL,               /**< 通常状態 */
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
ActionTbl_t STATE_IDLE_ActTbl[ MAX_CTRL_EVENT ] = {
    EVENT_OFF_ignore_Func,
    EVENT_ON_Func,
    NULL
};
ActionTbl_t STATE_NORMAL_ActTbl[ MAX_CTRL_EVENT ] = {
    EVENT_OFF_Func,
    EVENT_ON_ignore_Func,
    NULL
};

/**
 * @brief 制御ステートテーブル.
 */
StateTbl_t  StateCtrlTemp_StateTbl[ MAX_CTRL_STATE ] = {
    STATE_IDLE_ActTbl,
    STATE_NORMAL_ActTbl,
    NULL
};

/**
 * @brief 現在の制御状態番号.
 */
static CTRL_STATE   StateCtrlTemp_NowState;


/**
 * @brief 外部イベントから制御イベント番号への変換.
 * @param[in]   ext_event   外部イベントコード.
 * @return  変換したシーケンシャルな制御イベント番号.
 * @retval  EVENT_ERR   エラー.
 */
static CTRL_EVENT StateCtrlTemp_convExtEventToCtrlEvent( EXT_EVENT ext_event )
{
    CTRL_EVENT  retv = EVENT_ERR;

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

/**
 * @brief ステートアクション関数群.
 * @param[in]   now_s   現在状態（遷移番号）.
 * @param[out]  next_s  次状態（遷移番号）.
 * @retval  true    正常終了.
 * @retval  false   エラー発生.
 */
static bool EVENT_OFF_Func( CTRL_STATE now_s, CTRL_STATE *next_s )
{
    *next_s = STATE_IDLE;
    return true;
}

static bool EVENT_ON_Func( CTRL_STATE now_s, CTRL_STATE *next_s )
{
    *next_s = STATE_NORMAL;
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

/**
 * @brief ステートマシン.
 * @param[in]   ext_event   外部イベントコード.
 * @retval  true    正常終了.
 * @retval  false   エラー発生.
 */
bool StateCtrlTemp_StateMachine( EXT_EVENT ext_event )
{
    CTRL_EVENT  ctrl_event = EVENT_ERR;
    CTRL_STATE  next_state = STATE_ERR;
	StateTbl_t	*p_state = NULL;
	ActionTbl_t	*p_act   = NULL;
    bool       retv 	 = false;

    /**< 外部イベントを制御イベント番号への変換 */
    ctrl_event = StateCtrlTemp_convExtEventToCtrlEvent( ext_event );
    if( ctrl_event == EVENT_ERR ) {
        return false;
    }

    p_state = &StateCtrlTemp_StateTbl[ ctrl_event ];
    if( p_state == NULL ) {
        return false;
    }

    /**< アクション */
	p_act = p_state->p_act;
    if( p_act == NULL ) {
        return false;
    }
    retv = p_act->p_func( StateCtrlTemp_NowState, &next_state );

    if( retv ) {
        /**< 次状態へ遷移 */
        StateCtrlTemp_NowState = next_state;
    }
    return retv;
}

/**
 * @} end of addtogroup
 */
