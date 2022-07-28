# Lab 5 FreeRTOS: Queue

## Objective

* Understand how to use the queues with [`FreeRTOS`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#queue-api). In this lab, the main focus will be using queues to send data among tasks. Students must create queue to send and receive data between two tasks. Task 1 must send an integer i.e (`10`) and then modified to use a string i.e (`"Fall 2022"`). Task 2 should receive the data and print it.

<center>
| Task          |  Description                     |
| :---          | :---                             |
| **Task 1**    | Send data to **queue**           |
| **Task 2**    | Receive **queue** data and print |
</center>

## Bonus
- ***Undergrad Bonus:***
  *  Modify the code to use the structure down below on a queue.
    ~~~c
    typedef struct {
        char str[10];
        int32_t val;
    }Undergaduate_t;
    ~~~

- ***Grad Bonus:***
  *  Modify the code to use the structure down below on a queue. Also, add an LED to change states based on `isLedOn`.
    ~~~c
    typedef struct {
        char str[10];
        int32_t val;
        char studentName[15];
        uint32_t studentID;
        bool isLedOn;
    }Graduate_t;
    ~~~

## ESP32 Pinout
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

## Example 1

The following example is a quick demostration of how to use queues with FreeRTOS on the ESP32. First, a queue is created as a global variable to have name `myQueue`. Next, that variable must store an instance of a queue by `xQueueCreate` function. `Task 1` will send `data` which store a `10` to `Task 2` every second. `Task 2` will recieve data through the queue and display it in the terminal. 
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/* Global Queue handle */
QueueHandle_t myQueue;

/* Example task 1 */
void exampleTask1(void *pvParameter){
    /* data to be sent */
    int data = 10;
    while(1){
        /* Send data to back of the queue */
        xQueueSendToBack(myQueue, &data, 0);
        /* 1 second delay */
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

/* Example task 2 */
void exampleTask2(void *pvParameter){
    /* variable to store data */
    int storeData;

    while(1){
        /* Wait 100 ticks to receive queue, store in storeData */
        if(xQueueReceive(myQueue, &storeData, (TickType_t)100) == pdPASS){
            printf("Data receive from task 1: %d\n", storeData); /* display data recieve */
            vTaskDelay(100/portTICK_PERIOD_MS); /* 100 ms */ 
        }
        vTaskDelay(500/portTICK_PERIOD_MS); /* 500 ms */
    }
}

void app_main(void){
    /* Create a queue of size 5 */
    myQueue = xQueueCreate(5, sizeof(int));
    
    /* Create tasks */
    xTaskCreate(&exampleTask1, "example task 1", 2048, NULL, 4, NULL);
    xTaskCreate(&exampleTask2, "example task 2", 2048, NULL, 4, NULL);
    
}
~~~

## Example 2

The following example shows how to pass argument through a task. As mention in previous labs, `xTaskCreate` has various parameters that we went briefly in detail. However, the 4th argument is use to pass an argument to the task. In the bottom code, `Task 1` recieves an argument, this example may come handy when developing more advance task to reduce and create more abstract tasks. 
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int number = 69; /* random number */

/* Task 1 */
void task1(void *pvParameters){
    int data = (int)pvParameters; /* type cast data */
    while(1){
        printf("Data recieved: %d\n",data); /* print message */
        vTaskDelay(2000 / portTICK_PERIOD_MS); /* 2 second delay */
    }
}
void app_main(void){

    /* Create task */
    xTaskCreate(&task1, "Task 1", 2048, (void*)number, 4, NULL);
    
}
~~~

## Example 3
The following code is similar to the previous example however the major difference is using `global` and `static` variables. The `static` keyword states that the variable that we create will store its data in the static memory. 
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Task 1 */
void task1(void *pvParameters){
    int data = (int)pvParameters; /* type cast data */
    while(1){
        printf("Data recieved: %d\n",data); /* print message */
        vTaskDelay(2000 / portTICK_PERIOD_MS); /* 2 second delay */
    }
}
void app_main(void){

    /* Create a static variable if have value store in static memory */
    static int number = 420; /* random number */

    /* Create task */
    xTaskCreate(&task1, "Task 1", 2048, (void*)number, 4, NULL);

}
~~~

## Lab Template
~~~c
#include <stdio.h>
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

/* Task 1 */
void task1(void *pvParameters){
    int32_t data; /* store data */
    while(1){
        /* Send data from queue */
    }
}

/* Task 2 */
void task2(void *pvParameters){
    int32_t dataReceive; /* store queue data */

    while(1){
        /* Wait to receive from queue */
    }
}

void app_main(void){
    /*
        Create data to be sent: global or static variables
    */

    /* Create Queue of type int32_t */
    xQueue = xQueueCreate(5, sizeof(int32_t));
   
    /* Check if the queue was create successfully */
    if(xQueue != NULL){
        /* Create sender tasks and send int32_t as arguments */

        /* Create reciever task */

    }
}
~~~

## C helpful functions

For this lab, there are few important function calls that are going to be used throughout the lab. Queues are created by using `QueueHandle_t` data type. For instance, **`QueueHandle_t`** *myQueue* is how you would create a queue.

First, `xQueueCreate` is the function you must call if you want to create a queue. `xQueueCreate` has two parameters, `UBaseType_t` uxQueueLength, and `UBaseType_t` uxItemSize. Therefore, if we wanted to create a queue of `float`'s we would do the following:
~~~c
QueueHandle_t myQueue; /* global queue */
...
myQueue = xQueueCreate(10, sizeof(float)); /* create a queue of float with a max size of 5 */
~~~
~~~c
QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );
~~~

Next, `xQueueSendToBack` and `xQueueSendToFront` are similar function with the slight difference of how they send the data to the queue. As the name suggests, `xQueueSendToBack`, sends the data to the back of the queue. Both function have three parameters, `QueueHandle_t` xQueue, `const void *` pvItemToQueue, `TickType_t` xTicksToWait.
~~~c
 BaseType_t xQueueSendToBack(
                                QueueHandle_t xQueue,
                                const void * pvItemToQueue,
                                TickType_t xTicksToWait
                            );
~~~
~~~c
 BaseType_t xQueueSendToFront(
                                QueueHandle_t xQueue,
                                const void * pvItemToQueue,
                                TickType_t xTicksToWait
                             );
~~~
Lastly, `xQueueReceive` is the opposite of `xQueueSendToBack` and `xQueueSendToFront` in the sense that it will store the data in `void * const` pvBuffer. 
~~~c
BaseType_t xQueueReceive(
                            QueueHandle_t xQueue, 
                            void *const pvBuffer, 
                            TickType_t xTicksToWait
                        );
~~~

## Additional Links
* [FreeRTOS Queue](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#queue-api)
* [FreeRTOS Espressif Documenation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
* [FreeRTOS Documenation](https://www.freertos.org/a00125.html)


## Authors
* [***Jesus Minjares***](https://github.com/jminjares4)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesusminjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)
* [***Erick Baca***](https://github.com/eabaca2419)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/erick-baca/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/eabaca2419)

## GitHub
<div align='left'>
 <a href="https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_5">
 <img src="github.png">
 </a>
[Lab 5 Repository](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_5)
</div>

<span class="next_section_button">
Read Next: [Lab 6](@ref doc_pages/lab6.md)
</span>