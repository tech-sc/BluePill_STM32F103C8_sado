/************************************************************************************//**
 * @file ExtSW.c
 * @brief �O����H���(External Print Circuit Board)SW����API
 *
 * @author  Teru
 * @date    2019/06/19
 * @version Rev0.10
 * @par �ύX����:
 * - Rev0.10: 2019/06/19: �V�K�쐬.
 *
 * @copyright 2018 Emb-se.com. All rights reserved.
 */
/**
 * @addtogroup GROUP_ExtSW �O����H���LED����.
 * @{
 */
#include "ExtLED.h"
#include "ExtSW.h"

/**
 * @brief �O����H���SW�̏����ݒ�.
 *
 * ExtSW1�|�[�g�̓v���A�b�v�ŏ����ݒ肷��.
 * @param �Ȃ�.
 * @return �Ȃ�.
 */
void ExtSW_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	if( __HAL_RCC_GPIOA_IS_CLK_DISABLED() ) {
		__HAL_RCC_GPIOA_CLK_ENABLE();
	}

	/** Configure GPIO pin */
	GPIO_InitStruct.Pin   = ExtSW1_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_IT_FALLING;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	HAL_GPIO_Init( ExtSW1_POPT, &GPIO_InitStruct );

	/* EXTI line[9:5] interrupt init */
	HAL_NVIC_SetPriority( EXTI9_5_IRQn, EXTI9_5_PRIORITY, 0 );
	HAL_NVIC_EnableIRQ( EXTI9_5_IRQn );
}

/**
 * @brief SW1�����ݏ���.
 *
 * ������(active)�܂��͗��オ��(inactive)�G�b�W�����݂���������.
 * @param �Ȃ�.
 * @return �Ȃ�.
 */
void ExtSW1_interrupt(void)
{
	ExtLED1_toggle();
}

/**
 * @} end of addtogroup
 */