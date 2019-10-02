/**
 * @file  StateCtrlTemplate.c
 * @brief ��Ԑ���e���v���[�g.
 * 
 * ��Ԑ���e���v���[�g�ł���.
 * 
 * @author      Teru
 * @date        2019/09/28
 * @version     Rev0.01
 * @copyright   2019 Emb-se.com.
 * 
 * @par �ύX����:
 * - Rev0.01: 2019/09/28: �V�K�쐬.
 * 
 * @copyright   2019 Emb-se.com.
 */
/**
 * @addtogroup StateCtrlTemp_GROUP �A�v���e���v���[�g�@�\.
 * @{
 */
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief �O���C�x���g�R�[�h.
 * ��������O������̃C�x���g�R�[�h���`����.
 */
typedef enum EXT_EVENT {
    DEV_EVENT_OFF = 0x0100,
    DEV_EVENT_ON  = 0x0101,
}EXT_EVENT;


/**
 * @brief ����C�x���g�ԍ�.
 * �V�[�P���V�����Ȑ���C�x���g�ԍ����`����.
 */
typedef enum CTRL_EVENT {
    EVENT_OFF       = 0,
    EVENT_ON,
    EVENT_ERR,
    MAX_CTRL_EVENT
}CTRL_EVENT;

/**
 * @brief �����Ԕԍ�.
 * �V�[�P���V�����Ȑ���X�e�[�g�ԍ����`����.
 */
typedef enum CTRL_STATE {
    STATE_IDLE      = 0,        /**< IDLE��� */
    STATE_NORMAL,               /**< �ʏ��� */
    STATE_ERR,
    MAX_CTRL_STATE
}CTRL_STATE;

/**
 * @brief �A�N�V�����֐��^.
 */
typedef bool(*FUNC_p)( CTRL_STATE, CTRL_STATE* );

/**
 * @brief ����A�N�V�����e�[�u���^.
 */
typedef struct {
    FUNC_p  p_func;
}ActionTbl_t;

/**
 * @brief ����X�e�[�g�e�[�u���^.
 */
typedef struct {
    ActionTbl_t     *p_act;
}StateTbl_t;


/**
 * @brief �X�e�[�g�A�N�V�����֐��Q.
 */
static bool EVENT_OFF_Func( CTRL_STATE now_s, CTRL_STATE *next_s );
static bool EVENT_ON_Func( CTRL_STATE now_s, CTRL_STATE *next_s );
static bool EVENT_OFF_ignore_Func( CTRL_STATE now_s, CTRL_STATE *next_s );
static bool EVENT_ON_ignore_Func( CTRL_STATE now_s, CTRL_STATE *next_s );


/**
 * @brief �A�N�V�����e�[�u��.
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
 * @brief ����X�e�[�g�e�[�u��.
 */
StateTbl_t  StateCtrlTemp_StateTbl[ MAX_CTRL_STATE ] = {
    STATE_IDLE_ActTbl,
    STATE_NORMAL_ActTbl,
    NULL
};

/**
 * @brief ���݂̐����Ԕԍ�.
 */
static CTRL_STATE   StateCtrlTemp_NowState;


/**
 * @brief �O���C�x���g���琧��C�x���g�ԍ��ւ̕ϊ�.
 * @param[in]   ext_event   �O���C�x���g�R�[�h.
 * @return  �ϊ������V�[�P���V�����Ȑ���C�x���g�ԍ�.
 * @retval  EVENT_ERR   �G���[.
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
 * @brief �X�e�[�g�A�N�V�����֐��Q.
 * @param[in]   now_s   ���ݏ�ԁi�J�ڔԍ��j.
 * @param[out]  next_s  ����ԁi�J�ڔԍ��j.
 * @retval  true    ����I��.
 * @retval  false   �G���[����.
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
 * @brief �X�e�[�g�}�V��.
 * @param[in]   ext_event   �O���C�x���g�R�[�h.
 * @retval  true    ����I��.
 * @retval  false   �G���[����.
 */
bool StateCtrlTemp_StateMachine( EXT_EVENT ext_event )
{
    CTRL_EVENT  ctrl_event = EVENT_ERR;
    CTRL_STATE  next_state = STATE_ERR;
	StateTbl_t	*p_state = NULL;
	ActionTbl_t	*p_act   = NULL;
    bool       retv 	 = false;

    /**< �O���C�x���g�𐧌�C�x���g�ԍ��ւ̕ϊ� */
    ctrl_event = StateCtrlTemp_convExtEventToCtrlEvent( ext_event );
    if( ctrl_event == EVENT_ERR ) {
        return false;
    }

    p_state = &StateCtrlTemp_StateTbl[ ctrl_event ];
    if( p_state == NULL ) {
        return false;
    }

    /**< �A�N�V���� */
	p_act = p_state->p_act;
    if( p_act == NULL ) {
        return false;
    }
    retv = p_act->p_func( StateCtrlTemp_NowState, &next_state );

    if( retv ) {
        /**< ����Ԃ֑J�� */
        StateCtrlTemp_NowState = next_state;
    }
    return retv;
}

/**
 * @} end of addtogroup
 */
