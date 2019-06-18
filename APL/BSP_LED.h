/************************************************************************************//**
 * @file BSP_LED.h
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
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

/**
 * @page PAGE_BluePill_LED BluePillオンボード
 *
 * @section SEC1_BluePill_LED BluePillオンボードLED制御API
 * |APIs     |機能名称|
 * |:--------|:------ |
 * |BSP_LED_init() | オンボードLEDの初期設定 |
 * |BSP_LED_turnON() | オンボードLEDの点灯 |
 * |BSP_LED_turnOFF() | オンボードLEDの消灯 |
 * |BSP_LED_toggle() | オンボードLEDのトグル出力 |
 */
/**
 * @addtogroup GROUP_BluePill_LED BluePillオンボードLED制御
 * @brief BluePillオンボードLED制御APIを提供する。<br>
 * @details LEDは、PC13にプルアップ接続される。<br>
 * @{
 */

#define LED_POPT		GPIOC
#define LED_PIN			GPIO_PIN_13

/**
 * @brief オンボードLEDの初期設定
 *
 * GPIO(PC13)はプルアップで初期設定後、inactive(1)で初期化する。
 * @param なし
 * @return なし
 */
extern void BSP_LED_init(void);

/**
 * @brief オンボードLEDの点灯
 *
 * GPIO(PC13)のLEDを点灯する。
 * @param なし
 * @return なし
 */
#define BSP_LED_turnON()		HAL_GPIO_WritePin( LED_POPT, LED_PIN, GPIO_PIN_RESET )

/**
 * @brief オンボードLEDの消灯
 *
 * GPIO(PC13)のLEDを消灯する。
 * @param なし
 * @return なし
 */
#define BSP_LED_turnOFF()		HAL_GPIO_WritePin( LED_POPT, LED_PIN, GPIO_PIN_SET )

/**
 * @brief オンボードLEDのトグル出力
 *
 * GPIO(PC13)のLEDを点灯または消灯にトグルする。
 * @param なし
 * @return なし
 */
#define BSP_LED_toggle()		HAL_GPIO_TogglePin( LED_POPT, LED_PIN )

/**
 * @} end of addtogroup
 */
