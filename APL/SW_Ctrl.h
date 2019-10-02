/**
 * @file  SwCtrllate.h
 * @brief アプリテンプレート.
 * 
 * RTOSリソースを使用するアプリケーションテンプレートである.
 * 
 * @author      Teru
 * @date        2019/09/22
 * @version     Rev0.10
 * @copyright   2019 Emb-se.com.
 * 
 * @par 変更履歴:
 * - Rev0.01: 2019/09/22: 新規作成.
 * 
 * @copyright   2019 Emb-se.com.
 */
#ifndef _SwCtrl_H_
#define _SwCtrl_H_

/**
 * @page SwCtrl_TITLE アプリテンプレート
 *
 * @section SwCtrl_API API一覧
 * | API名          | 機能          |
 * |:-------------- |:------------- |
 * | SwCtrl_init()  | 初期設定   |
 *
 * @section SwCtrl_USAGE 使用方法.
 * - 予め一度だけ SwCtrl_init() を呼び出すこと.
 */
/**
 * @addtogroup SwCtrl_GROUP アプリテンプレート機能.
 * @{
 */

/**
 * @brief APIリターンコード.
 */
typedef enum SwCtrl_ERRCD {
	SwCtrl_OK		= 0x0000,		/**< 正常終了 */
	SwCtrl_PARAM	= 0x0001,		/**< パラメータエラー */
	SwCtrl_STATE	= 0x0002,		/**< ステートエラー */
	SwCtrl_MAX_ERRCD
}SwCtrl_ERRCD;


/**
 * @brief 初期設定API.
 */
extern void SwCtrl_init( void );


/**
 * @} end of addtogroup
 */
#endif //_SwCtrl_H_
