/**
 * @file  TIMER.c
 * @brief タイマコントローラの制御API.
 * 
 * タイマコントローラの制御APIを提供する.
 * 
 * @author      Teru
 * @date        2019.06.27
 * @version     Rev0.01
 * 
 * @par 変更履歴:
 * - Rev0.01: 2019.06.27: 新規作成
 * 
 * @copyright   2019 Emb-se.com.
 */
#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32_assert.h"
#include "stm32f1xx_ll_tim.h"

/**
 * @page PAGE_TIMER タイマコントローラ制御.
 *
 * @section SEC1_TIMER タイマコントローラ制御API.
 * | APIs              | 機能名称    |
 * |:------------------ |:---------- |
 * | TIMER_init()       | タイマコントローラの初期設定 |
 * | TIMER_setCompReg() | コンペアマッチ値セット |
 *
 * @section TIMER_usage 使用方法.
 * - 予め一度だけ TIMER_init() を呼び出す.
 */
/**
 * @addtogroup GROUP_TIMER タイマコントローラ制御.
 * @brief タイマコントローラ制御APIを提供する.<br>
 * @{
 */

/**
 * @brief タイマコントローラの初期設定.
 */
extern void TIMER_init( void );

/**
 * @brief コンペアマッチ値セット.
 */
#define TIMER_setCompReg( val )		LL_TIM_OC_SetCompareCH1(TIM4, val)
#define TIMER_getTick()				LL_TIM_GetCounter(TIM4)


/**
 * @brief TIM4 割込みハンドラ.
 */
extern void TIM4_IRQHandler(void);

/**
 * @} end of addtogroup
 */
#endif
