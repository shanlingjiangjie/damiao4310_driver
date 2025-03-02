/**
 * @file    rtos_tasks.c
 * @author  Deadline039
 * @brief   RTOS tasks.
 * @version 1.0
 * @date    2024-01-31
 */

 #include "includes.h"

 dm_handle_t motor_4310;
 
 static TaskHandle_t start_task_handle;
 void start_task(void *pvParameters);
 
 static TaskHandle_t task1_handle;
 void task1(void *pvParameters);
 
 static TaskHandle_t task2_handle;
 void task2(void *pvParameters);
 
 static TaskHandle_t task3_handle;
 void task3(void *pvParameters);
 
 /*****************************************************************************/
 
 /**
  * @brief FreeRTOS start up.
  *
  */
 void freertos_start(void) {
     xTaskCreate(start_task, "start_task", 256, NULL, 2, &start_task_handle);
     vTaskStartScheduler();
 }
 
 /**
  * @brief Start up task.
  *
  * @param pvParameters Start parameters.
  */
 void start_task(void *pvParameters) {
     UNUSED(pvParameters);
     taskENTER_CRITICAL();
 
     xTaskCreate(task1, "task1", 256, NULL, 3, &task1_handle);
     xTaskCreate(task2, "task2", 128, NULL, 2, &task2_handle);
     xTaskCreate(task3, "task3", 128, NULL, 2, &task3_handle);
 
     vTaskDelete(start_task_handle);
     taskEXIT_CRITICAL();
 }
 
 /**
  * @brief Task1: Damiao motor driver.
  *
  * @param pvParameters Start parameters.
  */
 void task1(void *pvParameters) {
     UNUSED(pvParameters);
     dm_motor_init(&motor_4310,0x00,0x01,DM_MODE_MIT,DM_J4310,12.5,30,10,can1_selected);
     //注意调整模式时要在上位机进行对应的修改
     dm_motor_enable(&motor_4310);
     dm_save_zero(&motor_4310);
     while(1)
     {
         dm_mit_ctrl(&motor_4310,6.28,3,0,0,0);
         vTaskDelay(1);
     }   
 }
 
 /**
  * @brief Task2: Blink.
  *
  * @param pvParameters Start parameters.
  */
 void task2(void *pvParameters) {
     UNUSED(pvParameters);
 
     LED0_OFF();
     LED1_ON();
 
     while (1) {
         LED0_TOGGLE();
         LED1_TOGGLE();
         vTaskDelay(1000);
     }
 }
 
 /**
  * @brief Task3: Scan the key and print which key pressed.
  *
  * @param pvParameters Start parameters.
  */
 void task3(void *pvParameters) {
     UNUSED(pvParameters);
 
     key_press_t key = KEY_NO_PRESS;
 
     while (1) {
         key = key_scan(0);
         switch (key) {
             case WKUP_PRESS: {
                 printf("Wake Up Pressed. \r\n");
             } break;
 
             case KEY0_PRESS: {
                 printf("KEY0 Pressed. \r\n");
             } break;
 
             case KEY1_PRESS: {
                 printf("KEY1 Pressed. \r\n");
             } break;
 
             case KEY2_PRESS: {
                 printf("KEY2 PRessed. \r\n");
             } break;
 
             default: {
             } break;
         }
 
         vTaskDelay(10);
     }
 }
