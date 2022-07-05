# Lab 4 FreeRTOS: Semaphores

## Objective:
* Understand how to use the semaphore with [`FreeRTOS`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html). In this lab, the main focus will be using semaphores to sycnrhonize various tasks. 
* Student must create four different tasks. Make task one run every two seconds. Task two should run twice as fast as task one. Task three should run every time either task one or task two run. And task four should run every time Task 3 runs.
* Print “Task X running”for every task in the console.

* **Tasks**
  * Task 1 -> run every two seconds
  * Task 2 -> should run twice as fast as task one
  * Task 3 -> run every time either task one or task two run
  * Task 4 -> run every time Task 3 runs
  
- ***Undergrad Bonus:***
  * Turn an led every time `Task 3` or `Task 4` runs.
- ***Grad Bonus:***
  * Create an other two task indepenent from the other tasks: `Task 5` and `Task 6`. `Task 5` will sent a semaphore to `Task 6` which will turn the `onboard led` else turn off the led. 

### ESP32 Pinout
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

### Example 1
The following example is a brief demostration of how to use semaphore. `Task 1` will run every 2 second and give a sempahore for `Task 2` to run. `Task 2` will consistently wait to receive the sempahore which `Task 1` will provide every 2 seconds. Once task 2 recieve the semaphore from task 1 it will run and print `"Task 2 is running!!"`.
~~~c
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t mySemaphore = NULL; //create a global instance of an semaphore

void exampleTask1(void *pvParameter){
    while(1){
        xSemaphoreGive(mySemaphore); //give semaphore to run task 2
        vTaskDelay(2000 / portTICK_RATE_MS); // 2 second delay
        printf("Task 1 running!\n");
    }
}
void exmapleTask2(void *pvParameter){
    while(1){
         //wait for atleast 1 second
         if(xSemaphoreTake(mySemaphore, 1000/portTICK_RATE_MS) == pdTRUE){ 
             printf("Task 2 is running!!\n");
             vTaskDelay(100/portTICK_RATE_MS); // 100 ms 
         }else{
             vTaskDelay(100/portTICK_RATE_MS); // 100 ms delay
         }
    }
}

void app_main(void){
    mySemaphore = xSemaphoreCreateBinary(); //create semaphore
    xTaskCreate(&exampleTask1, "example task 1", 2048, NULL, 4, NULL); //create task
    xTaskCreate(&exampleTask2, "example task 2", 2048, NULL, 4, NULL); //create task
}
~~~


### Example 2
The following example is a more advance demostration of how to use semaphore. `Task 1` will wait for `mySemaphore1` and it recieves it; it will give `mySeamphore2`. `Task 2` will recieve the semaphore from `Task 1` and run. Once `Task 2` task is done, `mySemaphore1` will be give back for `Task 1` to run again. In simple terms, task 1 will run task 2 and task 2 will run task 1. Therefore, task 1 and task 2 synchronize which demostrate the true purpose of semaphores.
~~~c
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t mySemaphore1 = NULL; //create a global instance of an semaphore
SemaphoreHandle_t mySemaphore2 = NULL; //create a global instance of an semaphore

