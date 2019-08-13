/**
 * @file  ASSERT.h
 * @brief アサート機能.
 * 
 * アサートAPLに提供する.
 * 
 * @author      Teru
 * @date        2019/08/04
 * @version     Rev0.10
 * @copyright   2019 Emb-se.com.
 * 
 * @par 変更履歴:
 * - Rev0.01: 2019/08/04: 新規作成.
 * 
 * @copyright   2019 Emb-se.com.
 */
#ifndef _ASSERT_H_
#define _ASSERT_H_

/**
 * @page ASSERT_title アサートライブラリ
 *
 * @section ASSERT_api API一覧
 * | API名          | 機能          |
 * |:-------------- |:------------- |
 * | assert_nv()    | リターンコードなしASSERT |
 * | assert_val()   | リターンコードありASSERT |
 *
 * @section ASSET_usage 使用方法
 * - 予め一度だけ TIMms_initTimer() を呼び出す。
 */
/**
 * @addtogroup GROUP_ASSERT アサート機能.
 * @{
 */

__weak void LOG( char const *func, int line )
{
}

/**
 * @brief 初期設定API
 */
extern void ASSERT_init( void );

/**
 * @brief リターンコードなしASSERT
 */
#define ASSERT_nv( exp )			if(!(exp)) { LOG(__FUNCTION__, __LINE__); return; }

/**
 * @brief リターンコードありASSERT
 */
#define ASSERT_val( exp, retv )		if(!(exp)) { LOG(__FUNCTION__, __LINE__); return retv; }

/**
 * @} end of addtogroup
 */
#endif //_TIMms_H_
