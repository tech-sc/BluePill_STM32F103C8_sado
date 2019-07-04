/************************************************************************************//**
 * @file os.h
 * @brief OS�C���^�t�F�[�X��`
 * @note OS�́AFreeRTOS v7.6.0 ���g�p����B
 */
#ifndef _OS_H_
#define _OS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

#define osKernel_start			vTaskStartScheduler
#define osEnterCritical         portENTER_CRITICAL
#define osExitCritical          portEXIT_CRITICAL

#define osTask_create			xTaskCreate

#define osQue_create			xQueueCreate
#define osQue_send				xQueueSend
#define osQue_receive			xQueueReceive

#define osMutex_create			vSemaphoreCreateBinary
#define osMutex_take			xSemaphoreTake
#define osMutex_give			xSemaphoreGive
#define osMutex_giveISR			xSemaphoreGiveFromISR

#define osMem_malloc			pvPortMalloc
#define osMem_free				pvPortFree

typedef xTaskHandle			osTaskHandle_t;
typedef xQueueHandle			osQueHandle_t;
typedef xSemaphoreHandle		osMutexHandle_t;

#endif //_OS_H_
