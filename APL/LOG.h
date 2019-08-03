/**
 * @file  LOG.h
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
#include <stdarg.h>
#include "LOG_user.h"

/**
 * @brief ���O T1-TAG�f�[�^�^.
 */
typedef enum LOG_TAG_t {
	TAG_NONE	= 0,		/**< TAG�Ȃ� */
	TAG_STR		= 0x02,		/**< ������o�̓f�[�^TAG�A�f�[�^��32bit�|�C���^. */
	TAG_DEC		= 0x04,		/**< 10�i���o�̓f�[�^TAG�A�f�[�^��32bit */
	TAG_HEX		= 0x08,		/**< 16�i���o�̓f�[�^TAG�A�f�[�^��32bit */
}LOG_TAG_t;

/**
 * @brief ���O T1-VALUE�f�[�^�^.
 * ������R�[�h/10�i��/16�i�� �o�̓f�[�^���Z�b�g����.
 */
typedef uint32_t	LOG_VAL_t;

/**
 * @brief ���O�f�[�^�^�C�v�^.
 */
typedef enum LOG_TYPE_t {
	LOG_DEBUG	= 1,		/**< �f�o�b�O�^�C�v. */
	LOG_INFO	= 2,		/**< ���^�C�v. */
	LOG_WARNING	= 3,		/**< ���[�j���O�^�C�v. */
	LOG_ERROR	= 4,		/**< �G���[�^�C�v. */
}LOG_TYPE_t;


/**
 * @brief ���O�����ݒ�API
 */
void LOG_init( void );

/**
 * @brief ���O��������.
 */
int LOG_write( LOG_TYPE_t type, int line, LOG_FID_t fn_id, int argc, ... );

/**
 * @} end of addtogroup
 */
