/**
 * @file  AplTemplate.h
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
#ifndef _AplTemplate_H_
#define _AplTemplate_H_

/**
 * @page AplTemp_TITLE �A�v���e���v���[�g
 *
 * @section AplTemp_API API�ꗗ
 * | API��          | �@�\          |
 * |:-------------- |:------------- |
 * | AplTemp_init()  | �����ݒ�   |
 *
 * @section AplTemp_USAGE �g�p���@.
 * - �\�߈�x���� AplTemp_init() ���Ăяo������.
 */
/**
 * @addtogroup AplTemp_GROUP �A�v���e���v���[�g�@�\.
 * @{
 */

/**
 * @brief API���^�[���R�[�h.
 */
typedef enum AplTemp_ERRCD {
	AplTemp_OK		= 0x0000,		/**< ����I�� */
	AplTemp_PARAM	= 0x0001,		/**< �p�����[�^�G���[ */
	AplTemp_STATE	= 0x0002,		/**< �X�e�[�g�G���[ */
	AplTemp_MAX_ERRCD
}AplTemp_ERRCD;


/**
 * @brief API���b�Z�[�W�f�[�^�^.
 */
typedef struct AplTemp_MSG_t {
    /// �@�\�R�[�h.
    uint16_t    feature;
}AplTemp_MSG_t;


/**
 * @brief �����ݒ�API.
 */
extern void AplTemp_init( void );


/**
 * @} end of addtogroup
 */
#endif //_AplTemplate_H_
