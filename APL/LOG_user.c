/**
 * @file  LOG_user.c
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
#include "LOG_user.h"

/**
 * @brief ログ出力文字列.
 */
char	*LOG_STR[STRID_MAX] = {
	"DEBUG: ",
	"INFO : ",
	"WARN : ",
	"ERROR: "
	"param",
};

/**
 * @} end of addtogroup
 */
