/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering
    Course:     EE 5190 Laboratory for Microprocessors Systems II

    Lab 5:

                Understand how to use the queues with FreeRTOS. In this lab,
                the main focus will be using queues to send data among tasks. 

                * Create a program which will create a queue to send and receive data.
                * Send data to the queue from multiple tasks, sending integers `10` and `20`
                  also  for strings `“EE4178”` and `“Fall2021”`.
                * Receive and print data from the queue

            Bonus
                - Undergrad Bonus:
                    * Modify the code by sending structures on a queue.
                - Grad Bonus:
                    * Modify the code by sending structures on a queue.
                      If `isLedOn` *true*, turn the `LED` else, turn off.

    Date:       09-22-2021
    Pinout:
                                         +-----------------------+
                                         | O      | USB |      O |
                                         |        -------        |
                                     3V3 | [ ]               [ ] | VIN
                                     GND | [ ]               [ ] | GND
     Touch3 / HSPI_CS0 / ADC2_3 / GPIO15 | [ ]               [ ] | GPIO13 / ADC2_4 / HSPI_ID / Touch4
 CS / Touch2 / HSPI_WP / ADC2_2 /  GPIO2 | [ ]               [ ] | GPIO12 / ADC2_5 / HSPI_Q / Touch5
      Touch0 / HSPI_HD / ADC2_0 /  GPIO4 | [ ]               [ ] | GPIO14 / ADC2_6 / HSPI_CLK / Touch6
                         U2_RXD / GPIO16 | [ ]               [ ] | GPIO27 / ADC2_7 / Touch7
                         U2_TXD / GPIO17 | [ ]               [ ] | GPIO26 / ADC2_9 / DAC2
                      V_SPI_CS0 /  GPIO5 | [ ]  ___________  [ ] | GPIO25 / ADC2_8 / DAC1
                SCK / V_SPI_CLK / GPIO18 | [ ] |           | [ ] | GPIO33 / ADC1_5 / Touch8 / XTAL32
        U0_CTS / MSIO / V_SPI_Q / GPIO19 | [ ] |           | [ ] | GPIO32 / ADC1_4 / Touch9 / XTAL32
                 SDA / V_SPI_HD / GPIO21 | [ ] |           | [ ] | GPIO35 / ADC1_7 
                  CLK2 / U0_RXD /  GPIO3 | [ ] |           | [ ] | GPIO34 / ADC1_6 
                  CLK3 / U0_TXD /  GPIO1 | [ ] |           | [ ] | GPIO39 / ADC1_3 / SensVN 
        SCL / U0_RTS / V_SPI_WP / GPIO22 | [ ] |           | [ ] | GPIO36 / ADC1_0 / SensVP 
                MOSI / V_SPI_WP / GPIO23 | [ ] |___________| [ ] | EN 
                                         |                       |
                                         |  |  |  ____  ____  |  |
                                         |  |  |  |  |  |  |  |  |
                                         |  |__|__|  |__|  |__|  |
                                         | O                   O |
                                         +-----------------------+
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/* Undergraduate Bonus */
typedef struct {
    char str[10];
    int32_t val;
}Undergaduate_t;

/* Graduate Bonus */
typedef struct {
    char str[10];
    int32_t val;
    char studentName[15];
    uint32_t studentID;
    bool isLedOn;
}Graduate_t;

/* Global Queue handle */
QueueHandle_t xQueue;

void task1(void *pvParameters){
    int32_t data = (int32_t)pvParameters; //store data pass by argument
    while(1){
        xQueueSendToBack(xQueue, &data, 0); //send data to the queue
        vTaskDelay(1000/portTICK_RATE_MS); // 1 second delay
    }
}

void task2(void *pvParameters){
    int32_t dataReceive;

    while(1){
        if(xQueueReceive(xQueue, &dataReceive, (TickType_t)1000) == pdPASS){
            printf("Data received: %d\n", dataReceive); //print data received 
            vTaskDelay(100 / portTick_RATE_MS);  //100 ms
        }
    }
}


// if you do not want to use static, comment the ones in main and uncomment these ones
// int32_t data1 = 10;
// int32_t data2 = 20;

void app_main(void){
    /*  Create static variable so they will be store in static memory
        and once the main terminate, it content will not be corrupted.

        If you do not want to use static variables make the global!!! :)
    */
    static int32_t data1 = 10, data2 = 20;

    /* Create Queue of type int32_t */
    xQueue = xQueueCreate(5, sizeof(int32_t));
   
    /* Check if the queue was create successfully */
    if(xQueue != NULL){
        /* Create sender tasks and send int32_t as arguments */
        xTaskCreate(&task1, "Task 1a ", 2048, (void *)data1, 1, NULL);
        xTaskCreate(&task1, "Task 1b", 2048, (void *)data2, 1, NULL);

        /* Create reciever task */
        xTaskCreate(&task2,"Task 2", 2048, NULL, 2, NULL);
    }
}