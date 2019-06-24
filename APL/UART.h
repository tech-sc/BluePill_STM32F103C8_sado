/**************************************************************************//**
 * @file	UART.h
 * @brief RS232C���x���ϊ�����UART����API
 *
 * @author  Teru
 * @date    2014/01/12
 * @version Rev1.02
 * @par �ύX����:
 * - Rev1.01: 2014/01/12: ���������[�X��.
 * - Rev1.10: 2019/06/19: STM32 HAL�h���C�o�g�p.
 *
 * @copyright 2014 Emb-se.com.
 */
#ifndef _UART_H_
#define _UART_H_

#include "FreeRTOS.h"
#include "semphr.h"

/**************************************************************************//**
 * @brief	�萔��`
 */
#define CR_CODE				'\r'		/* ���s�R�[�h */
#define LF_CODE				'\n'		/* ���s�R�[�h */


/**************************************************************************//**
 * @brief	�ϐ���`
 */
/** �V���A����M�Z�}�t�H */
extern xSemaphoreHandle	UART_RxSemaph;

/** �ʐM���[�h */
#define ASCII_MODE			0
#define BINARY_MODE			1


/**************************************************************************//**
 * @brief	�֐���`
 */
extern int  UART_init( void );
extern int  UART_setMode( uint16_t mode );
extern int  UART_getchar( void );
extern int  UART_putchar( char c );
extern int  UART_puts( char *ptr );
extern int  UART_putstr( char *ptr );

#endif /*_UART_H_*/
