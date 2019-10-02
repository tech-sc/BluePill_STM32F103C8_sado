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

/**
 * @brief ���O�^�C�v�^.
 */
typedef enum LOG_TYPE_t {
	/// �f�o�b�O�^�C�v.
	LOG_DEBUG = 0,
	/// �p�����[�^���^�C�v.
	LOG_PARAM,
	/// ���^�C�v.
	LOG_INFO,
	/// ���[�j���O�^�C�v.
	LOG_WARNING,
	/// �G���[�^�C�v.
	LOG_ERROR,
	/// �I�[(�w��s��)
	MAX_LOG_TYPE
}LOG_TYPE_t;

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
	/// ������o��TAG�F�f�[�^��32bit�|�C���^.
	TAG_STR,
}LOG_TAG_t;


/**
 * @brief ���O�����ݒ�API
 */
void LOG_init( LOG_TYPE_t level );

/**
 * @brief ���O��������API
 */
int LOG_write( LOG_TYPE_t type, int line, uint16_t fn_id, int argc, ... );

/**
 * @brief ���O�ǂݏo��API
 */
int LOG_read( char *str, size_t sz );

/**
 * @} end of addtogroup
 */