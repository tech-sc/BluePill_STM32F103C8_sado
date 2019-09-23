/**
 * @file  AplTemplate.c
 * @brief �A�v���e���v���[�g.
 * 
 * RTOS���\�[�X���g�p����A�v���P�[�V�����e���v���[�g�ł���.
 * 
 * @author		Teru
 * @date        2019/09/22
 * @version 	Rev0.10
 * @copyright	2019 Emb-se.com.
 * 
 * @par �ύX����:
 * - Rev0.01: 2019/09/22: �V�K�쐬.
 * 
 * @copyright	2019 Emb-se.com.
 */
/**
 * @addtogroup AplTemp_GROUP �A�v���e���v���[�g�@�\.
 * @{
 */
#include "os.h"
#include "AplTemplate.h"

#define MUTEX_SAMPLE
#define QUEUE_SAMPLE
#define TASK_SAMPLE

/**
 * @brief API���^�[���R�[�h.
 */
typedef enum AplTemp_STATE_e {
	AplTemp_IDLE		= 0,		/**< IDLE */
	AplTemp_MAX_STATE
}AplTemp_STATE_e;

/**
 * @brief APL�Ǘ��f�[�^�f�[�^�^.
 */
typedef struct AplTemp_MNGR_t {
	/// �X�e�[�g
	AplTemp_STATE_e		state;
}AplTemp_MNGR_t;


/// APL�Ǘ��f�[�^.
static AplTemp_MNGR_t	AplTemp_MNGR;

#ifdef MUTEX_SAMPLE
	/// MUTEX�n���h��
	static osMutexHandle_t	AplTemp_hMutex;
#endif
#ifdef QUEUE_SAMPLE
	// QUEUE�n���h��
	static osQueHandle_t	AplTemp_hQue;
#endif

/**
 * @brief �~���b�^�C�}���Ώ����^�X�N.
 * @param[in]	arg  ���g�p.
 */
static void AplTemp_Task( void *arg )
{
#ifdef QUEUE_SAMPLE
	AplTemp_MSG_t	msg;
#endif
#if defined(QUEUE_SAMPLE)||defined(MUTEX_SAMPLE)
	osRetVal_t		retv;
#endif

	while( 1 ){
#ifdef MUTEX_SAMPLE
		// MUTEX-take
		retv = osMutex_take( AplTemp_hMutex, osMAX_TIME );
		if( retv == pdFALSE ) {
			continue;
		}
#endif
#ifdef QUEUE_SAMPLE
		retv = osQue_receive( AplTemp_hQue, &msg, osMAX_TIME );
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
void AplTemp_init( void )
{
	osTaskHandle_t	handle;
	int		retv;

	/* �����Ǘ��f�[�^������ */

	/* OS���\�[�X���� */
#ifdef MUTEX_SAMPLE
	// MUTEX����
	AplTemp_hMutex = osMutex_create();
	configASSERT( AplTemp_hMutex == 0 );
#endif
#ifdef QUEUE_SAMPLE
	AplTemp_hQue = osQue_create( 3, sizeof(AplTemp_MNGR_t) );
	configASSERT( AplTemp_hQue == 0 );
#endif
#ifdef TASK_SAMPLE
	// �^�X�N����
	retv = osTask_create( &AplTemp_Task, "AplTemp", AplTemp_STACKSZ/4,
						NULL, AplTempTASK_PRI | portPRIVILEGE_BIT, &handle );
	configASSERT( retv == pdPASS );
#endif
}


/**
 * @brief �����݃n���h������.
 */
void AplTemp_intr( void )
{
	portBASE_TYPE	dispatch = pdFALSE;
	AplTemp_MNGR_t	msg;

	/* �^�X�N���N������ */
#ifdef MUTEX_SAMPLE
	// MUTEX-give
	osMutex_giveISR( AplTemp_hMutex, &dispatch );
#endif
#ifdef QUEUE_SAMPLE
	osQue_sendISR( AplTemp_hQue, &msg, &dispatch );
#endif

	portEND_SWITCHING_ISR( dispatch );
}

/**
 * @} end of addtogroup
 */
