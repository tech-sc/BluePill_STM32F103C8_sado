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
#include "os.h"
#include "LOG.h"
#include "TIMER_ms.h"

/**
 * @brief TLV-TAGデータ型.
 */
typedef enum LOG_TAG_t {
	/// 終端(これ以降にデータなし)
	TAG_TERM = 0,
	/// 10進数出力TAG：データ長32bit
	TAG_DEC,
	/// 16進数出力TAG：データ長32bit
	TAG_HEX,
	/// 文字出力TAG(MAX4文字.4文字未満は\0でターミネートする事)
	TAG_CHAR,
	/// 文字列出力TAG：データ長32bitポインタ.
	TAG_STR,
}LOG_TAG_t;

/**
 * @brief TLV-VALUEデータ型.
 * 文字列コード/10進数/16進数 出力データをセットする.
 */
typedef union LOG_VAL_t {
	uint8_t		byte[4];
	uint32_t	dword;
	/// 文字(MAX４文字.\0でターミネートする)
	char		chr[4];
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
static char	*LOG_STR[MAX_LOG_TYPE] = {
	"DBG: ",
	"PRM: ",
	"INF: ",
	"WRN: ",
	"ERR: ",
};

/**
 * @brief ログ出力レベル.
 */
uint8_t		LOG_level = LOG_DEBUG;

/**
 * @brief ログキューハンドル.
 */
osQueHandle_t	hLOG_MsgQue;

/**
 * @brief ログ出力タスク.
 * @param[in]	arg  未使用.
 */
static void LOG_task( void *arg )
{
	LOG_DATA_t	msg = {0};;

	while(1) {
		if( osQue_receive( hLOG_MsgQue, &msg, osMAX_TIME ) == 0 ){
			continue;
		}
		// ログ出力
		;
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
	hLOG_MsgQue = xQueueCreate( 10, sizeof(LOG_DATA_t) );
	configASSERT( hLOG_MsgQue != 0 );

	/* ログ出力用タスクを生成する. */
	retv = osTask_create( &LOG_task, "LOG", LOGTASK_STACKSZ/4,
						NULL, LOGTASK_PRI | portPRIVILEGE_BIT, &handle );
	configASSERT( retv == pdPASS );

	/* ログ出力レベルをセットする(デフォルト:LOG_DEBUGレベル). */
	if( level < MAX_LOG_TYPE ) {
		LOG_level = level;
	}
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
 * | TAG_CHAR  | MAX4文字出力 | データ長4byte |
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
	LOG_TAG_t	tag;
	uint32_t	val;
	va_list		ap;
	LOG_DATA_t	msg = {0};
	int		i, idx, retv = 0;
//	TIMCNT_t	tick;

//	tick = TIMER_ms_getTick();
	if( (argc & 0x01)||(argc > 8)  ) {
		return -1;
	}
	va_start( ap, argc );

	idx = 0;
	for( i = 0; i < argc /2; i++ ) {
		tag = (LOG_TAG_t)va_arg( ap, int );
		val = va_arg( ap, uint32_t );
		switch( tag ) {
		case TAG_STR:
		case TAG_DEC:
		case TAG_HEX:
			msg.tlv.tag[idx] = tag;
			msg.tlv.val[idx].dword = val;
			idx++;
			break;
		default:
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
 * @} end of addtogroup
 */
