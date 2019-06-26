/************************************************************************************//**
 * @file ExtSW.h
 * @brief �O����H���SW����API
 *
 * @author  Teru
 * @date    2019/06/19
 * @version Rev0.10
 * @par �ύX����:
 * - Rev0.10: 2019/06/19: �V�K�쐬.
 *
 * @copyright 2018 Emb-se.com. All rights reserved.
 */
#ifndef _ExtSW_H_
#define _ExtSW_H_

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"

/**
 * @page PAGE_ExtSW �O����H���ExtSW����.
 *
 * @section SEC1_ExtSW �O����H���ExtSW����API.
 * |APIs     |�@�\����|
 * |:--------|:------ |
 * |ExtSW_init() | �O����H���ExtSW�̏����ݒ� |
 */
/**
 * @addtogroup GROUP_ExtSW �O����H���ExtSW����.
 * @brief �O����H���ExtSW����API��񋟂���B<br>
 * @details ExtSW�̓v���A�b�v�ڑ������B<br>
 * @{
 */

#define ExtSW1_POPT			GPIOA
#define ExtSW1_PIN			GPIO_PIN_8
#define EXTI9_5_PRIORITY	5

/**
 * @brief �O����H���SW�̏����ݒ�.
 *
 * ExtSW1�|�[�g�̓v���A�b�v�ŏ����ݒ肷��B
 * @param �Ȃ�
 * @return �Ȃ�
 */
extern void ExtSW_init(void);

/**
 * @brief SW1�����ݏ���.
 *
 * ������(active)�܂��͗��オ��(inactive)�G�b�W�����݂���������.
 * @param �Ȃ�.
 * @return �Ȃ�.
 */
extern void ExtSW1_interrupt(void);


#endif // _ExtSW_H_
/**
 * @} end of addtogroup
 */
