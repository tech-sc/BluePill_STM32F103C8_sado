/**
 * @file  TIMER_us.c
 * @brief タイマコントローラ(マイクロ秒)の制御API.
 * 
 * μ秒タイマコントローラの制御APIを提供する.
 * 
 * @author      Teru
 * @date        2019/08/04
 * @version     Rev0.10
 * 
 * @par 変更履歴:
 * - Rev0.01: 2019/08/04: 新規作成
 * 
 * @copyright   2019 Emb-se.com.
 */
/**
 * @addtogroup GROUP_TIMER_US
 * @{
 */
#include "TIMER_us.h"

#include "stm32f1xx_hal.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_bus.h"

/**
 * @brief タイマコントローラをμ秒カウンタとして初期設定する.
 */
void TIMER_us_init( void )
{
	LL_TIM_InitTypeDef TIM_InitStruct = {0};
	LL_TIM_OC_InitTypeDef TIM_OC_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
	__HAL_DBGMCU_FREEZE_TIM3();

	/* TIM3 interrupt Init */
	NVIC_SetPriority( TIMER_US_IRQ, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 5, 0) );
	NVIC_EnableIRQ( TIMER_US_IRQ );

	/* setup */
	TIM_InitStruct.Prescaler = 6;
	TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
	TIM_InitStruct.Autoreload = 0xffff;
	TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	LL_TIM_Init(TIM3, &TIM_InitStruct);
	LL_TIM_DisableARRPreload( TIMER_US_DEV );
	LL_TIM_SetClockSource( TIMER_US_DEV, LL_TIM_CLOCKSOURCE_INTERNAL );
	TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_ACTIVE;
	TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
	TIM_OC_InitStruct.CompareValue = 0;
	TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_LOW;
	LL_TIM_OC_Init( TIMER_US_DEV, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct );
	LL_TIM_OC_DisableFast( TIMER_US_DEV, LL_TIM_CHANNEL_CH1 );
	LL_TIM_SetTriggerOutput( TIMER_US_DEV, LL_TIM_TRGO_CC1IF );
	LL_TIM_DisableMasterSlaveMode( TIMER_US_DEV );

	/* カウンタスタート */
	LL_TIM_EnableIT_CC1( TIMER_US_DEV );
	LL_TIM_EnableCounter( TIMER_US_DEV );
}

/**
 * @brief μ秒タイマ発火処理.
 */
__weak void TIMus_expire( int over )
{
}

/**
 * @brief TIM3 割込みハンドラ.
 *
 * μ秒タイマの発火処理をコールする.
 */
void TIM3_IRQHandler(void)
{
	int		over;

	/* コンペアマッチ */
	if( LL_TIM_IsActiveFlag_CC1( TIMER_US_DEV ) ) {
		LL_TIM_ClearFlag_CC1( TIMER_US_DEV );
		over = 0;
	}
	/* カウンタオーバーフロー */
	if( LL_TIM_IsActiveFlag_UPDATE( TIMER_US_DEV ) ) {
		LL_TIM_ClearFlag_UPDATE( TIMER_US_DEV );
		over = 1;
	}
	/** @li タイマの発火処理 */
	TIMus_expire( over );
}

/**
 * @} end of addtogroup
 */
