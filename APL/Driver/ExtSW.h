/************************************************************************************//**
 * @file ExtSW.h
 * @brief 外部回路基板SW制御API
 *
 * @author  Teru
 * @date    2019/06/19
 * @version Rev0.10
 * @par 変更履歴:
 * - Rev0.10: 2019/06/19: 新規作成.
 *
 * @copyright 2018 Emb-se.com. All rights reserved.
 */
#ifndef _ExtSW_H_
#define _ExtSW_H_

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

/**
 * @page PAGE_ExtSW 外部回路基板SW制御.
 *
 * @section SEC1_ExtSW 外部回路基板SW制御API.
 * |APIs     |機能名称|
 * |:--------|:------ |
 * |ExtSW_init() | 外部SWの初期設定 |
 */
/**
 * @addtogroup GROUP_ExtSW 外部回路基板SW制御.
 * @brief 外部回路基板SW制御APIを提供する.<br>
 * @details ExtSWはプルアップ接続される.<br>
 * @{
 */

#define ExtSW1_POPT			GPIOA
#define ExtSW1_PIN			GPIO_PIN_8
#define EXTI9_5_PRIORITY	5

/**
 * @brief 外部スイッチの初期設定.
 */
extern void ExtSW_init(void);

/**
 * @brief SW1割込み処理.
 */
extern void ExtSW1_interrupt(void);

/**
 * @} end of addtogroup
 */
#endif // _ExtSW_H_
