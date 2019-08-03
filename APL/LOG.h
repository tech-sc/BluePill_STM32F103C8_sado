/**
 * @file  LOG.h
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
#include <stdarg.h>
#include "LOG_user.h"

/**
 * @brief ログ T1-TAGデータ型.
 */
typedef enum LOG_TAG_t {
	TAG_NONE	= 0,		/**< TAGなし */
	TAG_STR		= 0x02,		/**< 文字列出力データTAG、データ長32bitポインタ. */
	TAG_DEC		= 0x04,		/**< 10進数出力データTAG、データ長32bit */
	TAG_HEX		= 0x08,		/**< 16進数出力データTAG、データ長32bit */
}LOG_TAG_t;

/**
 * @brief ログ T1-VALUEデータ型.
 * 文字列コード/10進数/16進数 出力データをセットする.
 */
typedef uint32_t	LOG_VAL_t;

/**
 * @brief ログデータタイプ型.
 */
typedef enum LOG_TYPE_t {
	LOG_DEBUG	= 1,		/**< デバッグタイプ. */
	LOG_INFO	= 2,		/**< 情報タイプ. */
	LOG_WARNING	= 3,		/**< ワーニングタイプ. */
	LOG_ERROR	= 4,		/**< エラータイプ. */
}LOG_TYPE_t;


/**
 * @brief ログ初期設定API
 */
void LOG_init( void );

/**
 * @brief ログ書き込み.
 */
int LOG_write( LOG_TYPE_t type, int line, LOG_FID_t fn_id, int argc, ... );

/**
 * @} end of addtogroup
 */
