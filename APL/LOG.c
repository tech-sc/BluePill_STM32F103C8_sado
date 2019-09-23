/**
 * @file  LOG.c
 * @brief ログ機能API.
 * 
 * ログ機能APIを提供する.
 * 
 * @author      Teru
 * @date        2019/07/27
 * @version     Rev0.10
 * 
 * @par 変更履歴:
 * - Rev0.01: 2019/07/27: 新規作成
 * 
 * @copyright   2019 Emb-se.com.
 */
/**
 * @addtogroup GROUP_LOG
 * @{
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "os.h"
#include "LOG.h"
#include "TIMms.h"

#define SHELL
#ifdef SHELL
#include "shell.h"
#include "UART.h"

/// コマンド提供データ
static SHELL_COMMAND_t regCmd_setLogOut;

/// ログ出力状態文字列.
static char		*LOG_STATUS[2] = {
	"off", "on"
};

/// ログ出力状態.
static uint8_t	LOG_out = false;

/// ログ出力文字列バッファ
static char		LOG_string[80];
#endif

/**
 * @brief TLV-VALUEデータ型.
 * 文字列コード/10進数/16進数 出力データをセットする.
 */
typedef union LOG_VAL_t {
	uint32_t	dword;
	/// 文字列(auto変数不可)
	char* const	ptr;
}LOG_VAL_t;

/**
 * @brief TLVフォーマット型.
 */
typedef struct LOG_TLV_t {
	/// TAG
	LOG_TAG_t	tag[4];
	/// LENGTHは、固定長のため実装しない.
	/// VALUE
	LOG_VAL_t	val[4];
}LOG_TLV_t;

/**
 * @brief ログデータ型.
 */
typedef struct LOG_DATA_t {
	/// tick
	uint32_t	tick;
	/// ログタイプ.
	uint16_t	type;
	/// 機能コード.(呼び出し(APL)側で定義する事)
	uint16_t	fn_id;
	/// 行番号.
	uint16_t	line;
	/// ログデータ(TLV)
	LOG_TLV_t	tlv;
}LOG_DATA_t;


/**
 * @brief ログ出力文字列.
 * @ref LOG_TYPE_t
 */
static char		*LOG_STR[MAX_LOG_TYPE] = {
	"DBG:",
	"PRM:",
	"INF:",
	"WRN:",
	"ERR:",
};

/**
 * @brief ログ出力レベル.
 */
static uint8_t		LOG_level = LOG_DEBUG;


#define MAX_LOG_POOL		10

/**
 * @brief ログ格納バッファ.
 */
static LOG_DATA_t	LOG_buff[MAX_LOG_POOL];
static int			WtPos = 0;
static int			RdPos = 0;

/**
 * @brief ログキューハンドル.
 */
osQueHandle_t	hLOG_MsgQue;


#ifdef SHELL
/**
 * @brief ログ出力コマンド.
 * shellからコールされるCLIコマンド.
 * @param[in]	argc  
 * @param[in]	argv  
 * @note	コマンドライン Usage
 *			# log [on|off] [--help]
 */
static int LOG_output( int argc, char **argv )
{
	if( argc == 1 ) {
		UART_puts( LOG_STATUS[LOG_out] );
	} else if( argc == 2 ) {
		if( strncmp( argv[1], "on", 3 ) == 0 ) {
			LOG_out = true;
			UART_puts( "set log on" );
			while( LOG_read( LOG_string, sizeof(LOG_string) ) == 0 ) {
				UART_puts( LOG_string );
			}
		}else if( strncmp( argv[1], "off", 4 ) == 0 ) {
			LOG_out = false;
			UART_puts( "set log off" );
		}
	} else {
		UART_puts( "Usage: log [on|off]" );
		return -1;
	}
	return 0;
}
#endif

/**
 * @brief ログ出力タスク.
 * @param[in]	arg  未使用.
 */
static void LOG_task( void *arg )
{
	LOG_DATA_t	msg = {0};

	while(1) {
		if( osQue_receive( hLOG_MsgQue, &msg, osMAX_TIME ) == 0 ){
			continue;
		}
		// ログ保存
		if( WtPos >= MAX_LOG_POOL ) {
			WtPos = 0;
		}
		memcpy( &LOG_buff[WtPos], &msg, sizeof(LOG_DATA_t) );
		WtPos++;

#ifdef SHELL
		if( LOG_out ) {
			while( LOG_read( LOG_string, sizeof(LOG_string) ) == 0 ) {
				UART_puts( LOG_string );
			}
		}
#endif
	}
}

/**
 * @brief ログ初期設定API
 *
 * ログ初期設定及びログ出力タスクの生成.
 * @param[in]	level ログ出力レベル.指定したログタイプ以上をロギングする.
 * @ref LOG_write 関数.
 */
void LOG_init( LOG_TYPE_t level )
{
	osTaskHandle_t	handle;
	int		retv;

	/* OSリソース生成 */
printf("%s\n", __FUNCTION__);
	hLOG_MsgQue = xQueueCreate( 10, sizeof(LOG_DATA_t) );
	configASSERT( hLOG_MsgQue != 0 );

	/* ログ出力用タスクを生成する. */
	retv = osTask_create( &LOG_task, "LOG", LOG_STACKSZ/4,
						NULL, LOGTASK_PRI | portPRIVILEGE_BIT, &handle );
	configASSERT( retv == pdPASS );

	/* ログ出力レベルをセットする(デフォルト:LOG_DEBUGレベル). */
	if( level < MAX_LOG_TYPE ) {
		LOG_level = level;
	}
	RdPos = 0;
	WtPos = 0;

#ifdef SHELL
	/** shell_init()を予めコール済みであること */
	regCmd_setLogOut.pCommand = "log";
	regCmd_setLogOut.pHelp    = "\tlogging data output.";
	regCmd_setLogOut.pFunc    = LOG_output;
	shell_registerCommand( &regCmd_setLogOut );
#endif
}


