/**************************************************************************//**
 * @file	apl_init.c
 * @brief	�A�v�������N���^�X�N
 * @version V0.10.0
 * @date	2019/07/04
 * @author	Teru
 */
#include "os.h"
#include "TIMms.h"
#include "ExtLED.h"
#include "BSP_LED.h"
#include "shell.h"
#include "apl_init.h"

/**************************************************************************//**
 * @brief	�萔��`
 */

/**************************************************************************//**
 * @brief	�֐���`
 */
static void tmX_expire( void *handle );

/**************************************************************************//**
 * @brief	�ϐ���`
 */
osMutexHandle_t     expire_mutex;
TIMms_t				tm1;

/**************************************************************************//**
 * @brief	�����N���^�X�N�𐶐�����
 */
void apl_init( void )
{
	int		retv;
	xTaskHandle		handle;

	TIMms_initTimer();
	shell_init();

    osMutex_create( expire_mutex );
    osMutex_take( expire_mutex, portMAX_DELAY );

	retv = osTask_create( &apl_initTask, "APL_init", APLinitTASK_STACKSZ/4,
						NULL, APLinitTASK_PRI | portPRIVILEGE_BIT, &handle );
	configASSERT(retv == pdPASS);
}

/**************************************************************************//**
 * @brief	�����N���^�X�N
 * @param[in]	arg		�^�X�N�N���p�����[�^
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
	ExtLED2_toggle();
	BSP_LED_toggle();
	TIMms_reqTimer( 500, &tmX_expire, handle );
}