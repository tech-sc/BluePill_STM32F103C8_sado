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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "os.h"
#include "LOG.h"
#include "TIMms.h"

#define SHELL
#ifdef SHELL
#include "shell.h"
#include "UART.h"

/// �R�}���h�񋟃f�[�^
static SHELL_COMMAND_t regCmd_setLogOut;

/// ���O�o�͏�ԕ�����.
static char		*LOG_STATUS[2] = {
	"off", "on"
};

/// ���O�o�͏��.
static uint8_t	LOG_out = false;

/// ���O�o�͕�����o�b�t�@
static char		LOG_string[80];
#endif

/**
 * @brief TLV-VALUE�f�[�^�^.
 * ������R�[�h/10�i��/16�i�� �o�̓f�[�^���Z�b�g����.
 */
typedef union LOG_VAL_t {
	uint32_t	dword;
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
	/// tick
	uint32_t	tick;
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
static char		*LOG_STR[MAX_LOG_TYPE] = {
	"DBG:",
	"PRM:",
	"INF:",
	"WRN:",
	"ERR:",
};

/**
 * @brief ���O�o�̓��x��.
 */
static uint8_t		LOG_level = LOG_DEBUG;


#define MAX_LOG_POOL		10

/**
 * @brief ���O�i�[�o�b�t�@.
 */
static LOG_DATA_t	LOG_buff[MAX_LOG_POOL];
static int			WtPos = 0;
static int			RdPos = 0;

/**
 * @brief ���O�L���[�n���h��.
 */
osQueHandle_t	hLOG_MsgQue;


#ifdef SHELL
/**
 * @brief ���O�o�̓R�}���h.
 * shell����R�[�������CLI�R�}���h.
 * @param[in]	argc  
 * @param[in]	argv  
 * @note	�R�}���h���C�� Usage
 *			# log [on|off] [--help]
 */
static int LOG_output( int argc, char **argv )
{
	if( argc == 1 ) {
		UART_puts( LOG_STATUS[LOG_out] );
	} else if( argc == 2 ) {
		if( strncmp( argv[1], "on", 3 ) == 0 ) {
			LOG_out = true;
			UART_puts( "set log on" );
			while( LOG_read( LOG_string, sizeof(LOG_string) ) == 0 ) {
				UART_puts( LOG_string );
			}
		}else if( strncmp( argv[1], "off", 4 ) == 0 ) {
			LOG_out = false;
			UART_puts( "set log off" );
		}
	} else {
		UART_puts( "Usage: log [on|off]" );
		return -1;
	}
	return 0;
}
#endif

/**
 * @brief ���O�o�̓^�X�N.
 * @param[in]	arg  ���g�p.
 */
static void LOG_task( void *arg )
{
	LOG_DATA_t	msg = {0};

	while(1) {
		if( osQue_receive( hLOG_MsgQue, &msg, osMAX_TIME ) == 0 ){
			continue;
		}
		// ���O�ۑ�
		if( WtPos >= MAX_LOG_POOL ) {
			WtPos = 0;
		}
		memcpy( &LOG_buff[WtPos], &msg, sizeof(LOG_DATA_t) );
		WtPos++;

#ifdef SHELL
		if( LOG_out ) {
			while( LOG_read( LOG_string, sizeof(LOG_string) ) == 0 ) {
				UART_puts( LOG_string );
			}
		}
#endif
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
printf("%s\n", __FUNCTION__);
	hLOG_MsgQue = xQueueCreate( 10, sizeof(LOG_DATA_t) );
	configASSERT( hLOG_MsgQue != 0 );

	/* ���O�o�͗p�^�X�N�𐶐�����. */
	retv = osTask_create( &LOG_task, "LOG", LOG_STACKSZ/4,
						NULL, LOGTASK_PRI | portPRIVILEGE_BIT, &handle );
	configASSERT( retv == pdPASS );

	/* ���O�o�̓��x�����Z�b�g����(�f�t�H���g:LOG_DEBUG���x��). */
	if( level < MAX_LOG_TYPE ) {
		LOG_level = level;
	}
	RdPos = 0;
	WtPos = 0;

#ifdef SHELL
	/** shell_init()��\�߃R�[���ς݂ł��邱�� */
	regCmd_setLogOut.pCommand = "log";
	regCmd_setLogOut.pHelp    = "\tlogging data output.";
	regCmd_setLogOut.pFunc    = LOG_output;
	shell_registerCommand( &regCmd_setLogOut );
#endif
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
	va_list		ap;
	LOG_DATA_t	msg = {0};
	int			i, idx, retv = 0;
	bool		loop = true;

	if( type < LOG_level ) {
		return 0;		// ���O�}�~
	}
	if( type >= MAX_LOG_TYPE ) {
		return -1;
	}

	msg.tick  = TIMms_getTick();
	msg.type  = type;
	msg.line  = (uint16_t)line;
	msg.fn_id = fn_id;

	if( (argc & 0x01)||(argc > 8)  ) {
		return -1;
	}
	va_start( ap, argc );

	idx = 0;
	for( i = 0; (i < argc /2)&&(loop); i++ ) {
		msg.tlv.tag[idx]       = (LOG_TAG_t)va_arg( ap, int );
		msg.tlv.val[idx].dword = va_arg( ap, uint32_t );
		switch( msg.tlv.tag[idx] ) {
		case TAG_STR:
		case TAG_DEC:
		case TAG_HEX:
			idx++;
			break;
		case TAG_TERM:
			msg.tlv.val[idx].dword = 0;
			loop = false;
			break;
		default:
			loop = false;
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
 * @brief ���O�ǂݏo��API
 *
 * ���O�i�[�o�b�t�@���烍�O�𕶎���ŕԂ�.
 *
 * @param[out]	str     ���O���i�[���镶����|�C���^.
 * @param[in]	sz      �ő啶����o�b�t�@�T�C�Y.(byte)
 * @retval	0	Success
 * @retval	-1	���O�f�[�^�Ȃ�.
 */
int LOG_read( char *str, size_t sz )
{
	LOG_DATA_t	*p_rd;
	int			len, save_len, i;
	bool		loop = true;
	int			retv = -1;

	if( RdPos != WtPos ) {
		if( RdPos >= MAX_LOG_POOL ) {
			RdPos = 0;
		}
		p_rd = &LOG_buff[RdPos];
		len = snprintf( str, sz, "%lu %s 0x%x %d ",
					 p_rd->tick, LOG_STR[p_rd->type], p_rd->fn_id, p_rd->line );
		if( (len <= 0)||(len >= sz) ) {
			return -1;
		}
		save_len = len;
		for( i=0; (i < 4)&&(loop); i++ ) {
			switch( p_rd->tlv.tag[i] ) {
			case TAG_STR:
				len = snprintf( &str[save_len], sz -save_len, " %s", p_rd->tlv.val[i].ptr );
				break;
			case TAG_DEC:
				len = snprintf( &str[save_len], sz -save_len, " %ld", p_rd->tlv.val[i].dword );
				break;
			case TAG_HEX:
				len = snprintf( &str[save_len], sz -save_len, " 0x%lX", p_rd->tlv.val[i].dword );
				break;
			case TAG_TERM:
				loop = false;
				retv = 0;
				break;
			}
			if( (len <= 0)||(len >= sz) ) {
				loop = false;
				break;
			}
		}
		RdPos++;
	}
	return retv;
}

/**
 * @} end of addtogroup
 */
