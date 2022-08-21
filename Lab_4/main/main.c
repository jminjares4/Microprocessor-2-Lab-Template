/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering
                
    Course:     EE 4178/5190 Laboratory for Microprocessors Systems II   

    Lab 4:
        Objective:
            Understand how to use the semaphore with FreeRTOS. In this lab, the main focus
            will be using semaphores to sycnrhonize various tasks. Students must create four 
            different tasks. Task 1 should run every two seconds. Task 2 should run twice as
            fast as task 1. Task 3 should run every time either task 1 or task 2 run. Lastly,
            task 4 should run every time task 3 runs. Each task must display "Task X running". 

            | Task          | Priority      | Description                           |
            | :---          | :---          | :---                                  | 
            | Task 1        | High          | Run every 2 seconds                   | 
            | Task 2        | High          | Run twice as fast as Task 1           |     
            | Task 3        | Low           | Run everytime either Task 1 or Task 2 |    
            | Task 4        | Low           | Run everytime Task 3 runs             |   

            Task Note                 
                * Low priority numbers denote **low** priority tasks

        Bonus:
            Undergrad Bonus:
                    Turn an led every time `Task 3` or `Task 4` runs.
            Grad Bonus:
                    Create two indepenent tasks: `Task 5` and `Task 6`.
                    `Task 5` will send a semaphore to `Task 6` which turns on the `onboard led`. 

*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/* Create global instance semaphore */
SemaphoreHandle_t xSemaphore = NULL;

/* Task 1 */
void task1(void *pvParameters)
{
    while(1)
    {
        printf("Task 1\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

/* Task 2 */
void task2(void *pvParameters)
{
    while(1)
    {
        printf("Task 2\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    /* Create semaphores */
    xSemaphore = xSemaphoreCreateBinary();

    /* Create tasks */
    xTaskCreate(&task1, "task1", 4096, NULL, 5, NULL);
    xTaskCreate(&task2, "task2", 4096, NULL, 5, NULL);

}