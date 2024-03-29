<div align='center'>
  <a href="https://www.utep.edu/engineering/ece/">
    <img src="../images/university_of_texas_at_el_paso_logo.png" height="200">
    <h1>
      Electrical and Computer Engineering Department
    </h1>
  </a>
</div>

# **Lab 4 FreeRTOS: Semaphores :zap:**

## **Objective**
* Understand how to use the semaphore with [`FreeRTOS`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html). In this lab, the main focus will be using semaphores to sycnrhonize various tasks. Students must create four different tasks. Task 1 should run every two seconds. Task 2 should run twice as fast as task 1. Task 3 should run every time either task 1 or task 2 run. Lastly, task 4 should run every time task 3 runs. Each task must display *"Task X running"*. 


| Task          | Priority      | Description                           |
| :---          | :---          | :---                                  | 
| **Task 1**    | *High*        | Run every 2 seconds                   | 
| **Task 2**    | *High*        | Run twice as fast as Task 1           |     
| **Task 3**    | *Low*         | Run everytime either Task 1 or Task 2 |    
| **Task 4**    | *Low*         | Run everytime Task 3 runs             |   

| Task Note                 |
| :---:                     |
|Low priority **numbers** denote **low** priority tasks. |  

## **Bonus**
- ***Undergrad Bonus:***
  * Turn an led every time `Task 3` or `Task 4` runs.
- ***Grad Bonus:***
  * Create two indepenent tasks: `Task 5` and `Task 6`. `Task 5` will send a semaphore to `Task 6` which turns on the `onboard led`. 

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

## **Example 1**
The following example is a brief demostration of how to use a semaphore. `Task 1` will run every 2 second and give a semaphore for `Task 2` to run. `Task 2` will consistently wait to receive the semaphore which `Task 1` will provide every 2 seconds. Once `Task 2` receives the semaphore from `Task 1` it will run and print `"Task 2 is running!!"`.

~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/* Create global instance semaphore */
SemaphoreHandle_t mySemaphore = NULL;

/* Example task 1 */
void exampleTask1(void *pvParameters){
    while(1){
        /* Give semaphor to run Task 2*/
        xSemaphoreGive(mySemaphore); 
        vTaskDelay(2000 / portTICK_PERIOD_MS); /* 2 second delay */
        printf("Task 1 running!\n");
    }
}

/* Example task 2 */
void exampleTask2(void *pvParameters){
    while(1){
         /* wait for 1000 ticks to receive semaphore */
         if(xSemaphoreTake(mySemaphore, 1000/portTICK_RATE_MS) == pdTRUE){ 
             printf("Task 2 is running!!\n");
             vTaskDelay(100/portTICK_RATE_MS); /* 100 ms delay */
         }else{
             vTaskDelay(100/portTICK_RATE_MS); /* 100 ms delay */
         }
    }
}

void app_main(void){

    /* Create binary semaphore */
    mySemaphore = xSemaphoreCreateBinary(); 

    /* Create tasks */
    xTaskCreate(&exampleTask1, "example task 1", 2048, NULL, 4, NULL); 
    xTaskCreate(&exampleTask2, "example task 2", 2048, NULL, 4, NULL);

}
~~~

## **Example 2**
The following example is a more advance demostration of how to use semaphores. `Task 1` will wait for `mySemaphore1` and once it receives it, it will give `mySeamphore2`. `Task 2` will receive the semaphore from `Task 1` and run. Once `Task 2` task is done, `mySemaphore1` will be given back to `Task 1` and run again. In simple terms, task 1 will run task 2 and task 2 will run task 1. Therefore, task 1 and task 2 synchronize which demostrates the true purpose of semaphores.
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

/* Create global instances of sempaphore */
SemaphoreHandle_t mySemaphore1 = NULL; 
SemaphoreHandle_t mySemaphore2 = NULL;

/* Example task 1 */
void exampleTask1(void *pvParameters){
    while(1){
        /* Check if semaphore was received every 100 ticks */
        if(xSemaphoreTake(mySemaphore1, (TickType_t)100 ) == pdTRUE){ /
              printf("Task 1 sent flag to task 2!\n");
              vTaskDelay(2000 / portTICK_PERIOD_MS); // 2 second delay
              xSemaphoreGive(mySemaphore2); /* give semaphore to run task 2 */
        }else{
              vTaskDelay(100 / portTICK_PERIOD_MS); /* 100 ms delay */
        }
    }
}

/* Example task 2 */
void exampleTask2(void *pvParameters){
    while(1){
         /* wait for atleast 100 ms to receive semaphore */
         if(xSemaphoreTake(mySemaphore2, 100/portTICK_PERIOD_MS) == pdTRUE){ 
             printf("Task 2 is running!!!!\n");
             vTaskDelay(100/portTICK_PERIOD_MS); /* 100 ms */
             xSemaphoreGive(mySemaphore1); /* give back semaphore to task 1 */
         }else{
             vTaskDelay(100/portTICK_PERIOD_MS); /* 100 ms delay */
         }
    }
}

void app_main(void){

    /* Create two semaphores binary semaphores */
    mySemaphore1 = xSemaphoreCreateBinary(); 
    mySemaphore2 = xSemaphoreCreateBinary(); 

    /* Create tasks */
    xTaskCreate(&exampleTask1, "example task 1", 2048, NULL, 4, NULL); 
    xTaskCreate(&exampleTask2, "example task 2", 2048, NULL, 4, NULL); 
    
    /* Give the semaphore for task 1 to run */
    xSemaphoreGive(mySemaphore1); 
}
~~~

## **Lab Template**
~~~c
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
~~~

## **C helpful functions**

For this lab, there are few important function calls that are going to be used throughout the lab. Semaphores are created by using `SemaphoreHandle_t` data type. For instance, `SemaphoreHandle_t` *mySemaphore* is how you would create a semaphore.

To be able to allocate an instance of a semaphore, you will need the following function: `xSemaphoreCreateBinary`. There are different types of semaphores, however, we will only look at the binary semaphore for now. A binary semaphore has a behavior that holds a value of `0` or `1`.
~~~c
SemaphoreHandle_t xSemaphoreCreateBinary( void );
~~~
Next, `xSemaphoreGive` will set a `0` in the semaphore. Therefore, by setting a `0` it will state that the semaphore is ready to be used.
~~~c
void xSemaphoreGive( SemaphoreHandle_t xSemaphore );
~~~

Lastly, `xSemaphoreTake` is the opposite from the previous function, as it will set a `1` and used the semaphore. However, `xSemaphoreTake` will require two parameters `SemaphoreHandle_t xSemaphore` and `TickType_t xTicksToWait`. You must pass by value the instance of a semaphore and how long to wait for the semaphore. If you receive the semaphore during the provided time frame, `xSemaphoreTake` will return `pdTRUE` otherwise `pdFALSE`.
~~~c
 xSemaphoreTake( SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait );
~~~

## **Additional Links**
* [FreeRTOS Semphores](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
* [FreeRTOS Espressif Documenation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
* [FreeRTOS Documenation](https://www.freertos.org/a00125.html)

## **Authors**
* [**Jesus Minjares**](https://github.com/jminjares4)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesusminjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)
* [**Erick Baca**](https://github.com/eabaca2419)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/erick-baca/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/eabaca2419)


