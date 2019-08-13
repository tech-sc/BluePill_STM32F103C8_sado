/**
 * @file  TIMER_us.h
 * @brief タイマコントローラ(マイクロ秒)の制御API.
 * 
 * μ秒タイマコントローラの制御APIを提供する.
 * 
 * @author      Teru
 * @date         2019/08/04
 * @version     Rev0.01
 * 
 * @par 変更履歴:
 * - Rev0.01: 2019/08/04: 新規作成
 * 
 * @copyright   2019 Emb-se.com.
 */
#ifndef _TIMER_US_H_
#define _TIMER_US_H_

#include "stm32_assert.h"
#include "stm32f1xx_ll_tim.h"

/**
 * @page PAGE_TIMER_US タイマコントローラ制御.
 *
 * @section SEC1_TIMER_US タイマコントローラ制御API.
 * | APIs              | 機能名称    |
 * |:------------------ |:---------- |
 * | TIMER_init()       | タイマコントローラの初期設定 |
 * | TIMER_setCompReg() | コンペアマッチ値セット |
 *
 * @section TIMER_US_usage 使用方法.
 * - 予め一度だけ TIMER_init() を呼び出す.
 */
/**
 * @addtogroup GROUP_TIMER_US タイマコントローラ制御.
 * @brief タイマコントローラ制御APIを提供する.<br>
 * @{
 */

/// μ秒タイマコントローラ
#define TIMER_US_DEV					TIM3
/// μ秒タイマコントローラIRQn
#define TIMER_US_IRQ					TIM3_IRQn
/**
 * @brief タイマコントローラの初期設定.
 */
extern void TIMER_US_init( void );

/// μ秒コンペアマッチ値セット.
#define TIMER_us_setCompReg( val )		LL_TIM_OC_SetCompareCH1( TIMER_US_DEV, val )
/// μ秒タイマカウンタ取得
#define TIMER_us_getTick()				LL_TIM_GetCounter( TIMER_US_DEV )


/**
 * @brief TIM3 割込みハンドラ.
 */
extern void TIM3_IRQHandler(void);

/**
 * @} end of addtogroup
 */
#endif //_TIMER_US_H_
