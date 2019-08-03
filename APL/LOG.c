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

/**
 * @brief ログデータ型.
 */
typedef struct LOG_DATA_t {
	/// ログタイプ.
	uint16_t	type;
	/// 機能コード.
	uint16_t	fn_id;
	/// 行番号.
	uint16_t	line;
	/// T1-TAG
	uint8_t		tag[4];
	/// T1-VALUE
	LOG_VAL_t	val[4];
}LOG_DATA_t;

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
	LOG_DATA_t	msg;

	while(1) {
		if( osQue_receive( hLOG_MsgQue, &msg, osMAX_TIME ) == 0 ){
			continue;
		}
		// ログ出力
		;
	}
}

/**
 * @brief 初期設定API
 *
 * ログ初期設定及びログ出力タスクの生成
 */
void LOG_init( void )
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
}

/**
 * @brief ログ書き込み.
 *
 * 可変長引数にT1フォーマットのTag、Valueを１ペアで指定してログ出力データを渡す.
 * | T1-TAG     | 機能                |データ長       |
 * |:---------- |:------------------- |:------------- |
 * | TAG_STR    | 文字列出力データTAG | データ長32bitポインタ |
 * | TAG_DEC    | 10進数出力データTAG | データ長32bit |
 * | TAG_HEX    | 16進数出力データTAG | データ長32bit |
 * @param[in]	type    ログタイプ.
 * @param[in]	line    行番号.
 * @param[in]	fn_id   機能コード.
 * @param[in]	argc    arg_list数(0/2/4/6/8)
 * @param[in]	...     arg_list tag1,val1, tag2,val2, tag3,val3, tag4,val4
 * @retval	0	Success
 * @retval	-1	パラメータエラー
 * @pre 	e.g.1) LOG_write( LOG_INFO,  __LINE__, 2, TAG_DEC, value1 );<br>
 *          e.g.2) LOG_write( LOG_ERROR, __LINE__, 4, TAG_STR, STRID_PARAM, TAG_HEX, value2 );<br>
 */
int LOG_write( LOG_TYPE_t type, int line, LOG_FID_t fn_id, int argc, ... )
{
	LOG_TAG_t	tag;
	LOG_VAL_t	val;
	va_list		ap;
	LOG_DATA_t	msg;
	int		i, idx, retv = 0;

	if( (argc & 0x01)||(argc > 8)  ) {
		return -1;
	}
	va_start( ap, argc );

	idx = 0;
	for( i = 0; i < argc /2; i++ ) {
		tag = (LOG_TAG_t)va_arg( ap, int );
		val = va_arg( ap, LOG_VAL_t );
		switch( tag ) {
		case TAG_STR:
		case TAG_DEC:
		case TAG_HEX:
			msg.tag[idx] = tag;
			msg.val[idx] = val;
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
