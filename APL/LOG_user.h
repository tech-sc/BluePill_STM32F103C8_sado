/**
 * @file  LOG_user.h
 * @brief ���O������ƃ��[�U�@�\�R�[�h�̒�`.
 * 
 * ���O�o�͂��镶����ƃ��[�U�@�\�R�[�h��APL����`����.
 * 
 * @author      application users
 * @date        2019/07/27
 * @version     Rev0.10
 * 
 * @par �ύX����:
 * - Rev0.01: 2019/07/27: �V�K�쐬
 */
/**
 * @addtogroup GROUP_LOG_user
 * @{
 */
/**
 * @brief ���[�U�@�\�R�[�h.
 */
typedef enum LOG_FNID_t {
	FID_TIMER	= 0x0101,
	FID_MAX
}LOG_FID_t;

/**
 * @brief ���O�o�͕�����R�[�h.
 */
typedef enum LOG_STRID_t {
	STRID_DEBUG		= 0x0000,
	STRID_INFO		= 0x0001,
	STRID_WARNING	= 0x0002,
	STRID_ERROR		= 0x0003,
	STRID_PARAM		= 0x0004,	/**< param */
	STRID_MAX
}LOG_STRID_t;

/**
 * @brief ���O�o�͕�����.
 * @ref LOG_STR of LOG_user.c
 */
extern char	*LOG_STR[STRID_MAX];

/**
 * @} end of addtogroup
 */
