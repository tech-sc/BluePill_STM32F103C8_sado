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
#ifndef _BSP_LED_H_
#define _BSP_LED_H_

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

/**
 * @page PAGE_BSP_LED BluePillオンボード
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
 * @addtogroup GROUP_BSP_LED BluePillオンボードLED制御
 * @brief BluePillオンボードLED制御APIを提供する。<br>
 * @details LEDは、PC13にプルアップ接続される。<br>
 * @{
 */

#define LED_POPT		GPIOC
#define LED_PIN			GPIO_PIN_13

/**
 * @brief オンボードLEDの初期設定.
 */
extern void BSP_LED_init(void);

/**
 * @brief オンボードLEDの点灯.
 *
 * LEDを点灯する.
 * @param なし.
 * @return なし.
 */
#define BSP_LED_turnON()		HAL_GPIO_WritePin( LED_POPT, LED_PIN, GPIO_PIN_RESET )

/**
 * @brief オンボードLEDの消灯.
 *
 * LEDを消灯する.
 * @param なし.
 * @return なし.
 */
#define BSP_LED_turnOFF()		HAL_GPIO_WritePin( LED_POPT, LED_PIN, GPIO_PIN_SET )

/**
 * @brief オンボードLEDのトグル出力.
 *
 * LEDを点灯または消灯にトグルする.
 * @param なし.
 * @return なし.
 */
#define BSP_LED_toggle()		HAL_GPIO_TogglePin( LED_POPT, LED_PIN )

/**
 * @} end of addtogroup
 */

#endif //_BSP_LED_H_
