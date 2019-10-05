/**
 * @file  SwCtrl.c
 * @brief �X�C�b�`����@�\.
 * 
 * �X�C�b�`����@�\�ł���.
 * 
 * @author      Teru
 * @date        2019/09/27
 * @version     Rev0.10
 * @copyright   2019 Emb-se.com.
 * 
 * @par �ύX����:
 * - Rev0.10: 2019/09/27: AplTemplate�����ɐV�K�쐬.
 * 
 * @copyright   2019 Emb-se.com.
 */
/**
 * @addtogroup SwCtrl_GROUP �A�v���e���v���[�g�@�\.
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
 * @brief �X�C�b�`�f�o�C�X.
 */
typedef enum EXT_DEV {
    DEV_ExtSW1    = 0x0101,     /**< Ext SW1 */
    DEV_ExtSW2    = 0x0102,     /**< Ext SW2 */
    DEV_BSPSW1    = 0x0001,     /**< BSP SW1 */
    MAX_EXT_DEV
}EXT_DEV;

/**
 * @brief �O���C�x���g�R�[�h.
 * ��������O������̃C�x���g�R�[�h���`����.
 */
typedef enum EXT_EVENT {
    DEV_EVENT_OFF = 0x0100,     /**< OFF�ʒm�C�x���g */
    DEV_EVENT_ON  = 0x0101,     /**< ON�ʒm�C�x���g */
}EXT_EVENT;

/**
 * @brief SwCtrl���b�Z�[�W�f�[�^�^.
 */
typedef struct SwCtrl_MSG_t {
    /// �X�C�b�`�f�o�C�X.
    EXT_DEV         dev_no;
    /// �X�C�b�`���.
    EXT_EVENT       dev_event;
}SwCtrl_MSG_t;


/**
 * @brief ����f�o�C�X�ԍ�.
 * �V�[�P���V�����Ȑ���f�o�C�X�ԍ����`����.
 */
typedef enum CTRL_DEV {
    DEV_SW1       = 0,
    DEV_SW2,
    DEV_SW3,
    MAX_CTRL_DEV
}CTRL_DEV;

/**
 * @brief ����C�x���g�ԍ�.
 * �V�[�P���V�����Ȑ���C�x���g�ԍ����`����.
 */
typedef enum CTRL_EVENT {
    EVENT_OFF     = 0,
    EVENT_ON,
    MAX_CTRL_EVENT
}CTRL_EVENT;

/**
 * @brief �����Ԕԍ�.
 * �V�[�P���V�����Ȑ���X�e�[�g�ԍ����`����.
 */
typedef enum CTRL_STATE {
    STATE_OFF      = 0,         /**< OFF��� */
    STATE_ON,                   /**< ON��� */
    STATE_ERR,
    MAX_CTRL_STATE
}CTRL_STATE;

/**
 * @brief ����X�C�b�`���
 * ���䂷��X�C�b�`�̏��
 */
CTRL_STATE  SwCtrl_State[MAX_CTRL_DEV];


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
ActionTbl_t STATE_OFF_ActTbl[ MAX_CTRL_EVENT ] = {
    EVENT_OFF_ignore_Func,
    EVENT_ON_Func,
};
ActionTbl_t STATE_ON_ActTbl[ MAX_CTRL_EVENT ] = {
    EVENT_OFF_Func,
    EVENT_ON_ignore_Func,
};

/**
 * @brief ����X�e�[�g�e�[�u��.
 */
StateTbl_t  SwCtrl_StateTbl[ MAX_CTRL_STATE ] = {
    STATE_OFF_ActTbl,
    STATE_ON_ActTbl,
    NULL
};

/**
 * @brief ���݂̐����Ԕԍ�.
 */
typedef struct SwCtrl_MNGR_t {
    CTRL_STATE          state;
}SwCtrl_MNGR_t;

static CTRL_STATE   SwCtrl_NowState;

uint8_t SwCtrl_counter;

/**
 * @brief SwCtrl�Ǘ��f�[�^�f�[�^�^.
 */


#ifdef MUTEX_SAMPLE
    /// MUTEX�n���h��
    static osMutexHandle_t  SwCtrl_hMutex;
#endif
#ifdef QUEUE_SAMPLE
    // QUEUE�n���h��
    static osQueHandle_t    SwCtrl_hQue;
#endif


