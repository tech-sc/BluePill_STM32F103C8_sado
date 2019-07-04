/**************************************************************************//**
 * @file	shell.c
 * @brief	CLI�R�}���h�V�F��
 * @version V1.0.0
 * @date	2014/01/12
 * @author	Teru
 */
#include <stdio.h>
#include <string.h>
#include "stm32_assert.h"
#include "FreeRTOS.h"
#include "task.h"
#include "UART.h"
#include "shell.h"

/**************************************************************************//**
 * @brief	�萔��`
 */
#undef  CRLF_MODE
#undef  CR_MODE
#define LF_MODE

#define SHELL_PROMPT		"sh #"

#if defined(CRLF_MODE)
  #define SHELL_CR				"\r\n"
  #define SHELL_NOTFOUND_ERR	"\r\ncommand not found.\r\n"
  #define SHELL_COMMAND_ERR		"\r\ncommand error.\r\n"
#elif defined(CR_MODE)
  #define SHELL_CR				"\n"
  #define SHELL_NOTFOUND_ERR	"\ncommand not found.\n"
  #define SHELL_COMMAND_ERR		"\ncommand error.\n"
#else //LF_MODE
  #define SHELL_CR				"\r"
  #define SHELL_NOTFOUND_ERR	"\rcommand not found.\r"
  #define SHELL_COMMAND_ERR		"\rcommand error.\r"
#endif //LF_MODE

/**************************************************************************//**
 * @brief	�֐���`
 */
/** �V�F�����o�͐�f�o�C�XI/O API��` */
/* ���o�͐�f�o�C�X�FUART */
#define MonIO_getchar		UART_getchar
#define MonIO_puts			UART_puts
#define MonIO_putstr		UART_putstr

/** �����֐� */
#define shell_promt()		MonIO_putstr( SHELL_PROMPT )
#define shell_cr_promt()	MonIO_putstr( SHELL_CR SHELL_PROMPT )

void shell_task( void *arg );
int  shell_help( int argc, char **argv );
int  shell_regist_command( SHELL_COMMAND_t *p_item );

static int  shell_core( void );
static int  shell_esc( void );
static SHELL_COMMAND_t *shell_find_command( SHELL_COMMAND_t *p_list, char *str, int len );
static void shell_execute( int argc, char **argv );
static int  shell_arglist( char **argv, char *str );


/**************************************************************************//**
 * @brief	�ϐ���`
 */
/** �V�F���R�A�o�b�t�@ */
#define SHELL_CORE_BUFF_SIZE		384
char		shell_CoreBuff[ SHELL_CORE_BUFF_SIZE ];
char		*shell_CoreBuffCursol_p;		/* �J�[�\���ʒu�|�C���^ */
int16_t		shell_CoreBuff_len;				/* �R�}���h������ */
char		shell_tempBuff[ 20 ];
int16_t		shell_tempBuff_len;

/** CLI�R�}���h�������X�g */
#define SHELL_ARGV_SIZE				16
char	*shell_argv[ SHELL_ARGV_SIZE ];

/** CLI�R�}���h�e�[�u�� */
SHELL_COMMAND_t	shell_CommandTbl = {
	"help", shell_help, "\tshow entry command lists." SHELL_CR "\tshell version 1.00", NULL
};


/**************************************************************************//**
 * @brief  CLI�R�}���h�V�F�������ݒ�
 */
void shell_init( void )
{
	portBASE_TYPE	retv;
	xTaskHandle		handle;

	UART_init();

	shell_CoreBuff_len     = 0;
	shell_CoreBuffCursol_p = shell_CoreBuff;

	retv = xTaskCreate( &shell_task, "Shell", shellTASK_STACKSZ/4,	// stack size=64*4=256byte
						NULL, 3 | portPRIVILEGE_BIT, &handle );
	configASSERT(retv == pdPASS);
}

/**************************************************************************//**
 * @brief  CLI�R�}���h�V�F���^�X�N
 * @param[in] arg  �^�X�N�N���p�����[�^
 */
void shell_task( void *arg )
{
	int			argc;

	shell_cr_promt();

	for( ;; ) {
		if( UART_getMode() == ASCII_MODE )
		{
			/** �e�L�X�g���[�h */
			shell_core();
			if( shell_CoreBuff_len == 0 ) {
				shell_promt();
			} else {
				argc = shell_arglist( shell_argv, (char*)shell_CoreBuff );
				/** CLI�R�}���h���s */
				shell_execute( argc, shell_argv );
				shell_cr_promt();
			}
		} else {
			/* �o�C�i�����[�h */
		}
	}
}

