/**
 * @file  TIMms.h
 * @brief �~���b�I�[�_�[�^�C�}�@�\.
 * 
 * �~���b�P�ʂŗv���\�ȃ\�t�g�E�F�A�^�C�}��APL�ɒ񋟂���.
 * 
 * @author      Teru
 * @date        2019.06.27
 * @version     Rev0.01
 * @copyright   2019 Emb-se.com.
 * 
 * @par �ύX����:
 * - Rev0.01: 2019.06.27: �V�K�쐬
 * 
 * @copyright   2019 Emb-se.com.
 */
/**
 * @page TIMER_title �^�C�}���C�u����
 *
 * @section TIMER_api API�ꗗ
 * | API��          | �@�\          |
 * |:-------------- |:------------- |
 * | TIMms_initTimer()  | �����ݒ�   |
 * | TIMms_reqTimer()   | �^�C�}�v�� |
 * | TIMms_cancelTimer()| �^�C�}��� |
 *
 * @section TIMER_usage �g�p���@
 * - �\�߈�x���� TIMms_initTimer() ���Ăяo���B
 */

typedef uint16_t    TIMCNT_t;	/**< �^�C�}�J�E���^���W�X�^�^(16bit�J�E���^) */

typedef struct TIMms_t {
    int64_t     remain_tick;		/**< ���΂܂ł̎c��TICK */
    void   (*expire_cb)(void *hdl);	/**< ���΃R�[���o�b�N�֐� */
    struct TIMms_t *next_list;		/**< �v���܂��͔��΃��X�g */
    TIMCNT_t    old_tick;			/**< �v�������܂��͔��Ώ�������TICK�l */
    uint16_t    feature;			/**< �I�v�V�����@�\�R�[�h */
}TIMms_t;

/** �I�v�V�����@�\�R�[�h */
#define TIMms_FEATURE	0x5000		/**< �~���b�I�[�_�[�^�C�}�@�\�R�[�h */
#define TIMms_STARTING	0x0100		/**< �^�C�}�v�������� */
#define TIMms_STOPING	0x0200		/**< �^�C�}��~������ */
#define TIMms_ONESHORT	0x0010		/**< OneShot */
#define TIMms_REPEAT	0x0020		/**< ���s�[�g */

							/* 1TICK=0.25msec */
#define MSEC_TO_TICK(ms)		(int64_t)((int64_t)ms * 4) +1
#define TICK_TO_MSEC(tick)		(int32_t)(tick / 4)

#define TIMms_getTick			TIMER_getTick

/**
 * @brief �����ݒ�API
 */
extern void TIMms_initTimer( void );

/**
 * @brief ���񔭉�TICK�Z�b�g
 */
extern void TIMms_setNextExpire( TIMCNT_t tick );

/**
 * @brief �^�C�}�v��API
 *
 * �~���b�P�ʂ̃^�C�}�v����񋟂���API�֐��ł���.
 * ���΂̓R�[���o�b�N�ɂĒʒm����.
 *
 * @param[in]	time  �^�C�}���ԁB�~���b�Ŏw�肷��.
 * @param[in] expire_cb  ���Ύ��̃R�[���o�b�N�֐����w�肷��.
 * @param[in]	p_req  �^�C�}�v���f�[�^�̈��n��.
 * @retval  handle			�^�C�}�v������������ƁA���̃^�C�}�n���h����Ԃ�.
 * @retval  NULL	�p�����[�^�G���[.
 * @pre     time��1ms�ȏ�ł��邱��.
 *			        expire_cb��NULL�łȂ�����.
 *          p_req��NULL�łȂ�����.
 * @post    �^�C�}��~�͕Ԃ����n���h���Œ�~�\�ł���. �܂����Ύ��ɊY���n���h����ʒm����.
 */
extern void *TIMms_reqTimer( int32_t time, void (*expire_cb)(void *handle), TIMms_t *p_req );

/**
 * @brief �^�C�}���API
 *
 * �^�C�}�v���������API�֐��ł���.
 * API�̎g�����ɂ���Ă͎���Ɣ��΂�����Ⴄ�\��������̂ŌĂяo�����őΉ����邱��.
 *
 * @param[in]	handle �^�C�}�v����������^�C�}�n���h��.
 * @retval  handle	�������������ƁA��������^�C�}�n���h����Ԃ�.
 * @retval  NULL	�p�����[�^�G���[.
 *                                        �܂��̓G���g������ĂȂ��i���ɔ��΂����j
 * @pre   handle��NULL�łȂ�����.
 */
extern void *TIMms_cancelTimer( TIMms_t *handle );

/**
 * @brief �^�C�}�����݃n���h������R�[������锭�Ώ���
* @param[in]	over =0/=1:�J�E���^Over flow.
 */
extern void TIMms_expire( int over );

/**
 * @brief �^�C�}���Ώ����^�X�N
 */
extern void TIMms_task( void *arg );