/**
 * @brief �O���C�x���g���琧��C�x���g�ԍ��ւ̕ϊ�.
 * @param[in]   ext_event   �O���C�x���g�R�[�h.
 * @return  �ϊ������V�[�P���V�����Ȑ���C�x���g�ԍ�.
 * @retval  EVENT_ERR   �G���[.
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
 * @brief �X�e�[�g�A�N�V�����֐��Q.
 * @param[in]   now_s   ���ݏ�ԁi�J�ڔԍ��j.
 * @param[out]  next_s  ����ԁi�J�ڔԍ��j.
 * @retval  true    ����I��.
 * @retval  false   �G���[����.
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

// �X�C�b�`�A�N�V����
static void SwCtrl_action( CTRL_DEV dev, CTRL_STATE next_s )
{
    int     i= 0;
    switch( dev ) {
    case DEV_SW1:
        SwCtrl_counter++;
        // LED�\��
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
 * @brief �X�e�[�g�}�V��.
 * @param[in]   ext_event   �O���C�x���g�R�[�h.
 * @retval  true    ����I��.
 * @retval  false   �G���[����.
 */
static bool StateMachine( EXT_EVENT ext_event, CTRL_STATE now_s, CTRL_STATE *next_s )
{
    CTRL_EVENT  ctrl_event = MAX_CTRL_EVENT;
    StateTbl_t  *p_state = NULL;
    ActionTbl_t *p_act   = NULL;
    bool       retv      = false;

    /**< �O���C�x���g�𐧌�C�x���g�ԍ��ւ̕ϊ� */
    ctrl_event = convExtEventToCtrlEvent( ext_event );
    if( ctrl_event == MAX_CTRL_EVENT ) {
        return false;
    }

    p_state = &SwCtrl_StateTbl[ ctrl_event ];
    if( p_state == NULL ) {
        return false;
    }

    /**< �A�N�V���� */
    p_act = p_state->p_act;
    if( p_act == NULL ) {
        return false;
    }
    retv = p_act->p_func( now_s, next_s );

    return retv;
}


/**
 * @brief �~���b�^�C�}���Ώ����^�X�N.
 * @param[in]   arg  ���g�p.
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
            // �����f�o�C�X�ԍ���_���V�[�P���V�����f�o�C�X�ԍ��ɕϊ�
            dev = convExtDevToCtrlDev( msg.dev_no );
            if( dev != MAX_CTRL_DEV ) {
                // ����ԑJ��
                SwCtrl_State[ dev ] = next_s;
                // �X�C�b�`�A�N�V����
                SwCtrl_action( dev, next_s );
            }
        }
    }
}


/**
 * @brief �����ݒ�API.
 *
 * �����ݒ�y��OS���\�[�X�̐���.
 */
void SwCtrl_init( void )
{
    osTaskHandle_t  handle;
    int     retv;

    /* �����Ǘ��f�[�^������ */

    /* OS���\�[�X���� */
#ifdef MUTEX_SAMPLE
    // MUTEX����
    SwCtrl_hMutex = osMutex_create();
    configASSERT( SwCtrl_hMutex == 0 );
#endif
#ifdef QUEUE_SAMPLE
    SwCtrl_hQue = osQue_create( 3, sizeof(SwCtrl_MSG_t) );
    configASSERT( SwCtrl_hQue == 0 );
#endif
#ifdef TASK_SAMPLE
    // �^�X�N����
    retv = osTask_create( &SwCtrl_Task, "SwCtrl", SwCtrl_STACKSZ/4,
                        NULL, SwCtrlTASK_PRI | portPRIVILEGE_BIT, &handle );
    configASSERT( retv == pdPASS );
#endif
}


/**
 * @brief ExtSW-ON�����݃n���h������.
 */
void ExtSW_activeHandler( EXT_DEV dev )
{
    portBASE_TYPE   dispatch = pdFALSE;
#ifdef QUEUE_SAMPLE
    SwCtrl_MSG_t    msg;
#endif

    /* �^�X�N���N������ */
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
 * @brief ExtSW-OFF�����݃n���h������.
 */
void ExtSW_inactiveHandler( EXT_DEV dev )
{
    portBASE_TYPE   dispatch = pdFALSE;
#ifdef QUEUE_SAMPLE
    SwCtrl_MSG_t    msg;
#endif

    /* �^�X�N���N������ */
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