/**
 * @brief ログ書き込みAPI
 *
 * ログタイプを指定してロギングするデータを渡す.
 * | Log Type    | 用途           | 出力レベル |
 * |:----------- |:-------------- |:---------- |
 * | LOG_DEBUG   | デバッグ       | 低い       |
 * | LOG_PARAM   | パラメータ情報 |  ↑        |
 * | LOG_INFO    | その他情報     |  ｜        |
 * | LOG_WARNING | ワーニング     |  ↓        |
 * | LOG_ERROR   | エラー         | 高い       |
 *   ※ログ出力レベルはLOG_init()で指定して抑止可能.
 *
 * 可変長引数にTLVフォーマットのTag、Valueを１ペアで指定してログ出力データを渡す.
 * | TLV Tag   | 機能         | データ長      |
 * |:--------- |:------------ |:------------- |
 * | TAG_DEC   | 10進数出力   | データ長32bit |
 * | TAG_HEX   | 16進数出力   | データ長32bit |
 * | TAG_STR   | 文字列出力   | データ長32bitポインタ |
 * | TAG_TERM  | 終端TAG      | これ以降にデータなし |
 *
 * @param[in]	type    ログタイプ.
 * @param[in]	line    行番号.
 * @param[in]	fn_id   機能コード.(呼び出し(APL)側で定義する事)
 * @param[in]	argc    arg_list数.(0/2/4/6/8)
 * @param[in]	...     arg_list tag1,val1, tag2,val2, tag3,val3, tag4,val4
 * @retval	0	Success
 * @retval	-1	パラメータエラー.
 * @pre 	e.g.1) LOG_write( LOG_INFO,  __LINE__, 2, TAG_DEC, value1 );<br>
 *          e.g.2) LOG_write( LOG_ERROR, __LINE__, 4, TAG_STR, STRID_PARAM, TAG_HEX, value2 );<br>
 */
int LOG_write( LOG_TYPE_t type, int line, uint16_t fn_id, int argc, ... )
{
	va_list		ap;
	LOG_DATA_t	msg = {0};
	int			i, idx, retv = 0;
	bool		loop = true;

	if( type < LOG_level ) {
		return 0;		// ログ抑止
	}
	if( type >= MAX_LOG_TYPE ) {
		return -1;
	}

	msg.tick  = TIMms_getTick();
	msg.type  = type;
	msg.line  = (uint16_t)line;
	msg.fn_id = fn_id;

	if( (argc & 0x01)||(argc > 8)  ) {
		return -1;
	}
	va_start( ap, argc );

	idx = 0;
	for( i = 0; (i < argc /2)&&(loop); i++ ) {
		msg.tlv.tag[idx]       = (LOG_TAG_t)va_arg( ap, int );
		msg.tlv.val[idx].dword = va_arg( ap, uint32_t );
		switch( msg.tlv.tag[idx] ) {
		case TAG_STR:
		case TAG_DEC:
		case TAG_HEX:
			idx++;
			break;
		case TAG_TERM:
			msg.tlv.val[idx].dword = 0;
			loop = false;
			break;
		default:
			loop = false;
			retv = -1;
			break;
		}
	}
	va_end( ap );

	if( retv == 0 ){
		retv = osQue_send( hLOG_MsgQue, &msg, 0 );
	}
	return retv;
}

/**
 * @brief ログ読み出しAPI
 *
 * ログ格納バッファからログを文字列で返す.
 *
 * @param[out]	str     ログを格納する文字列ポインタ.
 * @param[in]	sz      最大文字列バッファサイズ.(byte)
 * @retval	0	Success
 * @retval	-1	ログデータなし.
 */
int LOG_read( char *str, size_t sz )
{
	LOG_DATA_t	*p_rd;
	int			len, save_len, i;
	bool		loop = true;
	int			retv = -1;

	if( RdPos != WtPos ) {
		if( RdPos >= MAX_LOG_POOL ) {
			RdPos = 0;
		}
		p_rd = &LOG_buff[RdPos];
		len = snprintf( str, sz, "%lu %s 0x%x %d ",
					 p_rd->tick, LOG_STR[p_rd->type], p_rd->fn_id, p_rd->line );
		if( (len <= 0)||(len >= sz) ) {
			return -1;
		}
		save_len = len;
		for( i=0; (i < 4)&&(loop); i++ ) {
			switch( p_rd->tlv.tag[i] ) {
			case TAG_STR:
				len = snprintf( &str[save_len], sz -save_len, " %s", p_rd->tlv.val[i].ptr );
				break;
			case TAG_DEC:
				len = snprintf( &str[save_len], sz -save_len, " %ld", p_rd->tlv.val[i].dword );
				break;
			case TAG_HEX:
				len = snprintf( &str[save_len], sz -save_len, " 0x%lX", p_rd->tlv.val[i].dword );
				break;
			case TAG_TERM:
				loop = false;
				retv = 0;
				break;
			}
			if( (len <= 0)||(len >= sz) ) {
				loop = false;
				break;
			}
		}
		RdPos++;
	}
	return retv;
}

/**
 * @} end of addtogroup
 */
