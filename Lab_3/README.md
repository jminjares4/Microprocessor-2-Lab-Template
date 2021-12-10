# **`LAB 3 Introduction to FreeRTOS` :zap:**

## **Objective:**
---
* Understand how to use the FreeRTOS with [`Espressif`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#).
  
    **Tasks**
    * Task 1 -> toggle `onboard led`
    * Task 2 -> print `hello world` every second
- ***Undergrad Bonus:***
  * Create an other task that will toggle the led and print it state in the terminal
- ***Grad Bonus:***
  * Create addtional task of to run any led sequence of the previous labs and it should run every 5 seconds
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

### **Example**
Here is a simple example of creating a task with FreeRTOS. The task is called `task1` which is it own function. `Task 1` will turn the `onboard led` for 2 seconds and turn off for another 2 seconds.
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define ONBOARD_LED 2
#define HIGH 1
#define LOW  0

void task1(void *pvParameter){
    /* Select the GPIO pin that is going to be used */
    gpio_pad_select(ONBOARD_LED);
    /* Set the direction of the GPIO pin as OUTPUT */
    gpio_set_direction(ONBOARD_LED, GPIO_MODE_OUTPUT);

    while(1){
        /*Set the ONBOARD_LED as HIGH | 1 */
        gpio_set_level(ONBOARD_LED, HIGH);
        vTaskDelay(2000 / portTICK_RATE_MS); // 2 second delay
        /*Set the ONBOARD_LED as LOW | 0 */
        gpio_set_level(ONBOARD_LED, LOW);
        vTaskDelay(2000 / portTICK_RATE_MS); // 2 second delay
    }
}
void app_main(void){
    /* Create task 1 */
    xTaskCreate(&task1, "task1", 2048, NULL, 5, NULL);
}
~~~

### **Template Code**
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
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
~~~

## **C helpful functions**

For this lab, the most important function call is `xTaskCreate` which is a function in `task.h` file. This function has a few parameter that must be pass on when calling the function. For instance we want to create a simple that call `example_task` which has a stack of `2048` and it will have no arguments or handle; it will be the following:
*`xTaskCreate(`* **&example_task**,**"example task"**, **2048**, **NULL**, **NULL** *`);`*


| Data type         | Variable name | Description |
|--------------|:-----:|-----------:|
| `TaskFunction_t`          |  *pvTaskCode*     | task function name |
| `const char *`            |  *pcName*         | name to associate the task|
| `configSTACK_DEPTH_TYPE`  | *usStackDepth*    |    stack size       |
| `void *`                  |  *pvParameters*   | arguments  |
| `TaskHandle_t *`          |  *pxCreatedTask*  | handle to store task  |


~~~c 
 BaseType_t xTaskCreate( TaskFunction_t pvTaskCode,
                         const char * const pcName,
                         configSTACK_DEPTH_TYPE usStackDepth,
                         void *pvParameters,
                         UBaseType_t uxPriority,
                         TaskHandle_t *pxCreatedTask
                        );
~~~

### **Additional Links**
* [FreeRTOS Espressif Documenation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
* [FreeRTOS Documenation](https://www.freertos.org/a00125.html)
* [Espressif GPIO Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#)
---
### **Author**
* [`Jesus Minjares`](https://github.com/jminjares4)
  * Master of Science in Computer Engineering
* [`Erick Baca`](https://github.com/eabaca2419)
  * Master of Science in Computer Engineering
