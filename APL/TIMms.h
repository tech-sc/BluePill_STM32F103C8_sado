/**
 * @file  TIMms.h
 * @brief ミリ秒オーダータイマ機能.
 * 
 * ミリ秒単位で要求可能なソフトウェアタイマをAPLに提供する.
 * 
 * @author      Teru
 * @date        2019.06.27
 * @version     Rev0.01
 * @copyright   2019 Emb-se.com.
 * 
 * @par 変更履歴:
 * - Rev0.01: 2019.06.27: 新規作成
 * 
 * @copyright   2019 Emb-se.com.
 */
/**
 * @page TIMER_title タイマライブラリ
 *
 * @section TIMER_api API一覧
 * | API名          | 機能          |
 * |:-------------- |:------------- |
 * | TIMms_initTimer()  | 初期設定   |
 * | TIMms_reqTimer()   | タイマ要求 |
 * | TIMms_cancelTimer()| タイマ取消 |
 *
 * @section TIMER_usage 使用方法
 * - 予め一度だけ TIMms_initTimer() を呼び出す。
 */

typedef uint16_t    TIMCNT_t;	/**< タイマカウンタレジスタ型(16bitカウンタ) */

typedef struct TIMms_t {
    int64_t     remain_tick;		/**< 発火までの残りTICK */
    void   (*expire_cb)(void *hdl);	/**< 発火コールバック関数 */
    struct TIMms_t *next_list;		/**< 要求または発火リスト */
    TIMCNT_t    old_tick;			/**< 要求処理または発火処理時のTICK値 */
    uint16_t    feature;			/**< オプション機能コード */
}TIMms_t;

/** オプション機能コード */
#define TIMms_FEATURE	0x5000		/**< ミリ秒オーダータイマ機能コード */
#define TIMms_STARTING	0x0100		/**< タイマ要求処理中 */
#define TIMms_STOPING	0x0200		/**< タイマ停止処理中 */
#define TIMms_ONESHORT	0x0010		/**< OneShot */
#define TIMms_REPEAT	0x0020		/**< リピート */

							/* 1TICK=0.25msec */
#define MSEC_TO_TICK(ms)		(int64_t)((int64_t)ms * 4) +1
#define TICK_TO_MSEC(tick)		(int32_t)(tick / 4)

#define TIMms_getTick			TIMER_getTick

/**
 * @brief 初期設定API
 */
extern void TIMms_initTimer( void );

/**
 * @brief 次回発火TICKセット
 */
extern void TIMms_setNextExpire( TIMCNT_t tick );

/**
 * @brief タイマ要求API
 *
 * ミリ秒単位のタイマ要求を提供するAPI関数である.
 * 発火はコールバックにて通知する.
 *
 * @param[in]	time  タイマ時間。ミリ秒で指定する.
 * @param[in] expire_cb  発火時のコールバック関数を指定する.
 * @param[in]	p_req  タイマ要求データ領域を渡す.
 * @retval  handle			タイマ要求が成功すると、そのタイマハンドルを返す.
 * @retval  NULL	パラメータエラー.
 * @pre     timeは1ms以上であること.
 *			        expire_cbはNULLでないこと.
 *          p_reqはNULLでないこと.
 * @post    タイマ停止は返したハンドルで停止可能である. また発火時に該当ハンドルを通知する.
 */
extern void *TIMms_reqTimer( int32_t time, void (*expire_cb)(void *handle), TIMms_t *p_req );

/**
 * @brief タイマ取消API
 *
 * タイマ要求を取消すAPI関数である.
 * APIの使い方によっては取消と発火がすれ違う可能性があるので呼び出し側で対応すること.
 *
 * @param[in]	handle タイマ要求を取消すタイマハンドル.
 * @retval  handle	取消が成功すると、取消したタイマハンドルを返す.
 * @retval  NULL	パラメータエラー.
 *                                        またはエントリされてない（既に発火した）
 * @pre   handleはNULLでないこと.
 */
extern void *TIMms_cancelTimer( TIMms_t *handle );

/**
 * @brief タイマ割込みハンドラからコールされる発火処理
* @param[in]	over =0/=1:カウンタOver flow.
 */
extern void TIMms_expire( int over );

/**
 * @brief タイマ発火処理タスク
 */
extern void TIMms_task( void *arg );

