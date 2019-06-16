/************************************************************************************//**
 * @file    main.c
 * @brief   STM32F103C8 BluePill
 *
 * @mainpage STM32F103-BluePill������v���W�F�N�g
 * @brief   �g���݃V�X�e���Z�p��p�v���W�F�N�g�ł���.<br>
 *
 * @details STM32CubeF1���C�u�������g�p���Ď�������.<br>
 *
 * @par �ύX����:
 *	Rev0.01�@2019/06/16�@�e���v���[�g�쐬<br>
 * @par Copyright
 *	2014-19 Emb-se.com All rights reserved.
 */
#include <stdio.h>
#include "stm32f1xx.h"
#include "os.h"


/************************************************************************************//**
 * @brief   �v���O�������C��
 * @param   None
 * @retval  None
 */
void main( void )
{
	osKernel_start();
	for(;;);
}
