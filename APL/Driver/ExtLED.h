/************************************************************************************//**
 * @file ExtLED.h
 * @brief �O����H���LED����API
 *
 * @author  Teru
 * @date    2019/06/19
 * @version Rev0.10
 * @par �ύX����:
 * - Rev0.10: 2019/06/19: �V�K�쐬.
 *
 * @copyright 2018 Emb-se.com. All rights reserved.
 */
#ifndef _ExtLED_H_
#define _ExtLED_H_

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

/**
 * @page PAGE_ExtLED �O����H���LED����.
 *
 * @section SEC1_ExtLED �O����H���LED����API.
 * |APIs     |�@�\����|
 * |:--------|:------ |
 * |ExtLED_init() | �O����H���LED�̏����ݒ� |
 * |ExtLED1_turnON() | �O����H���LED1�̓_�� |
 * |ExtLED1_turnOFF() | �O����H���LED1�̏��� |
 * |ExtLED1_toggle() | �O����H���LED1�̃g�O���o�� |
 */
/**
 * @addtogroup GROUP_ExtLED �O����H���LED����.
 * @brief �O����H���LED����API��񋟂���B<br>
 * @details LED�̓v���A�b�v�ڑ������B<br>
 * @{
 */

#define ExtLED1_POPT		GPIOB
#define ExtLED1_PIN			GPIO_PIN_0
#define ExtLED2_POPT		GPIOB
#define ExtLED2_PIN			GPIO_PIN_1

/**
 * @brief �O����H���LED�̏����ݒ�.
 *
 * LED1�|�[�g�̓v���A�b�v�ŏ����ݒ��Ainactive(1)�ŏ���������B
 * @param �Ȃ�
 * @return �Ȃ�
 */
extern void ExtLED_init(void);

/**
 * @brief �O����H���LEDX�̓_��.
 *
 * LEDX�|�[�g��LED��_������B
 * @param �Ȃ�
 * @return �Ȃ�
 */
#define ExtLED1_turnON()	HAL_GPIO_WritePin( ExtLED1_POPT, ExtLED1_PIN, GPIO_PIN_RESET )
#define ExtLED2_turnON()	HAL_GPIO_WritePin( ExtLED2_POPT, ExtLED2_PIN, GPIO_PIN_RESET )

/**
 * @brief �O����H���LEDX�̏���.
 *
 * LEDX�|�[�g��LED����������B
 * @param �Ȃ�
 * @return �Ȃ�
 */
#define ExtLED1_turnOFF()	HAL_GPIO_WritePin( ExtLED1_POPT, ExtLED1_PIN, GPIO_PIN_SET )
#define ExtLED2_turnOFF()	HAL_GPIO_WritePin( ExtLED2_POPT, ExtLED2_PIN, GPIO_PIN_SET )

/**
 * @brief �O����H���LEDX�̃g�O���o��
 *
 * LEDX�|�[�g��LED��_���܂��͏����Ƀg�O������B
 * @param �Ȃ�
 * @return �Ȃ�
 */
#define ExtLED1_toggle()	HAL_GPIO_TogglePin( ExtLED1_POPT, ExtLED1_PIN )
#define ExtLED2_toggle()	HAL_GPIO_TogglePin( ExtLED2_POPT, ExtLED2_PIN )


#endif // _ExtLED_H_
/**
 * @} end of addtogroup
 */
