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
#include "os.h"
#include "SwCtrl.h"

#undef  MUTEX_SAMPLE
#define QUEUE_SAMPLE
#define TASK_SAMPLE

/**
 * @brief �X�C�b�`�f�o�C�X.
 */
typedef enum SW_DEV {
    DEV_ExtSW1      = 0,        /**< ExtSW1 */
    DEV_ExtSW2,                 /**< ExtSW2 */
    MAX_SW_DEV
}SW_DEV;

/**
 * @brief �X�C�b�`���.
 */
typedef enum SW_STATE {
    DEV1_OFF        = 0,        /**< OFF��� */
    DEV1_ON,                    /**< ON��� */
    MAX_SW_STATE
}SW_STATE;

/**
 * @brief SwCtrl���b�Z�[�W�f�[�^�^.
 */
typedef struct SwCtrl_MSG_t {
    /// �X�C�b�`�f�o�C�X.
    SW_DEV          dev_no;
    /// �X�C�b�`���.
    SW_STATE        dev_state;
}SwCtrl_MSG_t;


/**
 * @brief ����X�e�[�^�X.
 */
typedef enum CTRL_STATE {
    STATE_OFF        = 0,       /**< OFF�X�e�[�g */
    STATE_ON,                   /**< ON�X�e�[�g */
    STATE_ERR,
    MAX_CTRL_STATE
}CTRL_STATE;

/**
 * @brief ����C�x���g.
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
 * @brief SwCtrl�Ǘ��f�[�^�f�[�^�^.
 */
typedef struct SwCtrl_MNGR_t {
    /// �X�e�[�g
    SwCtrl_STATE        state;
}SwCtrl_MNGR_t;

/// SwCtrl�Ǘ��f�[�^.
static SwCtrl_MNGR_t    SwCtrl_MNGR;


#ifdef MUTEX_SAMPLE
    /// MUTEX�n���h��
    static osMutexHandle_t  SwCtrl_hMutex;
#endif
#ifdef QUEUE_SAMPLE
    // QUEUE�n���h��
    static osQueHandle_t    SwCtrl_hQue;
#endif

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
    SwCtrl_hQue = osQue_create( 3, sizeof(SwCtrl_MNGR_t) );
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
void ExtSW_activeHandler( ExtSW_e dev )
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
 * @brief ExtSW-OFF�����݃n���h������.
 */
void ExtSW_inactiveHandler( ExtSW_e dev )
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
