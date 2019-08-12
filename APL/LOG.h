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

/**
 * @brief ログタイプ型.
 */
typedef enum LOG_TYPE_t {
	/// デバッグタイプ.
	LOG_DEBUG = 0,
	/// パラメータ情報タイプ.
	LOG_PARAM,
	/// 情報タイプ.
	LOG_INFO,
	/// ワーニングタイプ.
	LOG_WARNING,
	/// エラータイプ.
	LOG_ERROR,
	/// 終端(指定不可)
	MAX_LOG_TYPE
}LOG_TYPE_t;


/**
 * @brief ログ初期設定API
 */
void LOG_init( LOG_TYPE_t level );

/**
 * @brief ログ書き込みAPI
 */
int LOG_write( LOG_TYPE_t type, int line, uint16_t fn_id, int argc, ... );

/**
 * @} end of addtogroup
 */
