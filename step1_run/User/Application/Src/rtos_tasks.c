/**
 * @file    rtos_tasks.c
 * @author  shanlingjiangjie
 * @brief   RTOS任务
 * @version 1.0
 * @date    2025-3-2
 */

#include "includes.h"
dm_handle_t motor_4310;

static TaskHandle_t start_task_handle;
void start_task(void *pvParameters);

static TaskHandle_t dm4310_handle;
void dm4310_task(void *pvParameters);

/**
 * @brief FreeRTOS启动函数
 * 
 */
void freertos_start(void) {
    xTaskCreate(start_task, "start_task", 128, NULL, 2, &start_task_handle);
    vTaskStartScheduler();
}

/**
 * @brief 启动任务
 * 
 * @param pvParameter 传入参数(未用到)
 */
void start_task(void *pvParameters) {
    UNUSED(pvParameters);
    taskENTER_CRITICAL();

    xTaskCreate(dm4310_task, "dm4310_task", 128, NULL, 2, &dm4310_handle);

    vTaskDelete(start_task_handle);
    taskEXIT_CRITICAL();
}

/**
 * @brief 任务 达妙4310电机驱动
 * 
 * @param pvParameter 传入参数(未用到)
 * 
 */
void dm4310_task(void *pvParameters) {
    UNUSED(pvParameters);
    key_press_t key = KEY_NO_PRESS;
    int8_t num = 0;
    float angle = 0.0f;
    dm_motor_init(&motor_4310, 0x00, 0x01, DM_MODE_MIT, DM_J4310, 12.5, 30, 10,
                  can1_selected);
    //注意调整模式时要在上位机进行对应的修改
    dm_motor_enable(&motor_4310);
    dm_save_zero(&motor_4310);
    while (1) {
        key = key_scan(0);
        if (key == KEY1_PRESS) {
            num--;
        }
        if (key == KEY2_PRESS) {
            num++;
        }
        if (num >= 3) {
            num = 3;
        }
        if (num <= -3) {
            num = -3;
        }

        angle = PI * num;
        dm_mit_ctrl(&motor_4310, angle, 0, 2, 1, 0);
    }
}