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
 * @page PAGE_ExtSW �O����H���SW����.
 *
 * @section SEC1_ExtSW �O����H���SW����API.
 * |APIs     |�@�\����|
 * |:--------|:------ |
 * |ExtSW_init() | �O��SW�̏����ݒ� |
 */
/**
 * @addtogroup GROUP_ExtSW �O����H���SW����.
 * @brief �O����H���SW����API��񋟂���.<br>
 * @details ExtSW�̓v���A�b�v�ڑ������.<br>
 * @{
 */

#define ExtSW1_POPT			GPIOA
#define ExtSW1_PIN			GPIO_PIN_8
#define EXTI9_5_PRIORITY	5

/**
 * @brief �O���X�C�b�`�̏����ݒ�.
 */
extern void ExtSW_init(void);

/**
 * @brief SW1�����ݏ���.
 */
extern void ExtSW1_interrupt(void);

/**
 * @} end of addtogroup
 */
#endif // _ExtSW_H_
