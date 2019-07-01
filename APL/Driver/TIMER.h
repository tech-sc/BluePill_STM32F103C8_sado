/**
 * @file  TIMER.c
 * @brief �^�C�}�R���g���[���ւ̐���API.
 * 
 * �^�C�}�R���g���[���ւ̐���API��񋟂���.
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
/**
 * @mainpage
 * �e���v���[�g�E�g�b�v�y�[�W
 * 
 * @ref TIMER_title
 */
/**
 * @page TIMER_title �^�C�}���C�u����
 *
 * @section TIMER_api API�ꗗ
 * | API��              | �@�\       |
 * |:------------------ |:---------- |
 * | TIMER_init()       | �^�C�}�����ݒ� |
 * | TIMER_setCompReg() | �R���y�A�}�b�`�l�Z�b�g |
 *
 * @section TIMER_usage �g�p���@
 * - �\�߈�x���� TIMER_init() ���Ăяo���B
 */
#ifndef _TIMER_H_
#define _TIMER_H_

//#include "stm32f1xx_hal.h"
//#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_ll_tim.h"

typedef uint16_t    TIMCNT_t;	/**< �^�C�}�J�E���^���W�X�^�^(16bit�J�E���^) */

/**
 * @brief �^�C�}�����ݒ�
 */
extern void TIMER_init( void );

/**
 * @brief �R���y�A�}�b�`�l�Z�b�g
 */
#define TIMER_setCompReg( val )		LL_TIM_OC_SetCompareCH1(TIM4, val)


/**
 * @brief TIM4 �����݃n���h��
 */
extern void TIM4_IRQHandler(void);

#endif
