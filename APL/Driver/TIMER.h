/**
 * @file  TIMER.c
 * @brief タイマコントローラへの制御API.
 * 
 * タイマコントローラへの制御APIを提供する.
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
/**
 * @mainpage
 * テンプレート・トップページ
 * 
 * @ref TIMER_title
 */
/**
 * @page TIMER_title タイマライブラリ
 *
 * @section TIMER_api API一覧
 * | API名              | 機能       |
 * |:------------------ |:---------- |
 * | TIMER_init()       | タイマ初期設定 |
 * | TIMER_setCompReg() | コンペアマッチ値セット |
 *
 * @section TIMER_usage 使用方法
 * - 予め一度だけ TIMER_init() を呼び出す。
 */
#ifndef _TIMER_H_
#define _TIMER_H_

//#include "stm32f1xx_hal.h"
//#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_ll_tim.h"

typedef uint16_t    TIMCNT_t;	/**< タイマカウンタレジスタ型(16bitカウンタ) */

/**
 * @brief タイマ初期設定
 */
extern void TIMER_init( void );

/**
 * @brief コンペアマッチ値セット
 */
#define TIMER_setCompReg( val )		LL_TIM_OC_SetCompareCH1(TIM4, val)


/**
 * @brief TIM4 割込みハンドラ
 */
extern void TIM4_IRQHandler(void);

#endif
