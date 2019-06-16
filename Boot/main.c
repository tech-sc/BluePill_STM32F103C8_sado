/************************************************************************************//**
 * @file    main.c
 * @brief   STM32F103C8 BluePill
 *
 * @mainpage STM32F103-BluePill基板向けプロジェクト
 * @brief   組込みシステム技術会合用プロジェクトである.<br>
 *
 * @details STM32CubeF1ライブラリを使用して実装する.<br>
 *
 * @par 変更履歴:
 *	Rev0.01　2019/06/16　テンプレート作成<br>
 * @par Copyright
 *	2014-19 Emb-se.com All rights reserved.
 */
#include <stdio.h>
#include "stm32f1xx.h"
#include "os.h"


/************************************************************************************//**
 * @brief   プログラムメイン
 * @param   None
 * @retval  None
 */
void main( void )
{
	osKernel_start();
	for(;;);
}
