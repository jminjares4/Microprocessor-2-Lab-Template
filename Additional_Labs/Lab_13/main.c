#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/*Task Handlers*/
TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t Task3;
TaskHandle_t Task4;
TaskHandle_t Task5;
/* Semaphores are placed as global */
SemaphoreHandle_t mySemaphore3 = NULL;
SemaphoreHandle_t mySemaphore4 = NULL;
SemaphoreHandle_t mySemaphore6 = NULL;

/*
    xTaskGetTickCount() keeps track of the timing.
    It was used to keep as a check that to be sure
    that timing was correct, to a degree

    And the arrow "->" was used to as an indicator
    that the task recieved a semaphore, once again
    just to make easier if it needs debugging later 
    on.
*/

void Task_code1(void *parameter)
{
    while(1)
    {
        printf("Task 1 running [%i]\n", xTaskGetTickCount());
        vTaskDelay(2500 / portTICK_PERIOD_MS); 
    }

}
void Task_code2(void *parameter)
{
    while(1)
    {
        printf("Task 2 running [%i]\n", xTaskGetTickCount());
        xSemaphoreGive(mySemaphore3);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
    }
}
void Task_code3(void *parameter)
{
    while(1)
    {
        if(xSemaphoreTake(mySemaphore3, 100/portTICK_PERIOD_MS) == pdTRUE)
        {
            printf("Task 3 running [%i]\n", xTaskGetTickCount());
            vTaskDelay(500/portTICK_RATE_MS);
            xSemaphoreGive(mySemaphore4);
        }
        else
        {
            vTaskDelay(100/portTICK_RATE_MS);
        }
    }
}
void Task_code4(void *parameter)
{
    while(1)
    {
        if(xSemaphoreTake(mySemaphore4, 100/portTICK_PERIOD_MS) == pdTRUE)
        {
            printf("Task 4 running [%i]\n", xTaskGetTickCount());
            vTaskDelay(500/portTICK_RATE_MS);
            xSemaphoreGive(mySemaphore6);
        }
        else
        {
            vTaskDelay(100/portTICK_RATE_MS);
        }
    }
}
void Task_code5(void *parameter)
{
    while(1)
    {
        if(xSemaphoreTake(mySemaphore6, 100/portTICK_PERIOD_MS) == pdTRUE)
        {
            printf("Task 5 running [%i]\n", xTaskGetTickCount());
            vTaskDelay(2000/portTICK_RATE_MS);
        }
        else
        {
            vTaskDelay(100/portTICK_RATE_MS);
        }
    }
}



void app_main(void)
{

        /* Created 3 mySemaphores which are going*/
    mySemaphore3 = xSemaphoreCreateBinary();
    mySemaphore4 = xSemaphoreCreateBinary();
    mySemaphore6 = xSemaphoreCreateBinary();

    /*Creation of the Task pinned to a core*/
    xTaskCreatePinnedToCore(Task_code1,"Task_1",2048,NULL,2, &Task1 ,0);
    xTaskCreatePinnedToCore(Task_code2,"Task_2",2048,NULL,2, &Task2 ,1);
    xTaskCreatePinnedToCore(Task_code3,"Task_2",2048,NULL,2, &Task3 ,0);
    xTaskCreatePinnedToCore(Task_code4,"Task_2",2048,NULL,2, &Task4 ,1);
    xTaskCreatePinnedToCore(Task_code5,"Task_2",2048,NULL,2, &Task5 ,0);
    
}