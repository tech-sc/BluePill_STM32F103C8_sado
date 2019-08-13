/**
 * @file  TIMER_us.h
 * @brief �^�C�}�R���g���[��(�}�C�N���b)�̐���API.
 * 
 * �ʕb�^�C�}�R���g���[���̐���API��񋟂���.
 * 
 * @author      Teru
 * @date         2019/08/04
 * @version     Rev0.01
 * 
 * @par �ύX����:
 * - Rev0.01: 2019/08/04: �V�K�쐬
 * 
 * @copyright   2019 Emb-se.com.
 */
#ifndef _TIMER_US_H_
#define _TIMER_US_H_

#include "stm32_assert.h"
#include "stm32f1xx_ll_tim.h"

/**
 * @page PAGE_TIMER_US �^�C�}�R���g���[������.
 *
 * @section SEC1_TIMER_US �^�C�}�R���g���[������API.
 * | APIs              | �@�\����    |
 * |:------------------ |:---------- |
 * | TIMER_init()       | �^�C�}�R���g���[���̏����ݒ� |
 * | TIMER_setCompReg() | �R���y�A�}�b�`�l�Z�b�g |
 *
 * @section TIMER_US_usage �g�p���@.
 * - �\�߈�x���� TIMER_init() ���Ăяo��.
 */
/**
 * @addtogroup GROUP_TIMER_US �^�C�}�R���g���[������.
 * @brief �^�C�}�R���g���[������API��񋟂���.<br>
 * @{
 */

/// �ʕb�^�C�}�R���g���[��
#define TIMER_US_DEV					TIM3
/// �ʕb�^�C�}�R���g���[��IRQn
#define TIMER_US_IRQ					TIM3_IRQn
/**
 * @brief �^�C�}�R���g���[���̏����ݒ�.
 */
extern void TIMER_US_init( void );

/// �ʕb�R���y�A�}�b�`�l�Z�b�g.
#define TIMER_us_setCompReg( val )		LL_TIM_OC_SetCompareCH1( TIMER_US_DEV, val )
/// �ʕb�^�C�}�J�E���^�擾
#define TIMER_us_getTick()				LL_TIM_GetCounter( TIMER_US_DEV )


/**
 * @brief TIM3 �����݃n���h��.
 */
extern void TIM3_IRQHandler(void);

/**
 * @} end of addtogroup
 */
#endif //_TIMER_US_H_
