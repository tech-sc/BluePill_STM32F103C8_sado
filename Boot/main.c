/************************************************************************************//**
 * @file    main.c
 * @brief   STM32F103C8 BluePill
 *
 * @mainpage STM32F103-BluePill������v���W�F�N�g
 *
 * @brief   �g���݃V�X�e���Z�p��p�v���W�F�N�g�ł���.<br>
 *
 * @details STM32CubeF1���C�u�������g�p���Ď�������.<br>
 *
 * @par �ύX����:
 *	Rev0.01�@2019/06/16�@�e���v���[�g�쐬
 * @par Copyright
 *	2014-19 Emb-se.com
 */
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_cortex.h"
#include "BSP_LED.h"
#include "ExtLED.h"
#include "ExtSW.h"
#include "os.h"
#include "apl_init.h"

/**
 * @brief �I���{�[�h�y���t�F���������ݒ�
 * @retval None
 */
static void BSP_init(void)
{
	/* �g�p����@�\�ŗL���ɂ��Ă��� */
	//LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
	/* RTC��L���ɂ���ۂɕK�v */
	//LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	/* LED��TACT-SW�̏����ݒ� */
	BSP_LED_init();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
	while(1);
}

/**
 * @brief System Clock Configuration
 *
 * @detail HSE=8MHz�AAPB1-PCLK=12MHz(for MCP23017(I2C1))�AAPB1-TCLK=24MHz
 *         SYSTICK, APB2-PCLK, APB2-CLK, HCLK, AHB=48MHz
 * @retval None
 */
static void SystemClock_Config(void)
{
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

	if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1)
	{
		Error_Handler();
	}
	LL_RCC_HSE_Enable();

	/* Wait till HSE is ready */
	while(LL_RCC_HSE_IsReady() != 1);

	LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_6);
	LL_RCC_PLL_Enable();

	/* Wait till PLL is ready */
	while(LL_RCC_PLL_IsReady() != 1);

	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

	/* Wait till System clock is ready */
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

	LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
	//LL_Init1msTick(48000000);
	//LL_SetSystemCoreClock(48000000);
}

/************************************************************************************//**
 * @brief   �v���O�������C��
 * @param   None
 * @retval  None
 */
void main( void )
{
	SystemClock_Config();

	BSP_init();
	ExtLED_init();
	ExtSW_init();

	apl_init();

	osKernel_start();
	while(1);
}
