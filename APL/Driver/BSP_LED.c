/************************************************************************************//**
 * @file BSP_LED.c
 * @brief BluePill�I���{�[�hLED����API
 *
 * @author  Teru
 * @date    2018/12/24
 * @version Rev0.10
 * @par �ύX����:
 * - Rev0.10: 2018/12/24: �V�K�쐬
 *
 * @copyright 2018 Emb-se.com. All rights reserved.
 */
/**
 * @addtogroup GROUP_BluePill_LED BluePill�I���{�[�hLED����
 * @{
 */
#include "BSP_LED.h"

/**
 * @brief �I���{�[�hLED�̏����ݒ�
 *
 * GPIO(PC13)�̓v���A�b�v�ŏ����ݒ��Ainactive(1)�ŏ���������B
 * @param �Ȃ�
 * @return �Ȃ�
 */
void BSP_LED_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	/* Initialize GPIO pin Output Level : High */
	HAL_GPIO_WritePin( LED_POPT, LED_PIN, GPIO_PIN_SET );

	/* Configure GPIO pin : LED_PC13_Pin */
	GPIO_InitStruct.Pin   = LED_PIN;
	GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull  = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init( LED_POPT, &GPIO_InitStruct );
}

#if 0
/**
 * @brief �I���{�[�hLED�̓_��
 *
 * GPIO(PC13)��LED��_������B
 * @param �Ȃ�
 * @return �Ȃ�
 */
void BSP_LED_turnON(void)
{
	HAL_GPIO_WritePin( LED_POPT, LED_PIN, GPIO_PIN_RESET );
}

/**
 * @brief �I���{�[�hLED�̏���
 *
 * GPIO(PC13)��LED����������B
 * @param �Ȃ�
 * @return �Ȃ�
 */
void BSP_LED_turnOFF(void)
{
	HAL_GPIO_WritePin( LED_POPT, LED_PIN, GPIO_PIN_SET );
}

/**
 * @brief �I���{�[�hLED�̃g�O���o��
 *
 * GPIO(PC13)��LED��_���܂��͏����Ƀg�O������B
 * @param �Ȃ�
 * @return �Ȃ�
 */
void BSP_LED_toggle(void)
{
	HAL_GPIO_TogglePin( LED_POPT, LED_PIN );
}
#endif // FALSE

/**
 * @} end of addtogroup
 */
