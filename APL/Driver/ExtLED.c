/************************************************************************************//**
 * @file ExtLED.c
 * @brief 外部回路基板(External Print Circuit Board)LED制御API
 *
 * @author  Teru
 * @date    2019/06/19
 * @version Rev0.10
 * @par 変更履歴:
 * - Rev0.10: 2019/06/19: 新規作成.
 *
 * @copyright 2018 Emb-se.com. All rights reserved.
 */
/**
 * @addtogroup GROUP_ExtLED
 * @{
 */
#include "ExtLED.h"

/**
 * @brief 外部LEDの初期設定.
 *
 * LED1,LED2はプルアップ設定後、inactive(1)で初期化する.
 * @param なし.
 * @return なし.
 */
void ExtLED_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	if( __HAL_RCC_GPIOB_IS_CLK_DISABLED() ) {
		__HAL_RCC_GPIOB_CLK_ENABLE();
	}

	/** @li ExtLED1初期設定 */
	ExtLED1_turnOFF();

	/* Configure GPIO pin */
	GPIO_InitStruct.Pin   = ExtLED1_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init( ExtLED1_POPT, &GPIO_InitStruct );

	/** @li ExtLED2初期設定 */
	ExtLED2_turnOFF();

	/* Configure GPIO pin */
	GPIO_InitStruct.Pin   = ExtLED2_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init( ExtLED2_POPT, &GPIO_InitStruct );
}

/**
 * @} end of addtogroup
 */
