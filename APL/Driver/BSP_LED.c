/************************************************************************************//**
 * @file BSP_LED.c
 * @brief BluePillオンボードLED制御API
 *
 * @author  Teru
 * @date    2018/12/24
 * @version Rev0.10
 * @par 変更履歴:
 * - Rev0.10: 2018/12/24: 新規作成
 *
 * @copyright 2018 Emb-se.com. All rights reserved.
 */
/**
 * @addtogroup GROUP_BSP_LED
 * @{
 */
#include "BSP_LED.h"

/**
 * @brief オンボードLEDの初期設定
 *
 * LEDはプルアップ設定後、inactive(1)で初期化する。
 * @param なし
 * @return なし
 */
void BSP_LED_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	if( __HAL_RCC_GPIOC_IS_CLK_DISABLED() ) {
		__HAL_RCC_GPIOC_CLK_ENABLE();
	}

	/** @li LED初期設定 */
	BSP_LED_turnOFF();

	/* Configure GPIO pin */
	GPIO_InitStruct.Pin   = LED_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init( LED_POPT, &GPIO_InitStruct );
}

/**
 * @} end of addtogroup
 */
