/************************************************************************************//**
 * @file stm32f1xx_it.c
 * @brief   Interrupt Service Routines.
 *
 * @author  Teru
 * @date    2019/06/19
 * @version Rev0.10
 * @par �ύX����:
 * - Rev0.10: 2019/06/19: �V�K�쐬.
 *
 * @copyright 2019 Emb-se.com.
 */
/**
 * @addtogroup GROUP_Interrupt �����݃n���h��.
 * @{
 */
#include "ExtSW.h"
#include "UART.h"

/**************************************************************************//**
 * @brief �ϐ���`
 */
/** UART����n���h�� */
extern UART_HandleTypeDef	UART_HandleStruct = {0};


/**************************************************************************//**
 * @brief This function handles Non maskable interrupt.
 */
void NMI_Handler(void)
{
	while(1);
}

/**************************************************************************//**
 * @brief This function handles Hard fault interrupt.
 */
void HardFault_Handler(void)
{
	while(1);
}

/**************************************************************************//**
 * @brief This function handles Memory management fault.
 */
void MemManage_Handler(void)
{
	while(1);
}

/**************************************************************************//**
 * @brief This function handles Prefetch fault, memory access fault.
 */
void BusFault_Handler(void)
{
	while(1);
}

/**************************************************************************//**
 * @brief This function handles Undefined instruction or illegal state.
 */
void UsageFault_Handler(void)
{
	while(1);
}

/**************************************************************************//**
 * @brief This function handles Debug monitor.
 */
void DebugMon_Handler(void)
{
	while(1);
}


/**************************************************************************//**
 * @brief This function handles PVD interrupt through EXTI line 16.
 */
void PVD_IRQHandler(void)
{
	HAL_PWR_PVD_IRQHandler();
}

/**************************************************************************//**
 * @brief This function handles EXTI line[9:5] interrupts.
 */
void EXTI9_5_IRQHandler(void)
{
	ExtSW_Handler();
}

/**
 * @} end of addtogroup
 */
