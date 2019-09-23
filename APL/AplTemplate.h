/**
 * @file  AplTemplate.h
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
#ifndef _AplTemplate_H_
#define _AplTemplate_H_

/**
 * @page AplTemp_TITLE アプリテンプレート
 *
 * @section AplTemp_API API一覧
 * | API名          | 機能          |
 * |:-------------- |:------------- |
 * | AplTemp_init()  | 初期設定   |
 *
 * @section AplTemp_USAGE 使用方法.
 * - 予め一度だけ AplTemp_init() を呼び出すこと.
 */
/**
 * @addtogroup AplTemp_GROUP アプリテンプレート機能.
 * @{
 */

/**
 * @brief APIリターンコード.
 */
typedef enum AplTemp_ERRCD {
	AplTemp_OK		= 0x0000,		/**< 正常終了 */
	AplTemp_PARAM	= 0x0001,		/**< パラメータエラー */
	AplTemp_STATE	= 0x0002,		/**< ステートエラー */
	AplTemp_MAX_ERRCD
}AplTemp_ERRCD;


/**
 * @brief APIメッセージデータ型.
 */
typedef struct AplTemp_MSG_t {
    /// 機能コード.
    uint16_t    feature;
}AplTemp_MSG_t;


/**
 * @brief 初期設定API.
 */
extern void AplTemp_init( void );


/**
 * @} end of addtogroup
 */
#endif //_AplTemplate_H_
