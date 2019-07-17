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
/**
 * @addtogroup GROUP_TIMER
 * @{
 */
#include "TIMER.h"
#include "TIMms.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_bus.h"

/**
 * @brief タイマコントローラの初期設定.
 */
void TIMER_init( void )
{
  LL_TIM_InitTypeDef TIM_InitStruct = {0};
  LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

  /* TIM4 interrupt Init */
  NVIC_SetPriority(TIM4_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),5, 0));
  NVIC_EnableIRQ(TIM4_IRQn);

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  TIM_InitStruct.Prescaler = 6000;
  TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
  TIM_InitStruct.Autoreload = 0xffff;
  TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
  LL_TIM_Init(TIM4, &TIM_InitStruct);
  LL_TIM_DisableARRPreload(TIM4);
  LL_TIM_SetClockSource(TIM4, LL_TIM_CLOCKSOURCE_INTERNAL);
  TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_ACTIVE;
  TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
  TIM_OC_InitStruct.CompareValue = 0;
  TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_LOW;
  LL_TIM_OC_Init(TIM4, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
  LL_TIM_OC_DisableFast(TIM4, LL_TIM_CHANNEL_CH1);
  LL_TIM_SetTriggerOutput(TIM4, LL_TIM_TRGO_CC1IF);
  LL_TIM_DisableMasterSlaveMode(TIM4);

  LL_TIM_EnableIT_CC1(TIM4);
  /* カウンタスタート */
	__HAL_DBGMCU_FREEZE_TIM4();
  LL_TIM_EnableCounter(TIM4);
}

/**
 * @brief TIM4 割込みハンドラ.
 *
 * タイマ機能の発火処理をコールする.
 */
void TIM4_IRQHandler(void)
{
	int		over;
	/* コンペアマッチ */
	if( LL_TIM_IsActiveFlag_CC1(TIM4) ) {
		LL_TIM_ClearFlag_CC1( TIM4 );
		over = 0;
	}
	/* カウンタオーバーフロー */
	if( LL_TIM_IsActiveFlag_UPDATE(TIM4) ) {
		LL_TIM_ClearFlag_UPDATE( TIM4 );
		over = 1;
	}
	/** @li タイマ機能の発火処理 */
	TIMms_expire( over );
}

/**
 * @} end of addtogroup
 */
