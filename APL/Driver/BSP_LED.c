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
 * @addtogroup GROUP_BluePill_LED BluePillオンボードLED制御
 * @{
 */
#include "BSP_LED.h"

/**
 * @brief オンボードLEDの初期設定
 *
 * GPIO(PC13)はプルアップで初期設定後、inactive(1)で初期化する。
 * @param なし
 * @return なし
 */
void BSP_LED_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* Initialize GPIO pin Output Level : High */
	HAL_GPIO_WritePin( LED_POPT, LED_PIN, GPIO_PIN_SET );

	/* Configure GPIO pin : LED_PC13_Pin */
	GPIO_InitStruct.Pin   = LED_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init( LED_POPT, &GPIO_InitStruct );
}

#if 0
/**
 * @brief オンボードLEDの点灯
 *
 * GPIO(PC13)のLEDを点灯する。
 * @param なし
 * @return なし
 */
void BSP_LED_turnON(void)
{
	HAL_GPIO_WritePin( LED_POPT, LED_PIN, GPIO_PIN_RESET );
}

/**
 * @brief オンボードLEDの消灯
 *
 * GPIO(PC13)のLEDを消灯する。
 * @param なし
 * @return なし
 */
void BSP_LED_turnOFF(void)
{
	HAL_GPIO_WritePin( LED_POPT, LED_PIN, GPIO_PIN_SET );
}

/**
 * @brief オンボードLEDのトグル出力
 *
 * GPIO(PC13)のLEDを点灯または消灯にトグルする。
 * @param なし
 * @return なし
 */
void BSP_LED_toggle(void)
{
	HAL_GPIO_TogglePin( LED_POPT, LED_PIN );
}
#endif // FALSE

/**
 * @} end of addtogroup
 */
