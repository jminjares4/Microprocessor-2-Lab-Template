/*
    Author:     Jesus Minjares and Erick A. Baca
                Master of Science in Computer Engineering
    
    Course:     EE 4178/5190 Laboratory for Microprocessors Systems II            
    
    Lab 3:
        Objective:
            Understand how to use the FreeRTOS with Espressif. The lab will consist of creating 
            two main tasks. `Task 1` will toggle onboard led every 250 ms, and `Task 2` must print
            'Hello World!' every 2 seconds.

            | Tasks  | Objective                     |
            | :---   | :---                          |
            | Task 1 | Toggle `onboard led` @ 250 ms |
            | Task 2 | Print `Hello World!` @ 2 sec  |

        Bonus:
            Undergrad Bonus:
                Create an other task that will toggle the led and print it state in the terminal
            Grad Bonus:
                Create addtional task of to run any led sequence of the previous labs and it should run every 5 seconds

*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

/* Define pin 13 as a “BLINK_GPIO” */
#define BLINK_GPIO 13

/* Code for the hello_task task */
void hello_task(void *pvParameter)
{
    
}

/* Code for the blinky task */
void blinky_task(void *pvParameter)
{
    
}

void led_sequence_task(void *pvParameter){

}

void app_main()
{
    /* Create the task defined by xTaskCreate.*/
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&blinky, "blinky", 512, NULL, 5, NULL);

    /*Create any additional tasks*/
    
}
