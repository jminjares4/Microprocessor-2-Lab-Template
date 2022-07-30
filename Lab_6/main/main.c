/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering
    
    Course:     EE 5190 Laboratory for Microprocessors Systems II

    Lab 6:
        Objective:
            Understand how to use queues and interrupts with FreeRTOS Queues. In this lab, students
            will learn how to send qyedata between tasks using interrupts. Student must create a program
            that uses `gpio` interrupts as in Lab 2, please feel free to use either configuration (Pull-up | Pull-down).
            The interrupt service routine (ISR) should send data (gpio pin number) to a queue and receive by task: `print_task`.
            The `print_task` must receive the data and display it. For instance, if the interrupt was trigger by pin 23, the output
            would be: "GPIO 23 received!!!". 

            | Task          |    Objective                               |
            | :---          | :---                                       |
            | print_task    | Receive data and print to terminal/monitor |

            | Interrupt     | Objective                 |
            | :---          | :---                      |
            | gpio_isr      | Send data to `print_task` |

        Bonus:
            Undergrad Bonus:
                Modify code to use two more interrupts. 
            
            Grad Bonus:
                Modify code to use two more interrupts and add two LEDs. 
                Each interrupt must toggle its corresponding LED. 


*/

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

#define ESP_INTR_FLAG_DEFAULT   0   /* Interrupt flag configuration */

/* Global Queue handle */
QueueHandle_t xQueue;

/* GPIO interrupt handler */
static void IRAM_ATTR gpio_isr_handler(void *arg)
{

}

/* Print task */
void print_task(void *pvParameters)
{
    
}

/* GPIO interrupt setup */
void gpio_interrupt_setup(void)
{


}

void app_main()
{
    /* Create a queue */

    /* Call gpio interrupt setup */
    gpio_interrupt_setup(); 

    /* Create task */
    xTaskCreate(&print_task, "print_task", 2048, NULL, 10, NULL); 

}