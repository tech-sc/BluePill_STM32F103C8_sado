/************************************************************************************//**
 * @file ExtLED.h
 * @brief 外部回路基板LED制御API
 *
 * @author  Teru
 * @date    2019/06/19
 * @version Rev0.10
 * @par 変更履歴:
 * - Rev0.10: 2019/06/19: 新規作成.
 *
 * @copyright 2018 Emb-se.com. All rights reserved.
 */
#ifndef _ExtLED_H_
#define _ExtLED_H_

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

/**
 * @page PAGE_ExtLED 外部回路基板LED制御.
 *
 * @section SEC1_ExtLED 外部回路基板LED制御API.
 * |APIs     |機能名称|
 * |:--------|:------ |
 * |ExtLED_init() | 外部回路基板LEDの初期設定 |
 * |ExtLED1_turnON() | 外部回路基板LED1の点灯 |
 * |ExtLED1_turnOFF() | 外部回路基板LED1の消灯 |
 * |ExtLED1_toggle() | 外部回路基板LED1のトグル出力 |
 */
/**
 * @addtogroup GROUP_ExtLED 外部回路基板LED制御.
 * @brief 外部回路基板LED制御APIを提供する。<br>
 * @details LEDはプルアップ接続される。<br>
 * @{
 */

#define ExtLED1_POPT		GPIOB
#define ExtLED1_PIN			GPIO_PIN_0
#define ExtLED2_POPT		GPIOB
#define ExtLED2_PIN			GPIO_PIN_1

/**
 * @brief 外部回路基板LEDの初期設定.
 *
 * LED1ポートはプルアップで初期設定後、inactive(1)で初期化する。
 * @param なし
 * @return なし
 */
extern void ExtLED_init(void);

/**
 * @brief 外部回路基板LEDXの点灯.
 *
 * LEDXポートのLEDを点灯する。
 * @param なし
 * @return なし
 */
#define ExtLED1_turnON()	HAL_GPIO_WritePin( ExtLED1_POPT, ExtLED1_PIN, GPIO_PIN_RESET )
#define ExtLED2_turnON()	HAL_GPIO_WritePin( ExtLED2_POPT, ExtLED2_PIN, GPIO_PIN_RESET )

/**
 * @brief 外部回路基板LEDXの消灯.
 *
 * LEDXポートのLEDを消灯する。
 * @param なし
 * @return なし
 */
#define ExtLED1_turnOFF()	HAL_GPIO_WritePin( ExtLED1_POPT, ExtLED1_PIN, GPIO_PIN_SET )
#define ExtLED2_turnOFF()	HAL_GPIO_WritePin( ExtLED2_POPT, ExtLED2_PIN, GPIO_PIN_SET )

/**
 * @brief 外部回路基板LEDXのトグル出力
 *
 * LEDXポートのLEDを点灯または消灯にトグルする。
 * @param なし
 * @return なし
 */
#define ExtLED1_toggle()	HAL_GPIO_TogglePin( ExtLED1_POPT, ExtLED1_PIN )
#define ExtLED2_toggle()	HAL_GPIO_TogglePin( ExtLED2_POPT, ExtLED2_PIN )


#endif // _ExtLED_H_
/**
 * @} end of addtogroup
 */
