/**
 * @file  TIMER.c
 * @brief �^�C�}�R���g���[���̐���API.
 * 
 * �^�C�}�R���g���[���̐���API��񋟂���.
 * 
 * @author      Teru
 * @date        2019.06.27
 * @version     Rev0.01
 * 
 * @par �ύX����:
 * - Rev0.01: 2019.06.27: �V�K�쐬
 * 
 * @copyright   2019 Emb-se.com.
 */
#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32_assert.h"
#include "stm32f1xx_ll_tim.h"

/**
 * @page PAGE_TIMER �^�C�}�R���g���[������.
 *
 * @section SEC1_TIMER �^�C�}�R���g���[������API.
 * | APIs              | �@�\����    |
 * |:------------------ |:---------- |
 * | TIMER_init()       | �^�C�}�R���g���[���̏����ݒ� |
 * | TIMER_setCompReg() | �R���y�A�}�b�`�l�Z�b�g |
 *
 * @section TIMER_usage �g�p���@.
 * - �\�߈�x���� TIMER_init() ���Ăяo��.
 */
/**
 * @addtogroup GROUP_TIMER �^�C�}�R���g���[������.
 * @brief �^�C�}�R���g���[������API��񋟂���.<br>
 * @{
 */

/**
 * @brief �^�C�}�R���g���[���̏����ݒ�.
 */
extern void TIMER_init( void );

/**
 * @brief �R���y�A�}�b�`�l�Z�b�g.
 */
#define TIMER_setCompReg( val )		LL_TIM_OC_SetCompareCH1(TIM4, val)
#define TIMER_getTick()				LL_TIM_GetCounter(TIM4)


/**
 * @brief TIM4 �����݃n���h��.
 */
extern void TIM4_IRQHandler(void);

/**
 * @} end of addtogroup
 */
#endif
