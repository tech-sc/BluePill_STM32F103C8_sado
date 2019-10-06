/************************************************************************************//**
 * @file ExtSW.c
 * @brief 外部回路基板(External Print Circuit Board)SW制御API
 *
 * @author  Teru
 * @date    2019/06/19
 * @version Rev0.10
 * @par 変更履歴:
 * - Rev0.10: 2019/06/19: 新規作成.
 *
 * @copyright 2018 Emb-se.com. All rights reserved.
 */
/**
 * @addtogroup GROUP_ExtSW 外部回路基板LED制御.
 * @{
 */
#include "ExtLED.h"
#include "ExtSW.h"

/**
 * @brief 外部SWの初期設定.
 *
 * SWはプルアップで初期設定する.
 * @param なし.
 * @return なし.
 */
void ExtSW_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	if( __HAL_RCC_GPIOA_IS_CLK_DISABLED() ) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}

	/** @li ExtSW1初期設定 */
	/* Configure GPIO pin */
	GPIO_InitStruct.Pin   = ExtSW1_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	HAL_GPIO_Init( ExtSW1_POPT, &GPIO_InitStruct );

	/** @li EXTI[9:5]割込みの設定 */
	/* EXTI line[9:5] interrupt init */
	HAL_NVIC_SetPriority( EXTI9_5_IRQn, EXTI9_5_PRIORITY, 0 );
	HAL_NVIC_EnableIRQ( EXTI9_5_IRQn );
}

/**
 * @brief SW状態処理割込みハンドラ.
 *
 * 立下り(active)エッジ割込み時にコールされる.
 * @param[in] sw_no Active状態に変化したスイッチ番号.
 * @return なし.
 */
__weak void ExtSW_activeHandler( ExtSW sw_no )
{
}

/**
 * @brief SW状態処理割込みハンドラ.
 *
 * 立上がり(inactive)エッジ割込み時にコールされる.
 * @param[in] sw_no Inactive状態に変化したスイッチ番号.
 * @return なし.
 */
__weak void ExtSW_inactiveHandler( ExtSW sw_no )
{
}

/**
 * @brief SW割込み処理.
 *
 * 立下り(active)または立上がり(inactive)エッジ割込みを処理する.
 * @param なし.
 * @return なし.
 */
inline void ExtSW_Handler(void)
{
	/* EXTI line interrupt detected */
	if( __HAL_GPIO_EXTI_GET_IT(ExtSW1_PIN) ) {
		__HAL_GPIO_EXTI_CLEAR_IT( ExtSW1_PIN );
		if( HAL_GPIO_ReadPin( ExtSW1_POPT, ExtSW1_PIN ) == GPIO_PIN_SET ) {
			ExtSW_inactiveHandler( ExtSW1 );
		} else {
			ExtSW_activeHandler( ExtSW1 );
		}
	}
}

/**
 * @} end of addtogroup
 */
