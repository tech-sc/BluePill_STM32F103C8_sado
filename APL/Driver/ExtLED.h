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
 * |ExtLED_init() | 外部LEDの初期設定 |
 * |ExtLED1_turnON() | 外部LED1の点灯 |
 * |ExtLED1_turnOFF() | 外部LED1の消灯 |
 * |ExtLED1_toggle() | 外部LED1のトグル出力 |
 * |ExtLED2_turnON() | 外部LED2の点灯 |
 * |ExtLED2_turnOFF() | 外部LED2の消灯 |
 * |ExtLED2_toggle() | 外部LED2のトグル出力 |
 */
/**
 * @addtogroup GROUP_ExtLED 外部回路基板LED制御.
 * @brief 外部回路基板LED制御APIを提供する.<br>
 * @details LEDはプルアップ接続される.<br>
 * @{
 */

#define ExtLED1_POPT		GPIOB
#define ExtLED1_PIN			GPIO_PIN_0
#define ExtLED2_POPT		GPIOB
#define ExtLED2_PIN			GPIO_PIN_1

/**
 * @brief 外部LEDの初期設定.
 *
 */
extern void ExtLED_init(void);

/**
 * @brief 外部LED1の点灯.
 *
 * LED1を点灯する.
 * @param なし.
 * @return なし.
 */
#define ExtLED1_turnON()	HAL_GPIO_WritePin( ExtLED1_POPT, ExtLED1_PIN, GPIO_PIN_RESET )

/**
 * @brief 外部LED2の点灯.
 *
 * LED2を点灯する.
 * @param なし.
 * @return なし.
 */
#define ExtLED2_turnON()	HAL_GPIO_WritePin( ExtLED2_POPT, ExtLED2_PIN, GPIO_PIN_RESET )

/**
 * @brief 外部LED1の消灯.
 *
 * LED1を消灯する.
 * @param なし.
 * @return なし.
 */
#define ExtLED1_turnOFF()	HAL_GPIO_WritePin( ExtLED1_POPT, ExtLED1_PIN, GPIO_PIN_SET )

/**
 * @brief 外部LED2の消灯.
 *
 * LED2を消灯する.
 * @param なし.
 * @return なし.
 */
#define ExtLED2_turnOFF()	HAL_GPIO_WritePin( ExtLED2_POPT, ExtLED2_PIN, GPIO_PIN_SET )

/**
 * @brief 外部LED1のトグル出力.
 *
 * LED1を点灯または消灯にトグルする.
 * @param なし.
 * @return なし.
 */
#define ExtLED1_toggle()	HAL_GPIO_TogglePin( ExtLED1_POPT, ExtLED1_PIN )

/**
 * @brief 外部LED2のトグル出力.
 *
 * LED2を点灯または消灯にトグルする.
 * @param なし.
 * @return なし.
 */
#define ExtLED2_toggle()	HAL_GPIO_TogglePin( ExtLED2_POPT, ExtLED2_PIN )

/**
 * @brief 外部LED2の点灯・消灯.
 */
#define ExtLED_turnON(port, pin)	HAL_GPIO_WritePin( port, pin, GPIO_PIN_RESET )
#define ExtLED_turnOFF(port, pin)	HAL_GPIO_WritePin( port, pin, GPIO_PIN_RESET )

/**
 * @} end of addtogroup
 */
#endif // _ExtLED_H_
