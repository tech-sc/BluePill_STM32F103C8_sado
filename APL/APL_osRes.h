#ifndef _APL_osRes_h_
#define _APL_osRes_h_

enum {
	IDLE_TASKID = 0,
	TMRSVC_TASKID,
	SHELL_TASKID,
	TIMms_TASKID,
	LOG_TASKID,
	APL_INITTASKID,
	MAX_TASKNUM,
};
enum {
	TMRSVC_QUEID = 0,
	LOG_QUEID,
	MAX_QUENUM,
};
enum {
	UART_RXMUTEXID = 0,
	TIMms_MUTEXID,
	APL_INITMUTEXID,
	MAX_MUTEXNUM,
};

// OSリソースの管理に使用されるメモリサイズ
#define	osRESOURCEMNG_SZ	( 80+8+8 )
#define	osTASKMNG_SZ		( MAX_TASKNUM  * osRESOURCEMNG_SZ )
#define	osQUEMNG_SZ			( MAX_QUENUM   * osRESOURCEMNG_SZ )
#define	osMUTEXMNG_SZ		( MAX_MUTEXNUM * osRESOURCEMNG_SZ )
#define	TOTAL_MNGSZ			( osTASKMNG_SZ			\
							+ osQUEMNG_SZ			\
							+ osMUTEXMNG_SZ )

// タスクのスタック・メモリサイズ
#define	osIDLE_STACKSZ		256
#define	osTMRSVC_STACKSZ	256
#define	shellTASK_STACKSZ	256
#define	TIMmsTASK_STACKSZ	256
#define	LOGTASK_STACKSZ		256
#define	APLinitTASK_STACKSZ	256
#define TOTAL_STACKSZ		( osIDLE_STACKSZ + osTMRSVC_STACKSZ \
							+ shellTASK_STACKSZ \
							+ TIMmsTASK_STACKSZ \
							+ LOGTASK_STACKSZ \
							+ APLinitTASK_STACKSZ )

// キューのコンテンツ・メモリサイズ(コンテンツサイズ*個数)
#define	osTMRSVC_QUESZ		( 12*10+8 )
#define	LOG_QUESZ			( 44*10+8 )
#define TOTAL_QUESZ			( osTMRSVC_QUESZ \
							+ LOG_QUESZ )

// OSヒープサイズ
#define osHEAP_SZ			( TOTAL_MNGSZ			\
							+ TOTAL_STACKSZ 		\
							+ TOTAL_QUESZ)

#define TIMmsTASK_PRI		(configMAX_PRIORITIES -1)
#define shellTASK_PRI		(configMAX_PRIORITIES -2)
#define LOGTASK_PRI			(configMAX_PRIORITIES -4)
#define APLinitTASK_PRI		(configMAX_PRIORITIES -4)


#endif // _APL_osRes.h_
