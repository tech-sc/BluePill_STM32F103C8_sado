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

/**
 * @brief ���O�f�[�^�^.
 */
typedef struct LOG_DATA_t {
	/// ���O�^�C�v.
	uint16_t	type;
	/// �@�\�R�[�h.
	uint16_t	fn_id;
	/// �s�ԍ�.
	uint16_t	line;
	/// T1-TAG
	uint8_t		tag[4];
	/// T1-VALUE
	LOG_VAL_t	val[4];
}LOG_DATA_t;

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
	LOG_DATA_t	msg;

	while(1) {
		if( osQue_receive( hLOG_MsgQue, &msg, osMAX_TIME ) == 0 ){
			continue;
		}
		// ���O�o��
		;
	}
}

/**
 * @brief �����ݒ�API
 *
 * ���O�����ݒ�y�у��O�o�̓^�X�N�̐���
 */
void LOG_init( void )
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
}

/**
 * @brief ���O��������.
 *
 * �ϒ�������T1�t�H�[�}�b�g��Tag�AValue���P�y�A�Ŏw�肵�ă��O�o�̓f�[�^��n��.
 * | T1-TAG     | �@�\                |�f�[�^��       |
 * |:---------- |:------------------- |:------------- |
 * | TAG_STR    | ������o�̓f�[�^TAG | �f�[�^��32bit�|�C���^ |
 * | TAG_DEC    | 10�i���o�̓f�[�^TAG | �f�[�^��32bit |
 * | TAG_HEX    | 16�i���o�̓f�[�^TAG | �f�[�^��32bit |
 * @param[in]	type    ���O�^�C�v.
 * @param[in]	line    �s�ԍ�.
 * @param[in]	fn_id   �@�\�R�[�h.
 * @param[in]	argc    arg_list��(0/2/4/6/8)
 * @param[in]	...     arg_list tag1,val1, tag2,val2, tag3,val3, tag4,val4
 * @retval	0	Success
 * @retval	-1	�p�����[�^�G���[
 * @pre 	e.g.1) LOG_write( LOG_INFO,  __LINE__, 2, TAG_DEC, value1 );<br>
 *          e.g.2) LOG_write( LOG_ERROR, __LINE__, 4, TAG_STR, STRID_PARAM, TAG_HEX, value2 );<br>
 */
int LOG_write( LOG_TYPE_t type, int line, LOG_FID_t fn_id, int argc, ... )
{
	LOG_TAG_t	tag;
	LOG_VAL_t	val;
	va_list		ap;
	LOG_DATA_t	msg;
	int		i, idx, retv = 0;

	if( (argc & 0x01)||(argc > 8)  ) {
		return -1;
	}
	va_start( ap, argc );

	idx = 0;
	for( i = 0; i < argc /2; i++ ) {
		tag = (LOG_TAG_t)va_arg( ap, int );
		val = va_arg( ap, LOG_VAL_t );
		switch( tag ) {
		case TAG_STR:
		case TAG_DEC:
		case TAG_HEX:
			msg.tag[idx] = tag;
			msg.val[idx] = val;
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
