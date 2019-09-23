/**
 * @file  AplTemplate.c
 * @brief アプリテンプレート.
 * 
 * RTOSリソースを使用するアプリケーションテンプレートである.
 * 
 * @author		Teru
 * @date        2019/09/22
 * @version 	Rev0.10
 * @copyright	2019 Emb-se.com.
 * 
 * @par 変更履歴:
 * - Rev0.01: 2019/09/22: 新規作成.
 * 
 * @copyright	2019 Emb-se.com.
 */
/**
 * @addtogroup AplTemp_GROUP アプリテンプレート機能.
 * @{
 */
#include "os.h"
#include "AplTemplate.h"

#define MUTEX_SAMPLE
#define QUEUE_SAMPLE
#define TASK_SAMPLE

/**
 * @brief APIリターンコード.
 */
typedef enum AplTemp_STATE_e {
	AplTemp_IDLE		= 0,		/**< IDLE */
	AplTemp_MAX_STATE
}AplTemp_STATE_e;

/**
 * @brief APL管理データデータ型.
 */
typedef struct AplTemp_MNGR_t {
	/// ステート
	AplTemp_STATE_e		state;
}AplTemp_MNGR_t;


/// APL管理データ.
static AplTemp_MNGR_t	AplTemp_MNGR;

#ifdef MUTEX_SAMPLE
	/// MUTEXハンドル
	static osMutexHandle_t	AplTemp_hMutex;
#endif
#ifdef QUEUE_SAMPLE
	// QUEUEハンドル
	static osQueHandle_t	AplTemp_hQue;
#endif

/**
 * @brief ミリ秒タイマ発火処理タスク.
 * @param[in]	arg  未使用.
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
 * @brief 初期設定API.
 *
 * 初期設定及びOSリソースの生成.
 */
void AplTemp_init( void )
{
	osTaskHandle_t	handle;
	int		retv;

	/* 内部管理データ初期化 */

	/* OSリソース生成 */
#ifdef MUTEX_SAMPLE
	// MUTEX生成
	AplTemp_hMutex = osMutex_create();
	configASSERT( AplTemp_hMutex == 0 );
#endif
#ifdef QUEUE_SAMPLE
	AplTemp_hQue = osQue_create( 3, sizeof(AplTemp_MNGR_t) );
	configASSERT( AplTemp_hQue == 0 );
#endif
#ifdef TASK_SAMPLE
	// タスク生成
	retv = osTask_create( &AplTemp_Task, "AplTemp", AplTemp_STACKSZ/4,
						NULL, AplTempTASK_PRI | portPRIVILEGE_BIT, &handle );
	configASSERT( retv == pdPASS );
#endif
}


/**
 * @brief 割込みハンドラ処理.
 */
void AplTemp_intr( void )
{
	portBASE_TYPE	dispatch = pdFALSE;
	AplTemp_MNGR_t	msg;

	/* タスクを起床する */
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
