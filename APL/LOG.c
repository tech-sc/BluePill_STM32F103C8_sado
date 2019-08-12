/**
 * @file  LOG.c
 * @brief ���O�@�\API.
 * 
 * ���O�@�\API��񋟂���.
 * 
 * @author      Teru
 * @date        2019/07/27
 * @version     Rev0.10
 * 
 * @par �ύX����:
 * - Rev0.01: 2019/07/27: �V�K�쐬
 * 
 * @copyright   2019 Emb-se.com.
 */
/**
 * @addtogroup GROUP_LOG
 * @{
 */
#include "os.h"
#include "LOG.h"
#include "TIMER_ms.h"

/**
 * @brief TLV-TAG�f�[�^�^.
 */
typedef enum LOG_TAG_t {
	/// �I�[(����ȍ~�Ƀf�[�^�Ȃ�)
	TAG_TERM = 0,
	/// 10�i���o��TAG�F�f�[�^��32bit
	TAG_DEC,
	/// 16�i���o��TAG�F�f�[�^��32bit
	TAG_HEX,
	/// �����o��TAG(MAX4����.4����������\0�Ń^�[�~�l�[�g���鎖)
	TAG_CHAR,
	/// ������o��TAG�F�f�[�^��32bit�|�C���^.
	TAG_STR,
}LOG_TAG_t;

/**
 * @brief TLV-VALUE�f�[�^�^.
 * ������R�[�h/10�i��/16�i�� �o�̓f�[�^���Z�b�g����.
 */
typedef union LOG_VAL_t {
	uint8_t		byte[4];
	uint32_t	dword;
	/// ����(MAX�S����.\0�Ń^�[�~�l�[�g����)
	char		chr[4];
	/// ������(auto�ϐ��s��)
	char* const	ptr;
}LOG_VAL_t;

/**
 * @brief TLV�t�H�[�}�b�g�^.
 */
typedef struct LOG_TLV_t {
	/// TAG
	LOG_TAG_t	tag[4];
	/// LENGTH�́A�Œ蒷�̂��ߎ������Ȃ�.
	/// VALUE
	LOG_VAL_t	val[4];
}LOG_TLV_t;

/**
 * @brief ���O�f�[�^�^.
 */
typedef struct LOG_DATA_t {
	/// ���O�^�C�v.
	uint16_t	type;
	/// �@�\�R�[�h.(�Ăяo��(APL)���Œ�`���鎖)
	uint16_t	fn_id;
	/// �s�ԍ�.
	uint16_t	line;
	/// ���O�f�[�^(TLV)
	LOG_TLV_t	tlv;
}LOG_DATA_t;


/**
 * @brief ���O�o�͕�����.
 * @ref LOG_TYPE_t
 */
static char	*LOG_STR[MAX_LOG_TYPE] = {
	"DBG: ",
	"PRM: ",
	"INF: ",
	"WRN: ",
	"ERR: ",
};

/**
 * @brief ���O�o�̓��x��.
 */
uint8_t		LOG_level = LOG_DEBUG;

/**
 * @brief ���O�L���[�n���h��.
 */
osQueHandle_t	hLOG_MsgQue;

/**
 * @brief ���O�o�̓^�X�N.
 * @param[in]	arg  ���g�p.
 */
static void LOG_task( void *arg )
{
	LOG_DATA_t	msg = {0};;

	while(1) {
		if( osQue_receive( hLOG_MsgQue, &msg, osMAX_TIME ) == 0 ){
			continue;
		}
		// ���O�o��
		;
	}
}

/**
 * @brief ���O�����ݒ�API
 *
 * ���O�����ݒ�y�у��O�o�̓^�X�N�̐���.
 * @param[in]	level ���O�o�̓��x��.�w�肵�����O�^�C�v�ȏ�����M���O����.
 * @ref LOG_write �֐�.
 */
