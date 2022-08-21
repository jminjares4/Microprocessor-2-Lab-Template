/*
    Author:     Jesus Minjares and Erick A. Baca
                Master of Science in Computer Engineering
    
    Course:     EE 4178/5190 Laboratory for Microprocessors Systems II            
    
    Lab 3:
        Objective:
            Understand how to use the FreeRTOS with Espressif. The lab will consist of creating 
            two main tasks. `Task 1` must toggle onboard led every 250 ms, and `Task 2` must print
            'Hello World!' every 2 seconds.

            | Tasks  | Objective                     |
            | :---   | :---                          |
            | Task 1 | Toggle `onboard led` @ 250 ms |
            | Task 2 | Print `Hello World!` @ 2 sec  |

        Bonus:
            Undergrad Bonus:
                Create an additional task that toggles an external led and print it states
            Grad Bonus:
                Create an addtional task that runs any led sequence from the previous labs and it should run every 5 seconds

*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

/* Define pin 13 as a “BLINK_GPIO” */
#define BLINK_GPIO 13

/* Code for the hello_task task */
void hello_task(void *pvParameters)
{
    
}

/* Code for the blinky task */
void blinky_task(void *pvParameters)
{
    
}

void led_sequence_task(void *pvParameters){

}

void app_main()
{
    /* Create the task defined by xTaskCreate.*/
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&blinky, "blinky", 512, NULL, 5, NULL);

    /*Create any additional tasks*/
    
}
