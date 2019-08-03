/**
 * @file  LOG_user.h
 * @brief ログ文字列とユーザ機能コードの定義.
 * 
 * ログ出力する文字列とユーザ機能コードをAPLが定義する.
 * 
 * @author      application users
 * @date        2019/07/27
 * @version     Rev0.10
 * 
 * @par 変更履歴:
 * - Rev0.01: 2019/07/27: 新規作成
 */
/**
 * @addtogroup GROUP_LOG_user
 * @{
 */
/**
 * @brief ユーザ機能コード.
 */
typedef enum LOG_FNID_t {
	FID_TIMER	= 0x0101,
	FID_MAX
}LOG_FID_t;

/**
 * @brief ログ出力文字列コード.
 */
typedef enum LOG_STRID_t {
	STRID_DEBUG		= 0x0000,
	STRID_INFO		= 0x0001,
	STRID_WARNING	= 0x0002,
	STRID_ERROR		= 0x0003,
	STRID_PARAM		= 0x0004,	/**< param */
	STRID_MAX
}LOG_STRID_t;

/**
 * @brief ログ出力文字列.
 * @ref LOG_STR of LOG_user.c
 */
extern char	*LOG_STR[STRID_MAX];

/**
 * @} end of addtogroup
 */
