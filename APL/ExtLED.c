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
 * @addtogroup GROUP_ExtLED 外部回路基板LED制御.
 * @{
 */
#include "ExtLED.h"

/**
 * @brief LEDの初期設定
 *
 * LEDポートはプルアップで初期設定後、inactive(1)で初期化する。
 * @param なし
 * @return なし
 */
void ExtLED_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	if( __HAL_RCC_GPIOA_IS_CLK_DISABLED() ) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}

	/* Initialize GPIO pin Output Level : High */
	HAL_GPIO_WritePin( ExtLED1_POPT, ExtLED1_PIN, GPIO_PIN_SET );

	/* Configure GPIO pin */
	GPIO_InitStruct.Pin   = ExtLED1_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init( ExtLED1_POPT, &GPIO_InitStruct );
}

/**
 * @} end of addtogroup
 */
