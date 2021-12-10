# **LAB 5 FreeRTOS: Queue :zap:**

## **Objective:**

***
* Understand how to use the queues with [`FreeRTOS`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#queue-api). In this lab, the main focus will be using queues to send data among tasks. 

* Create a program which will create a queue to send and receive data.
* Send data to the queue from multiple tasks, sending integers `10` and `20` also  for strings `“EE4178”` and `“Fall2021”`.
* Receive and print data from the queue

### **Bonus**
***
- ***Undergrad Bonus:***
  *  Modify the code by sending structures on a queue.
~~~c
typedef struct {
    char str[10];
    int32_t val;
}Undergaduate_t;
~~~

- ***Grad Bonus:***
  *  Modify the code by sending structures on a queue. If `isLedOn` *true*, turn the `LED` else, turn off.

~~~c
typedef struct {
    char str[10];
    int32_t val;
    char studentName[15];
    uint32_t studentID;
    bool isLedOn;
}Graduate_t;
~~~

### **ESP32 Pinout**
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

### **Example 1**

The following example is a quick demostration of how to use queues with FreeRTOS on the ESP32. First, a queue is create as a global variable to scope accessiblity name `myQueue`. Next, that variable must store an instance of a queue by `xQueueCreate` function. `Task 1` will send `data` which store a `10` to `Task 2` every second. `Task 2` will recieve data through the queue and display it in the terminal. 
~~~c
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

/* Global Queue handle */
QueueHandle_t myQueue;

void exampleTask1(void *pvParameter){
    int data = 10;
    while(1){
        xQueueSendToBack(myQueue, &data, 0);
        vTaskDelay(1000/portTICK_RATE_MS);
    }
}
void exampleTask2(void *pvParameter){
    int storeData;

    while(1){
        if(xQueueReceive(myQueue, &storeData, (TickType_t)100) == pdPASS){
            printf("Data receive from task 1: %d\n", storeData); // display data recieve
            vTaskDelay(100/portTICK_RATE_MS); // 100 ms 
        }
        vTaskDelay(500/portTICK_RATE_MS); // 500 ms 
    }
}

void app_main(void){
    myQueue = xQueueCreate(5, sizeof(int));
    xTaskCreate(&exmapleTask1, "example task 1", 2048, NULL, 4, NULL);
    xTaskCreate(&exampleTask2, "example task 2), 2048, NULL, 4, NULL);
}
~~~

### **Example 2**

The following example is an demostration of how to pass argument through a task. As mention in previous labs, `xTaskCreate` has the various parameters that we went briefly in detail. The 4th argument is the argument that is pass to the task. In the bottom code, `Task 1` is recieving the data store in `number` when it being created in main. 
~~~c
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

int number = 69; // make it fun

void task1(void *pvParameters){
    int data = (int)pvParameters; //type cast data
    while(1){
        printf("Data recieved: %d\n",data); //print message 
        vTaskDelay(2000/portTICK_RATE_MS); // 2 second delay
    }
}
void app_main(void){
    xTaskCreate(&task1, "Task 1", 2048, (void*)number, 4, NULL);
}
~~~

### **Example 3**
The following code is similar to the previous example however the major difference is using `global` and `static` variables.`static` keyword states that the variable that we create will store its data in the static memory.
~~~c
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void task1(void *pvParameters){
    int data = (int)pvParameters; //type cast data
    while(1){
        printf("Data recieved: %d\n",data); //print message 
        vTaskDelay(2000/portTICK_RATE_MS); // 2 second delay
    }
}
void app_main(void){
    static int number = 420; // make it fun
    xTaskCreate(&task1, "Task 1", 2048, (void*)number, 4, NULL);
}
~~~


### **Template Code**

~~~c
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
~~~

## **C helpful functions**

For this lab, there are few important function calls that are going to be used throughout the lab. Queues are created by using `QueueHandle_t` data type. For instance, **`QueueHandle_t`** *myQueue* is how you would create a queue.

First, `xQueueCreate` is the function you must call if you want to create a queue. `xQueueCreate` has two parameters, `UBaseType_t` uxQueueLength, and `UBaseType_t` uxItemSize. Therefore, if we wanted to create a queue of `float`'s we would do the following:
~~~c
QueueHandle_t myQueue; //global queue
...
myQueue = xQueueCreate(10, sizeof(float)); //create a queue of float with a max size of 5
~~~

~~~c
QueueHandle_t xQueueCreate( UBaseType_t uxQueueLength, UBaseType_t uxItemSize );
~~~

Next, `xQueueSendToBack` and `xQueueSendToFront` are similar function with the slight differnet of where they send the data to the queue. As the name suggests, `xQueueSendToBack`, sends the data to the back of the queue. Both function have three parameters, `QueueHandle_t` xQueue, `const void *` pvItemToQueue, `TickType_t` xTicksToWait.
Therefore, if want to send something through our queue we would set the queue in which the data will be sent, pass by reference the data, and how long we wait to the send the data.
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
BaseType_t xQueueReceive(QueueHandle_txQueue, void *const pvBuffer, TickType_t xTicksToWait)
~~~

### **Additional Links**
* [FreeRTOS Queue](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#queue-api)
* [FreeRTOS Espressif Documenation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
* [FreeRTOS Documenation](https://www.freertos.org/a00125.html)
---
### **Author**
* [`Jesus Minjares`](https://github.com/jminjares4)
  * Master of Science in Computer Engineering
* [`Erick Baca`](https://github.com/eabaca2419)
  * Master of Science in Computer Engineering
