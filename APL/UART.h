/**************************************************************************//**
 * @file	UART.h
 * @brief RS232Cレベル変換向けUART制御API
 *
 * @author  Teru
 * @date    2014/01/12
 * @version Rev1.02
 * @par 変更履歴:
 * - Rev1.01: 2014/01/12: 初期リリース版.
 * - Rev1.10: 2019/06/19: STM32 HALドライバ使用.
 *
 * @copyright 2014 Emb-se.com.
 */
#ifndef _UART_H_
#define _UART_H_

#include "FreeRTOS.h"
#include "semphr.h"

/**************************************************************************//**
 * @brief	定数定義
 */
#define CR_CODE				'\r'		/* 改行コード */
#define LF_CODE				'\n'		/* 改行コード */


/**************************************************************************//**
 * @brief	変数定義
 */
/** シリアル受信セマフォ */
extern xSemaphoreHandle	UART_RxSemaph;

/** 通信モード */
#define ASCII_MODE			0
#define BINARY_MODE			1


/**************************************************************************//**
 * @brief	関数定義
 */
extern int  UART_init( void );
extern int  UART_setMode( uint16_t mode );
extern int  UART_getchar( void );
extern int  UART_putchar( char c );
extern int  UART_puts( char *ptr );
extern int  UART_putstr( char *ptr );

#endif /*_UART_H_*/