void LOG_init( LOG_TYPE_t level )
{
	osTaskHandle_t	handle;
	int		retv;

	/* OS���\�[�X���� */
	hLOG_MsgQue = xQueueCreate( 10, sizeof(LOG_DATA_t) );
	configASSERT( hLOG_MsgQue != 0 );

	/* ���O�o�͗p�^�X�N�𐶐�����. */
	retv = osTask_create( &LOG_task, "LOG", LOGTASK_STACKSZ/4,
						NULL, LOGTASK_PRI | portPRIVILEGE_BIT, &handle );
	configASSERT( retv == pdPASS );

	/* ���O�o�̓��x�����Z�b�g����(�f�t�H���g:LOG_DEBUG���x��). */
	if( level < MAX_LOG_TYPE ) {
		LOG_level = level;
	}
}


/**
 * @brief ���O��������API
 *
 * ���O�^�C�v���w�肵�ă��M���O����f�[�^��n��.
 * | Log Type    | �p�r           | �o�̓��x�� |
 * |:----------- |:-------------- |:---------- |
 * | LOG_DEBUG   | �f�o�b�O       | �Ⴂ       |
 * | LOG_PARAM   | �p�����[�^��� |  ��        |
 * | LOG_INFO    | ���̑����     |  �b        |
 * | LOG_WARNING | ���[�j���O     |  ��        |
 * | LOG_ERROR   | �G���[         | ����       |
 *   �����O�o�̓��x����LOG_init()�Ŏw�肵�ė}�~�\.
 *
 * �ϒ�������TLV�t�H�[�}�b�g��Tag�AValue���P�y�A�Ŏw�肵�ă��O�o�̓f�[�^��n��.
 * | TLV Tag   | �@�\         | �f�[�^��      |
 * |:--------- |:------------ |:------------- |
 * | TAG_DEC   | 10�i���o��   | �f�[�^��32bit |
 * | TAG_HEX   | 16�i���o��   | �f�[�^��32bit |
 * | TAG_CHAR  | MAX4�����o�� | �f�[�^��4byte |
 * | TAG_STR   | ������o��   | �f�[�^��32bit�|�C���^ |
 * | TAG_TERM  | �I�[TAG      | ����ȍ~�Ƀf�[�^�Ȃ� |
 *
 * @param[in]	type    ���O�^�C�v.
 * @param[in]	line    �s�ԍ�.
 * @param[in]	fn_id   �@�\�R�[�h.(�Ăяo��(APL)���Œ�`���鎖)
 * @param[in]	argc    arg_list��.(0/2/4/6/8)
 * @param[in]	...     arg_list tag1,val1, tag2,val2, tag3,val3, tag4,val4
 * @retval	0	Success
 * @retval	-1	�p�����[�^�G���[.
 * @pre 	e.g.1) LOG_write( LOG_INFO,  __LINE__, 2, TAG_DEC, value1 );<br>
 *          e.g.2) LOG_write( LOG_ERROR, __LINE__, 4, TAG_STR, STRID_PARAM, TAG_HEX, value2 );<br>
 */
int LOG_write( LOG_TYPE_t type, int line, uint16_t fn_id, int argc, ... )
{
	LOG_TAG_t	tag;
	uint32_t	val;
	va_list		ap;
	LOG_DATA_t	msg = {0};
	int		i, idx, retv = 0;
//	TIMCNT_t	tick;

//	tick = TIMER_ms_getTick();
	if( (argc & 0x01)||(argc > 8)  ) {
		return -1;
	}
	va_start( ap, argc );

	idx = 0;
	for( i = 0; i < argc /2; i++ ) {
		tag = (LOG_TAG_t)va_arg( ap, int );
		val = va_arg( ap, uint32_t );
		switch( tag ) {
		case TAG_STR:
		case TAG_DEC:
		case TAG_HEX:
			msg.tlv.tag[idx] = tag;
			msg.tlv.val[idx].dword = val;
			idx++;
			break;
		default:
			retv = -1;
			break;
		}
	}
	va_end( ap );

	if( retv == 0 ){
		retv = osQue_send( hLOG_MsgQue, &msg, 0 );
	}
	return retv;
}

/**
 * @} end of addtogroup
 */