void exampleTask1(void *pvParameter){
    while(1){
        if(xSemaphoreTake(mySemaphore1, (TickType_t)100 ) == pdTRUE){ //TickType_t typecast
              printf("Task 1 sent flag to task 2!\n");
              vTaskDelay(2000 / portTICK_RATE_MS); // 2 second delay
              xSemaphoreGive(mySemaphore2`); //give semaphore to run task 2
        }else{
              vTaskDelay(100 / portTICK_RATE_MS); // 100 ms delay
        }
       
    }
}

void exmapleTask2(void *pvParameter){
    while(1){
         //wait for atleast 100 second
         if(xSemaphoreTake(mySemaphore2, 100/portTICK_RATE_MS) == pdTRUE){ 
             printf("Task 2 is running!!!!\n");
             vTaskDelay(100/portTICK_RATE_MS); // 100 ms 
             xSemaphoreGive(mySemaphore1); //give back semaphore to task 1
         }else{
             vTaskDelay(100/portTICK_RATE_MS); // 100 ms delay
         }
    }
}

void app_main(void){
    //Create two semaphores
    mySemaphore1 = xSemaphoreCreateBinary(); //create semaphore
    mySemaphore2 = xSemaphoreCreateBinary(); //create semaphore

    //Create two task 
    xTaskCreate(&exampleTask1, "example task 1", 2048, NULL, 4, NULL); //create task
    xTaskCreate(&exampleTask2, "example task 2", 2048, NULL, 4, NULL); //create task
    
    // give the semaphore for task 1 to run
    xSemaphoreGive(mySemaphore1); 
}
~~~

### Template Code
~~~c
/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering
    Course:     EE 5190 Laboratory for Microprocessors Systems II   

    Lab 4:
            Understand how to use the semaphore with FreeRTOS. In this lab,
            the main focus will be using semaphores to sycnrhonize various tasks. 
                
            * Student must create four different tasks. Make task one run every two seconds. Task two should run twice as fast as task one. Task three should run every time either task one or task two run. And task four should run every time Task 3 runs.
            * Print “Task X running”for every task in the console.

            Tasks
                * Task 1 -> run every two seconds
                * Task 2 -> should run twice as fast as task one
                * Task 3 -> run every time either task one or task two run
                * Task 4 -> run every time Task 3 runs
  
            Bonus 
            - Undergrad Bonus:
                * Turn an led every time `Task 3` or `Task 4` runs.
            - Grad Bonus:
                * Create an other two task indepenent from the other tasks: `Task 5` and `Task 6`. 
                * `Task 5` will sent a semaphore to `Task 6` which will turn the `onboard led` else turn off the led. 
*/
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
SemaphoreHandle_t xSemaphore = NULL;
void task1(void *pvParameter)
{
    while (1)
    {
        printf("Task 1\n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void task2(void *pvParameter)
{
    while (1)
    {
        printf("Task 2\n");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}

void app_main()
{
    xSemaphore = xSemaphoreCreateBinary();
    xTaskCreate(&task1, "task1", 4096, NULL, 5, NULL);
    xTaskCreate(&task2, "task2", 4096, NULL, 5, NULL);
    xTaskCreate(&task3, "task3", 4096, NULL, 6, NULL);
    xTaskCreate(&task4, "task4", 4096, NULL, 7, NULL);
}

~~~

## C helpful functions
For this lab, there are few important function calls that are going to be used throughout the lab. Semaphores are created by using `SemaphoreHandle_t` data type. For instance, <strong>`SemaphoreHandle_t`</strong> *mySemaphore* is how you would create a semaphore.

To be able to allocate an instance of an semaphore, you will need the following function: `xSemaphoreCreateBinary`. There are different types of semaphores, however we will only look at the binary semahpore. A binary semaphore as the name states it can have a value either `0` or `1`.
~~~c
SemaphoreHandle_t xSemaphoreCreateBinary( void );
~~~
Next, `xSemahporeGive` will set a `0` in the semaphore. Therefore, by setting a `0` it will state that the semaphore is ready to be used.
~~~c
void xSemaphoreGive( SemaphoreHandle_t xSemaphore );
~~~
Lastly, `xSemaphoreTake` is the opposite of the prior function. Therefore, it will set a `1` meaning that the semahpore is being use. However, `xSemaphoreTake` will require two parameters ` SemaphoreHandle_t xSemaphore` and `TickType_t xTicksToWait`. You must pass by value the instance of a semaphore and also how long are you planning to wait for the semaphore. If you receive the semaphore during the time frame `xSemaphoreTake` will return `pdTRUE` else `pdFALSE`.
~~~c
 xSemaphoreTake( SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait );
~~~

### Additional Links
* [FreeRTOS Semphores](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
* [FreeRTOS Espressif Documenation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
* [FreeRTOS Documenation](https://www.freertos.org/a00125.html)

### Authors
* [***Jesus Minjares***](https://github.com/jminjares4)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesusminjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)
* [***Erick Baca***](https://github.com/eabaca2419)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/erick-baca/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/eabaca2419)

@see [GitHub Lab 4](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_4)