/**************************************************************************//**
 * @brief  CLI�R�}���h�V�F���R�A
 * @return	None
 */
static int shell_core( void )
{
	SHELL_COMMAND_t		*p_list = NULL;
	short		loop = 1;
	uint8_t		ch;

	shell_tempBuff_len = 0;
	shell_CoreBuffCursol_p  = shell_CoreBuff;
	*shell_CoreBuffCursol_p = '\0';
	shell_CoreBuff_len      = 0;

	while( loop )
	{
		MonIO_putstr( ESC_SAVE );

		ch = MonIO_getchar();
		switch( ch )
		{
		  case KEY_ESC:
			if( ! shell_esc() ) {
				MonIO_putstr( ESC_LOAD );
			}
			break;
#if 0
		  case KEY_BS:
			if( shell_CoreBuff_len > 0 ) {
				shell_CoreBuff_len--;
				shell_CoreBuffCursol_p--;
				shell_CoreBuff[ shell_CoreBuff_len ] = '\0';
			} else {
				MonIO_putstr( ESC_LOAD );
			}
			break;
		  case KEY_DEL:
			if( shell_CoreBuff_len > 0 ) {
				shell_CoreBuff_len--;
				memcpy( shell_CoreBuffCursol_p, shell_CoreBuffCursol_p+1, shell_CoreBuff_len );
			} else {
				MonIO_putstr( ESC_LOAD );
			}
			break;
#endif
		  case KEY_TAB:
			if( shell_tempBuff_len == 0 ) {
				strcpy( shell_tempBuff, shell_CoreBuff );
				shell_tempBuff_len = shell_CoreBuff_len;
			}
			p_list = shell_find_command( p_list, shell_tempBuff, shell_tempBuff_len );
			if( p_list != NULL ) {
				// �J�[�\�������� shell_tempBuff_len �ړ�����
				;
				strcpy( shell_CoreBuff, p_list->pCommand );
				shell_CoreBuff_len = strlen( shell_CoreBuff );
				MonIO_putstr( shell_CoreBuff );
			} else {
				MonIO_putstr( ESC_LOAD );
			}
			break;

#if defined(CR_MODE)||defined(CRLF_MODE)
		  case KEY_CR:
			shell_CoreBuff[ shell_CoreBuff_len ] = '\0';
#if defined(CR_MODE)
			loop = 0;
			break;
#endif //CR_MODE
#endif //CR_MODE||CRLF_MODE

#if defined(LF_MODE)||defined(CRLF_MODE)
		  case KEY_LF:
			shell_CoreBuff[ shell_CoreBuff_len ] = '\0';
			loop = 0;
			break;
#endif //LF_MODE||CRLF_MODE

		  default:
			if( shell_CoreBuff_len < SHELL_CORE_BUFF_SIZE -1 ) {
				if( shell_CoreBuffCursol_p - shell_CoreBuff == shell_CoreBuff_len ) {
					shell_CoreBuff_len++;
				}
				*shell_CoreBuffCursol_p = ch;
				shell_CoreBuffCursol_p++;
			}
			break;
		}
	}
	return 0;
}

/**************************************************************************//**
 * @brief  ESC�V�[�P���X����
 * @retval 1  �L�[�����ς�
 * @retval 0  �s���L�[(������)
 */
static int shell_esc( void )
{
	char		ch;
	char		virtual_code = 0;
	char		numeric = 0;
	int			retv = 0;

	while( virtual_code == 0 )
	{
		ch = MonIO_getchar();
		switch( ch )
		{
		  case ESC_2ND:
			break;

		  case ALLOW_LEFT:
			if( shell_CoreBuffCursol_p > shell_CoreBuff ) {
				shell_CoreBuffCursol_p --;
				retv = 1;
			}
			virtual_code = KEY_LEFT;
			break;
		  case ALLOW_RIGHT:
			if( shell_CoreBuffCursol_p - shell_CoreBuff < shell_CoreBuff_len ) {
				shell_CoreBuffCursol_p ++;
				retv = 1;
			}
			virtual_code = KEY_RIGHT;
			break;

		  default:
			if( (ch >= '0')&&(ch <= '9') ) {
				numeric *= 10;
				numeric += ch - '0';
			} else if( ch == '~' ) {
				switch( numeric )
				{
				  case ESCPLUS_INS:
					virtual_code = KEY_INS;
					break;
				  case ESCPLUS_HOME:
					virtual_code = KEY_HOME;
					break;
				  case ESCPLUS_END:
					virtual_code = KEY_END;
					break;
				  case ESCPLUS_PGUP:
					virtual_code = KEY_PGUP;
					break;
				  case ESCPLUS_PGDN:
					virtual_code = KEY_PGDN;
					break;
				  default:
					virtual_code = -1;
					break;
				}
			} else {
				virtual_code = -1;
			}
			break;
		}
	}
	return retv;
}

