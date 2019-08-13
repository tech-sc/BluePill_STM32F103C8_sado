/**
 * @file  TIMER_ms.h
 * @brief �~���b�^�C�}�R���g���[���̐���API.
 * 
 * �~���b�^�C�}�R���g���[���̐���API��񋟂���.
 * 
 * @author      Teru
 * @date        2019/08/04
 * @version     Rev0.10
 * 
 * @par �ύX����:
 * - Rev0.01: 2019/06/27: �V�K�쐬
 * - Rev0.10: 2019/08/04: �@�\���i�^�C�}���~���b�^�C�}�j�ύX
 * 
 * @copyright   2019 Emb-se.com.
 */
#ifndef _TIMER_MS_H_
#define _TIMER_MS_H_

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

/// �~���b�^�C�}�R���g���[��
#define TIMER_MS_DEV					TIM4
/// �~���b�^�C�}�R���g���[��IRQn
#define TIMER_MS_IRQ					TIM4_IRQn

/**
 * @brief �^�C�}�R���g���[���̏����ݒ�.
 */
extern void TIMER_ms_init( void );

/**
 * @brief �R���y�A�}�b�`�l�Z�b�g.
 */
#define TIMER_ms_setCompReg( val )		LL_TIM_OC_SetCompareCH1( TIMER_MS_DEV, val )
#define TIMER_ms_getTick()				LL_TIM_GetCounter( TIMER_MS_DEV )


/**
 * @brief TIM4 �����݃n���h��.
 */
extern void TIM4_IRQHandler(void);

/**
 * @} end of addtogroup
 */
#endif //_TIMER_MS_H_
