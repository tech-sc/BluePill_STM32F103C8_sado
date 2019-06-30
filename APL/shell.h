/**************************************************************************//**
 * @file	shell.c
 * @brief	CLI�R�}���h�V�F��
 * @version V1.0.0
 * @date	2014/01/12
 * @author	Teru
 */
#ifndef _SHELL_H_
#define _SHELL_H_

/**************************************************************************//**
 * @brief	�萔��`
 */
// shell.h �ֈړ�
//---------------------------
/** �L�[���͂ɂ��オ���Ă���ESC�V�[�P���X */
#define ESC_2ND				'['			/*<! ESC�L�[�Q������ */
#define ALLOW_UP			'A'			/* ���L�[:ESC+[+A */
#define ALLOW_DOWN			'B'			/* ���L�[:ESC+[+B */
#define ALLOW_RIGHT			'C'			/* ���L�[:ESC+[+C */
#define ALLOW_LEFT			'D'			/* ���L�[:ESC+[+D */
#define ESCPLUS_HOME		1			/* HOME�L�[:ESC+[+1+~ */
#define ESCPLUS_INS			2			/* INS�L�[:ESC+[+2+~ */
#define ESCPLUS_END			4			/* HOME�L�[:ESC+[+4+~ */
#define ESCPLUS_PGUP		5			/* PageUp�L�[:ESC+[+5+~ */
#define ESCPLUS_PGDN		6			/* PageDn�L�[:ESC+[+6+~ */
#define ESCPLUS_F1			11			/* F1�L�[:ESC+[+11+~ */
#define ESCPLUS_F2			12			/* F2�L�[:ESC+[+12+~ */
#define ESCPLUS_F3			13			/* F3�L�[:ESC+[+13+~ */
#define ESCPLUS_F4			14			/* F4�L�[:ESC+[+14+~ */
#define ESCPLUS_F5			15			/* F5�L�[:ESC+[+15+~ */
#define ESCPLUS_F6			17			/* F6�L�[:ESC+[+17+~ */
#define ESCPLUS_F7			18			/* F7�L�[:ESC+[+18+~ */
#define ESCPLUS_F8			19			/* F8�L�[:ESC+[+19+~ */
#define ESCPLUS_F9			20			/* F9�L�[:ESC+[+20+~ */
#define ESCPLUS_F10			21			/* F10�L�[:ESC+[+21+~ */
#define ESCPLUS_F11			23			/* F10�L�[:ESC+[+23+~ */
#define ESCPLUS_F12			24			/* F10�L�[:ESC+[+24+~ */

/** �L�[�R�[�h */
#define KEY_INS				0x07			/* �G�~�����[�g�R�[�h */
#define KEY_BS				0x08
#define KEY_TAB				0x09
#define KEY_LF				0x0a
#define KEY_CR				0x0d
#define KEY_ESC				0x1b
#define KEY_HOME			0x11			/* �G�~�����[�g�R�[�h */
#define KEY_END				0x12			/* �G�~�����[�g�R�[�h */
#define KEY_PGUP			0x13			/* �G�~�����[�g�R�[�h */
#define KEY_PGDN			0x14			/* �G�~�����[�g�R�[�h */
#define KEY_UP				0x1c			/* �G�~�����[�g�R�[�h */
#define KEY_DOWN			0x1d			/* �G�~�����[�g�R�[�h */
#define KEY_RIGHT			0x1e			/* �G�~�����[�g�R�[�h */
#define KEY_LEFT			0x1f			/* �G�~�����[�g�R�[�h */
#define KEY_DEL				0x7f

/** �o�͂���ESC�V�[�P���X */
#define ESC_SAVE			"\x1b[s"		/* �J�[�\���ʒu�ƕ\���������Z�[�u���� */
#define ESC_LOAD			"\x1b[u"		/* �J�[�\���ʒu�ƕ\�����������[�h���� */


/**************************************************************************//**
 * @brief	�ϐ���`
 */
typedef int (*SHELL_COMMAND_cb)( int argc, char **argv );

typedef struct SHELL_COMMAND_t	{
	char					*pCommand;	/* �R�}���h���� */
	SHELL_COMMAND_cb		pFunc;		/* �R�}���h�֐� */
	char					*pHelp;		/* �w���v�� */
	struct SHELL_COMMAND_t	*pNext;		/* �����f�[�^�F�R�}���h���X�g */
}SHELL_COMMAND_t;


/**************************************************************************//**
 * @brief	API�֐���`
 */
extern void shell_init( void );
extern int  shell_command_registory( );

#endif /*_SHELL_H_*/
