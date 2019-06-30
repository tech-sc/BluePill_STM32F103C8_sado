#ifndef _APL_osRes_h_
#define _APL_osRes_h_

enum {
	IDLE_TASKID = 0,
	TMRSVC_TASKID,
	SHELL_IASKID,
	MAX_TASKNUM,
};
enum {
	TMRSVC_QUEID = 0,
	MAX_QUENUM,
};
enum {
	UART_RXMUTEXID = 0,
	MAX_MUTEXNUM,
};

// OS���\�[�X�̊Ǘ��Ɏg�p����郁�����T�C�Y
#define	osRESOURCEMNG_SZ	( 80+8+8 )
#define	osTASKMNG_SZ		( MAX_TASKNUM  * osRESOURCEMNG_SZ )
#define	osQUEMNG_SZ			( MAX_QUENUM   * osRESOURCEMNG_SZ )
#define	osMUTEXMNG_SZ		( MAX_MUTEXNUM * osRESOURCEMNG_SZ )
#define	TOTAL_MNGSZ			( osTASKMNG_SZ			\
							+ osQUEMNG_SZ			\
							+ osMUTEXMNG_SZ )

// �^�X�N�̃X�^�b�N�E�������T�C�Y
#define	osIDLE_STACKSZ		256
#define	osTMRSVC_STACKSZ	256
#define	SHELLTASK_STACKSZ	256
#define TOTAL_STACKSZ		( osIDLE_STACKSZ 		\
							+ osTMRSVC_STACKSZ )

// �L���[�̃R���e���c�E�������T�C�Y
#define	osTMRSVC_QUESZ		( 12*10+8 )
#define TOTAL_QUESZ			( osTMRSVC_QUESZ )

// OS�q�[�v�T�C�Y
#define osHEAP_SZ			( TOTAL_MNGSZ			\
							+ TOTAL_STACKSZ 		\
							+ TOTAL_QUESZ)

#endif // _APL_osRes.h_
