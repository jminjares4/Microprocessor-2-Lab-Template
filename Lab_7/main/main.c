/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering
    
    Course:     EE 4178/5190 Laboratory for Microprocessors Systems II

    Lab 7:
        Objective:
            The objective for this lab is to understand how use the Espressif `LEDC` and `ADC` drivers. In this lab, 
            create 2 task: adc and pwm task. The `adc task` perform ADC readings every 100 millisecond (10 hertz). 
            This task must store the adc reading and be sent through a queue. Lastly, `pwm task` should receive 
            the queue data from the `adc task` and update `ledc` duty cycle. 

            | Task          |  Description               | Queue        |
            | :---          | :---                       | :---         |
            | `adc_task`    | Read analog value @ 10 hz  | send data    |
            | `pwm_task`    | Update duty cycle          | receive data |

        Bonus:
            Undergrad Bonus:
                Add `gpio` interrupt to start and stop **PWM** signal
            Grad Bonus:
                Modifiy code to use **2** more PWM pins. 
*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <driver/ledc.h>
#include <driver/adc.h>

/* Global queue handle */
QueueHandle_t duty_queue = NULL;

/* ADC task */
void ADCtask(void *pvParameters)
{
    while (1)
    {
       /* Read adc channel*/

       /* Send data to queue */
    }
}

/* PWM task */
void PWMtask(void *pvParameters)
{
    while (1)
    {
        /* Receive data */

        /* Update duty cycle */

    }
}

/* ADC setup */
void adc_setup(void)
{
    /* Set adc */
}

/* PWM setup */ 
void pwm_setup(void)
{

}

void app_main()
{
    /* Setup ADC and PWM */

    /* Create queue */
    

    /* Create tasks */
    xTaskCreate(&ADCtask, "ADCtask", 2048, NULL, 5, NULL);
    xTaskCreate(&PWMtask, "PWMtask", 2048, NULL, 5, NULL);
}