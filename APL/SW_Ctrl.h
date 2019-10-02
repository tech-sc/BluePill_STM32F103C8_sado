/**
 * @file  SwCtrllate.h
 * @brief �A�v���e���v���[�g.
 * 
 * RTOS���\�[�X���g�p����A�v���P�[�V�����e���v���[�g�ł���.
 * 
 * @author      Teru
 * @date        2019/09/22
 * @version     Rev0.10
 * @copyright   2019 Emb-se.com.
 * 
 * @par �ύX����:
 * - Rev0.01: 2019/09/22: �V�K�쐬.
 * 
 * @copyright   2019 Emb-se.com.
 */
#ifndef _SwCtrl_H_
#define _SwCtrl_H_

/**
 * @page SwCtrl_TITLE �A�v���e���v���[�g
 *
 * @section SwCtrl_API API�ꗗ
 * | API��          | �@�\          |
 * |:-------------- |:------------- |
 * | SwCtrl_init()  | �����ݒ�   |
 *
 * @section SwCtrl_USAGE �g�p���@.
 * - �\�߈�x���� SwCtrl_init() ���Ăяo������.
 */
/**
 * @addtogroup SwCtrl_GROUP �A�v���e���v���[�g�@�\.
 * @{
 */

/**
 * @brief API���^�[���R�[�h.
 */
typedef enum SwCtrl_ERRCD {
	SwCtrl_OK		= 0x0000,		/**< ����I�� */
	SwCtrl_PARAM	= 0x0001,		/**< �p�����[�^�G���[ */
	SwCtrl_STATE	= 0x0002,		/**< �X�e�[�g�G���[ */
	SwCtrl_MAX_ERRCD
}SwCtrl_ERRCD;


/**
 * @brief �����ݒ�API.
 */
extern void SwCtrl_init( void );


/**
 * @} end of addtogroup
 */
#endif //_SwCtrl_H_