/**************************************************************************//**
 * @brief  CLI�R�}���h��������
 * @param[in] p_item  �����J�n����R�}���h���
 * @param[in] str  ��������R�}���h������
 * @param[in] len  ��������R�}���h�̕�����
 * @retval NULL  ������Ȃ�
 * @retval !NULL  ������v�����R�}���h�̃R�}���h���
 */
static SHELL_COMMAND_t *shell_find_command( SHELL_COMMAND_t *p_list, char *str, int len )
{
	if( p_list == NULL ) {
		p_list = &shell_CommandTbl;
	}
	while( p_list != NULL ) {
		if( strncmp( p_list->pCommand, str, len ) == 0 ) {
			break;
		}
		p_list = p_list->pNext;
	}
	return p_list;
}

/**************************************************************************//**
 * @brief	�o�^���ꂽCLI�R�}���h�̎��s
 * @param	argc	[in]CLI�R�}���h�����̐�
 * @param	argv	[in]CLI�R�}���h�������X�g
 * @return	None
 */
static void shell_execute( int argc, char **argv )
{
	SHELL_COMMAND_t		*p_list;
	int					retv = 0;

	p_list = &shell_CommandTbl;
	while( p_list != NULL ) {
		if( strcmp( p_list->pCommand, argv[0] ) == 0 ) {
			/* �R�}���h���s */
			retv = (p_list->pFunc)( argc, argv );
			break;
		}
		p_list = p_list->pNext;
	}

	if( p_list == NULL ) {
		MonIO_puts( SHELL_NOTFOUND_ERR );
	} else if( retv != 0 ) {
		MonIO_puts( SHELL_COMMAND_ERR );
	}
	MonIO_puts( SHELL_CR );
}

/**************************************************************************//**
 * @brief	CLI�R�}���h�������X�g����
 * @param	argv	[out]CLI�R�}���h�������X�g
 * @param	str		[in,out]CLI�R�}���h�s������^�󔒕�����NULL�����ɒu��������
 * @return	���������R�}���h�����̐�
 */
static int shell_arglist( char **argv, char *str )
{
	int			argc = 0;
	char		*top;

	top = str;
	while( *str != '\0' ) {
		if( *str == ' ' ) {
			*str++  = '\0';
			if( (*top != '\0') && (argc < SHELL_ARGV_SIZE -1) ) {
				argc++;
				*argv++ = top;
			}
			top = str;
		} else {
			str++;
		}
	}
	if( (*top != '\0') && (argc < SHELL_ARGV_SIZE -1) ) {
		argc++;
		*argv++ = top;
	}
	*argv = NULL;
	return argc;
}

/**************************************************************************//**
 * @brief	�w���v�R�}���h�i�R�}���h�ꗗ�\���j
 * @param	argc	[in]�R�}���h�����̐�
 * @param	argv	[in]�R�}���h�������X�g
 * @return	0		OK
 */
int shell_help( int argc, char **argv )
{
	SHELL_COMMAND_t		*p_list;

	p_list = &shell_CommandTbl;
	while( p_list != NULL ) {
		MonIO_putstr( p_list->pCommand );
		MonIO_puts( p_list->pHelp );

		p_list = p_list->pNext;
	}
	return 0;
}


/**************************************************************************//**
 * @brief	CLI�R�}���h�o�^
 * @param	p_item	[in]�o�^����R�}���h��� ���X�^�b�N�̈�NG
 * @return	0		�o�^OK
 * @return	-1		�p�����[�^�G���[
 */
int shell_regist_command( SHELL_COMMAND_t *p_item )
{
	SHELL_COMMAND_t		*p_list;

	if( p_item == NULL ) {
		return -1;
	}
	if( (p_item->pCommand == NULL)||(p_item->pFunc == NULL)||
		(p_item->pHelp == NULL) ) {
		return -1;
	}

	p_list = shell_CommandTbl.pNext;
	while( p_list != NULL ) {
		if( strcmp( p_list->pCommand, p_item->pCommand ) > 0 ) {
			break;
		}
		p_list = p_list->pNext;
	}
	p_list->pNext = p_item;
	return 0;
}
