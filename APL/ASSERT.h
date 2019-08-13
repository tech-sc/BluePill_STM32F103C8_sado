/**
 * @file  ASSERT.h
 * @brief �A�T�[�g�@�\.
 * 
 * �A�T�[�gAPL�ɒ񋟂���.
 * 
 * @author      Teru
 * @date        2019/08/04
 * @version     Rev0.10
 * @copyright   2019 Emb-se.com.
 * 
 * @par �ύX����:
 * - Rev0.01: 2019/08/04: �V�K�쐬.
 * 
 * @copyright   2019 Emb-se.com.
 */
#ifndef _ASSERT_H_
#define _ASSERT_H_

/**
 * @page ASSERT_title �A�T�[�g���C�u����
 *
 * @section ASSERT_api API�ꗗ
 * | API��          | �@�\          |
 * |:-------------- |:------------- |
 * | assert_nv()    | ���^�[���R�[�h�Ȃ�ASSERT |
 * | assert_val()   | ���^�[���R�[�h����ASSERT |
 *
 * @section ASSET_usage �g�p���@
 * - �\�߈�x���� TIMms_initTimer() ���Ăяo���B
 */
/**
 * @addtogroup GROUP_ASSERT �A�T�[�g�@�\.
 * @{
 */

__weak void LOG( char const *func, int line )
{
}

/**
 * @brief �����ݒ�API
 */
extern void ASSERT_init( void );

/**
 * @brief ���^�[���R�[�h�Ȃ�ASSERT
 */
#define ASSERT_nv( exp )			if(!(exp)) { LOG(__FUNCTION__, __LINE__); return; }

/**
 * @brief ���^�[���R�[�h����ASSERT
 */
#define ASSERT_val( exp, retv )		if(!(exp)) { LOG(__FUNCTION__, __LINE__); return retv; }

/**
 * @} end of addtogroup
 */
#endif //_TIMms_H_
