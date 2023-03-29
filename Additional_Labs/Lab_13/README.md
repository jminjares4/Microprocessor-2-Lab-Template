# **Lab 13 Assignment of Task to Cores**

# **Objective**
The objective of this lab is to learn how to assign tasks to each of the cores manually. Up until now, you have been making tasks, but never had any control of the actual CPU core. In this lab, you will be assigning each task to the CPU core and will be using a semaphore for some of the tasks!


| Tasks      | Core Assigned    | Execution Time|
| :---       | :---      | :--- |
| Task 1 | Core 0 | 2.5 seconds |
| Task 2 | Core 1 | 1 second |
| Task 3 | Core 0 | 0.5 second or 500 msec|
| Task 4 | Core 1 | 0.5 second or 500 msec |
| Task 5 | Core 0 | 2 seconds |

## **ESP32 Pinout**
~~~
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
~~~

# **Pre-lab**
Analyze the example code and demonstrate some understanding from the sample code below.

~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
/*
    Code was made by Christopher A. Mendoza
    UTEP 2022
    last edited: 11/3/2022
*/

/*Task Handlers*/
TaskHandle_t Task1;
TaskHandle_t Task2;

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
   
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
    }
}
~~~

# **Lab Template**
~~~c
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
~~~

# Author 
* Christopher Mendoza
    * **Bachelor of Science in Electrical Engineering**