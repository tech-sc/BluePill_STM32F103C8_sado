/**************************************************************************//**
 * @file	apl_init.c
 * @brief	アプリ初期起動タスク
 * @version V0.10.0
 * @date	2019/07/04
 * @author	Teru
 */
#include "BSP_LED.h"
#include "ExtLED.h"
#include "TIMms.h"
#include "LOG.h"
#include "shell.h"
#include "os.h"
#include "apl_init.h"
#include "SW_Ctrl.h"

/**************************************************************************//**
 * @brief	定数定義
 */

/**************************************************************************//**
 * @brief	関数定義
 */
static void tmX_expire( void *handle );

/**************************************************************************//**
 * @brief	変数定義
 */
osMutexHandle_t     expire_mutex;
TIMms_t				tm1;

/**************************************************************************//**
 * @brief	初期起動タスクを生成する
 */
void apl_init( void )
{
	int		retv;
	xTaskHandle		handle;

	TIMms_initTimer();
	shell_init();
	LOG_init( LOG_DEBUG );
    SwCtrl_init();

	printf("%s\n", __FUNCTION__);
    expire_mutex = osMutex_create();

	retv = osTask_create( &apl_initTask, "APL_init", APLinit_STACKSZ/4,
						NULL, APLinitTASK_PRI | portPRIVILEGE_BIT, &handle );
	configASSERT(retv == pdPASS);
}

/**************************************************************************//**
 * @brief	初期起動タスク
 * @param[in]	arg		タスク起動パラメータ
 */
void apl_initTask( void *arg )
{
	TIMms_reqTimer( 500, &tmX_expire, &tm1 );

    while( 1 ){
        osMutex_take( expire_mutex, portMAX_DELAY );
	}
}

/**************************************************************************/
static void tmX_expire( void *handle )
{
	LOG_write( LOG_DEBUG, __LINE__, 0x0010, 2, TAG_STR, "500ms Expire" );

	BSP_LED_toggle();
	TIMms_reqTimer( 500, &tmX_expire, handle );
}