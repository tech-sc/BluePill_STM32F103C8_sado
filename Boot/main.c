/************************************************************************************//**
 * @file    main.c
 * @brief   STM32F103C8 BluePill
 *
 * @mainpage STM32F103-BluePill基板向けプロジェクト
 * @brief   組込みシステム技術会合用プロジェクトである.<br>
 *
 * @details STM32CubeF1ライブラリを使用して実装する.<br>
 *
 * @par 変更履歴:
 *	Rev0.01　2019/06/16　テンプレート作成<br>
 * @par Copyright
 *	2014-19 Emb-se.com All rights reserved.
 */
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "BSP_LED.h"
#include "ExtLED.h"
#include "ExtSW.h"
#include "os.h"
#include "apl_init.h"

/**
 * @brief オンボードペリフェラル初期設定
 * @retval None
 */
static void BSP_init(void)
{
//	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	/* LEDとTACT-SWの初期設定 */
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
 * @detail HSE=8MHz、HSI=32.768KHz、APB1-PCLK=12MHz(for MCP23017(I2C1))、APB1-CLK=24MHz
 *         SYSTICK, APB2-PCLK, APB2-CLK, HCLK, AHB=48MHz
 * @retval None
 */
static void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
	RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

	/**Initializes the CPU, AHB and APB busses clocks 
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/**Initializes the CPU, AHB and APB busses clocks 
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

/************************************************************************************//**
 * @brief   プログラムメイン
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
	//Reset->LED ON(2.4sec)
	for(;;);
}
