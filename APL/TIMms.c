/**
 * @file  TIMms.c
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
 * | API��          | �@�\                             |
 * |:-------------- |:------------- |
 * | TIMms_initTimer()  | �����ݒ�   |
 * | TIMms_reqTimer()   | �^�C�}�v�� |
 * | TIMms_cancelTimer()| �^�C�}��� |
 *
 * @section TIMER_usage �g�p���@
 * - �\�߈�x���� TIMms_initTimer() ���Ăяo���B
 */
#include "TIMER.h"
#include "os.h"
#include "TIMms.h"
#include "ExtLED.h"		//�v���p

/** �����Ǘ��f�[�^ */
TIMms_t     *TIMms_TopReq;
TIMms_t     *TIMms_TailReq;
TIMms_t     *TIMms_TopExpire;

TIMCNT_t    TIMms_NextExpire;

osMutexHandle_t     TIMms_ExpireMutex;


/**
 * @brief �����ݒ�API.
 */
void TIMms_initTimer( void )
{
    osTaskHandle_t  handle;
	int		retv;

	/* �^�C�}�N�� */
	TIMER_init();

	/* �����Ǘ��f�[�^������ */
    TIMms_TopReq     = NULL;
    TIMms_TailReq    = NULL;
    TIMms_TopExpire  = NULL;
    TIMms_NextExpire = (TIMCNT_t)-1;

	/* OS���\�[�X���� */
    osMutex_create( TIMms_ExpireMutex );
    osMutex_take( TIMms_ExpireMutex, portMAX_DELAY );

    /* ���΃R�[���o�b�N�p�^�X�N�𐶐�����. */
    retv = osTask_create( &TIMms_task, "TIMms", TIMmsTASK_STACKSZ/4,
                        NULL, TIMmsTASK_PRI | portPRIVILEGE_BIT, &handle );
    configASSERT( retv == pdPASS );
}

/**
 * @brief ���񔭉�TICK�Z�b�g.
 * @param[in] tick  ������TICK�l.
 */
static void TIMms_setNextExpire( TIMCNT_t tick )
{
    /* �R���y�A�}�b�`���W�X�^��ύX����. */
    TIMER_setCompReg( tick );

    TIMms_NextExpire = tick;
}

/**
 * @brief �^�C�}�v��API.
 *
 * �~���b�P�ʂ̃^�C�}�v����񋟂���API�֐��ł���.
 * ���΂̓R�[���o�b�N�ɂĒʒm����.
 *
 * @param[in] time  �^�C�}���ԁB�~���b�Ŏw�肷��.
 * @param[in] expire_cb  ���Ύ��̃R�[���o�b�N�֐����w�肷��.
 * @param[in] p_req  �^�C�}�v���f�[�^�̈��n��.
 * @retval  handle  �^�C�}�v������������ƁA���̃^�C�}�n���h����Ԃ�.
 * @retval  NULL    �p�����[�^�G���[.
 * @pre     time��1ms�ȏ�ł��邱��.
 *          expire_cb��NULL�łȂ�����.
 *          p_req��NULL�łȂ�����.
 * @post    �^�C�}��~�͕Ԃ����n���h���Œ�~�\�ł���. �܂����Ύ��ɊY���n���h����ʒm����.
 */
void *TIMms_reqTimer( int32_t time, void (*expire_cb)(void *handle), TIMms_t *p_req )
{
    TIMCNT_t  tmp_tick;

    configASSERT(time > 0);
    configASSERT(expire_cb != NULL);
    configASSERT(p_req != NULL);

    p_req->old_tick    = TIMms_getTick();
    p_req->remain_tick = MSEC_TO_TICK(time);
    p_req->expire_cb   = expire_cb;
    p_req->next_list   = NULL;
    p_req->feature     = TIMms_FEATURE;

    osEnterCritical();
    if( TIMms_TailReq != NULL ){
        TIMms_TailReq->next_list = p_req;
        TIMms_TailReq = p_req;
    }else{
        TIMms_TailReq = p_req;
        TIMms_TopReq  = p_req;
    }
    osExitCritical();

    tmp_tick  = p_req->old_tick;
    tmp_tick += p_req->remain_tick;
    if( tmp_tick < TIMms_NextExpire ){
        TIMms_setNextExpire( (TIMCNT_t)tmp_tick );
    }
    return p_req;
}


/**
 * @brief �^�C�}���API.
 *
 * �^�C�}�v���������API�֐��ł���.
 * API�̎g�����ɂ���Ă͎���Ɣ��΂�����Ⴄ�\��������̂ŌĂяo�����őΉ����邱��.
 *
 * @param[in] handle  �^�C�}�v����������^�C�}�n���h��.
 * @retval  handle  �������������ƁA��������^�C�}�n���h����Ԃ�.
 * @retval  NULL    �p�����[�^�G���[.
 *                  �܂��̓G���g������ĂȂ��i���ɔ��΂����j
 * @pre     handle��NULL�łȂ�����.
 */
void *TIMms_cancelTimer( TIMms_t *handle )
{
    TIMms_t     **pre, *p;

    osEnterCritical();
    pre = &TIMms_TopReq;
    p = TIMms_TopReq;
    while(p != NULL) {
        if( p == handle ) {
            if( TIMms_TailReq == handle ) {
                TIMms_TailReq = (*pre)->next_list;
            }
            *pre = p->next_list;
            break;
        }
        pre = &p->next_list;
        p = p->next_list;
    }
    osExitCritical();
    return p;
}


void TIMms_expire( int over )
{
    portBASE_TYPE   dispatch;
    TIMms_t     **pre, *p;
    TIMms_t     **exp;
    TIMCNT_t    now_tick, min_tick;
	int64_t		temp_tick;

	if( over ){
		// �v���p
		ExtLED1_toggle();
	}

    dispatch  = pdFALSE;
	min_tick = (TIMCNT_t)-1;	//set MAX
    now_tick = TIMms_getTick();

    osEnterCritical();
    exp = &TIMms_TopExpire;
    while( *exp != NULL ) {
        *exp = (*exp)->next_list;
    }
    pre = &TIMms_TopReq;
    p = TIMms_TopReq;
    while(p != NULL) {
		if( now_tick > p->old_tick ){
			temp_tick = 0;
		}else{
			temp_tick = (TIMCNT_t)-1;
			temp_tick ++;
		}
        temp_tick  += now_tick;
        temp_tick  -= p->old_tick;

		p->remain_tick -= temp_tick;
        if( p->remain_tick <= 0 ) {
             *pre = p->next_list;
             p->next_list = NULL;
             *exp = p;
             exp = &p->next_list;
             p = *pre;
        } else {
			if( p->remain_tick < min_tick ) {
				min_tick = (TIMCNT_t)p->remain_tick;
			}
			p->old_tick = now_tick;
            p = p->next_list;
        }
    }
	if( TIMms_TopReq == NULL ) {
		TIMms_TailReq = NULL;
	}
	TIMms_setNextExpire( min_tick );
    osExitCritical();

    if( TIMms_TopExpire != NULL ){
        /* �^�X�N���N������ */
        osMutex_giveISR( TIMms_ExpireMutex, &dispatch );
        portEND_SWITCHING_ISR( dispatch );
    }
}

void TIMms_task( void *arg )
{
    TIMms_t     **exp, *p;

    while( 1 ){
        osMutex_take( TIMms_ExpireMutex, portMAX_DELAY );

        if( TIMms_TopExpire != NULL ){
            exp = &TIMms_TopExpire;
            while( *exp != NULL ) {
                p = *exp;
                *exp = p->next_list;
                (p->expire_cb)( p );
            }
        }
    }
}
