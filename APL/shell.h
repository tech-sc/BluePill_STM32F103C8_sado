/**************************************************************************//**
 * @file	shell.c
 * @brief	CLIコマンドシェル
 * @version V1.0.0
 * @date	2014/01/12
 * @author	Teru
 */
#ifndef _SHELL_H_
#define _SHELL_H_

/**************************************************************************//**
 * @brief	定数定義
 */
// shell.h へ移動
//---------------------------
/** キー入力による上がってくるESCシーケンス */
#define ESC_2ND				'['			/*<! ESCキー２文字目 */
#define ALLOW_UP			'A'			/* ↑キー:ESC+[+A */
#define ALLOW_DOWN			'B'			/* ↓キー:ESC+[+B */
#define ALLOW_RIGHT			'C'			/* →キー:ESC+[+C */
#define ALLOW_LEFT			'D'			/* ←キー:ESC+[+D */
#define ESCPLUS_HOME		1			/* HOMEキー:ESC+[+1+~ */
#define ESCPLUS_INS			2			/* INSキー:ESC+[+2+~ */
#define ESCPLUS_END			4			/* HOMEキー:ESC+[+4+~ */
#define ESCPLUS_PGUP		5			/* PageUpキー:ESC+[+5+~ */
#define ESCPLUS_PGDN		6			/* PageDnキー:ESC+[+6+~ */
#define ESCPLUS_F1			11			/* F1キー:ESC+[+11+~ */
#define ESCPLUS_F2			12			/* F2キー:ESC+[+12+~ */
#define ESCPLUS_F3			13			/* F3キー:ESC+[+13+~ */
#define ESCPLUS_F4			14			/* F4キー:ESC+[+14+~ */
#define ESCPLUS_F5			15			/* F5キー:ESC+[+15+~ */
#define ESCPLUS_F6			17			/* F6キー:ESC+[+17+~ */
#define ESCPLUS_F7			18			/* F7キー:ESC+[+18+~ */
#define ESCPLUS_F8			19			/* F8キー:ESC+[+19+~ */
#define ESCPLUS_F9			20			/* F9キー:ESC+[+20+~ */
#define ESCPLUS_F10			21			/* F10キー:ESC+[+21+~ */
#define ESCPLUS_F11			23			/* F10キー:ESC+[+23+~ */
#define ESCPLUS_F12			24			/* F10キー:ESC+[+24+~ */

/** キーコード */
#define KEY_INS				0x07			/* エミュレートコード */
#define KEY_BS				0x08
#define KEY_TAB				0x09
#define KEY_LF				0x0a
#define KEY_CR				0x0d
#define KEY_ESC				0x1b
#define KEY_HOME			0x11			/* エミュレートコード */
#define KEY_END				0x12			/* エミュレートコード */
#define KEY_PGUP			0x13			/* エミュレートコード */
#define KEY_PGDN			0x14			/* エミュレートコード */
#define KEY_UP				0x1c			/* エミュレートコード */
#define KEY_DOWN			0x1d			/* エミュレートコード */
#define KEY_RIGHT			0x1e			/* エミュレートコード */
#define KEY_LEFT			0x1f			/* エミュレートコード */
#define KEY_DEL				0x7f

/** 出力するESCシーケンス */
#define ESC_SAVE			"\x1b[s"		/* カーソル位置と表示属性をセーブする */
#define ESC_LOAD			"\x1b[u"		/* カーソル位置と表示属性をロードする */


/**************************************************************************//**
 * @brief	変数定義
 */
typedef int (*SHELL_COMMAND_cb)( int argc, char **argv );

typedef struct SHELL_COMMAND_t	{
	char					*pCommand;	/* コマンド名称 */
	SHELL_COMMAND_cb		pFunc;		/* コマンド関数 */
	char					*pHelp;		/* ヘルプ文 */
	struct SHELL_COMMAND_t	*pNext;		/* 内部データ：コマンドリスト */
}SHELL_COMMAND_t;


/**************************************************************************//**
 * @brief	API関数定義
 */
extern void shell_init( void );
extern int  shell_command_registory( );

#endif /*_SHELL_H_*/
