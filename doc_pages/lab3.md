# Lab 3 Introduction to FreeRTOS

## Objective
* Understand how to use the FreeRTOS with [`Espressif`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#). The lab will consist of creating two main tasks. `Task 1` must toggle *onboard led* every **250 ms**, and `Task 2` must print <strong>'Hello World!'</strong> every **2 seconds**.

<div align='center'>
| Tasks      | Objective                     |
| :---       | :---                          |
| Task 1     | Toggle `onboard led` @ 250 ms |
| Task 2     | Print `Hello World!` @ 2 sec  |
</div>

## Bonus 
- ***Undergrad Bonus:***
  * Create an `additional task` that toggles an external **led** and print it **states**
- ***Grad Bonus:***
  * Create an `addtional task` that runs any *led sequence* from the previous labs and it should run every **5 seconds**

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

## Example
Here is a simple example of creating a task with FreeRTOS. The task is called `task1` which is it own function. `Task 1` will turn the `onboard led` for 2 seconds and turn off for another 2 seconds.
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LOW  0  /* LOGIC LOW*/
#define HIGH 1  /* LOGIC HIGH*/
#define ONBOARD_LED 2 /* ONBOARD LED GPIO Pin*/

void task1(void *pvParameters){
    /* Select the GPIO pin that is going to be used */
    esp_rom_gpio_pad_select_gpio(ONBOARD_LED);
    /* Set the direction of the GPIO pin as OUTPUT */
    gpio_set_direction(ONBOARD_LED, GPIO_MODE_OUTPUT);

    while(1){
        /*Set the ONBOARD_LED as HIGH | 1 */
        gpio_set_level(ONBOARD_LED, HIGH);
        vTaskDelay(2000 / portTICK_PERIOD_MS); // 2 second delay
        /*Set the ONBOARD_LED as LOW | 0 */
        gpio_set_level(ONBOARD_LED, LOW);
        vTaskDelay(2000 / portTICK_PERIOD_MS); // 2 second delay
    }
}
void app_main(void){
    /* Create task 1 */
    xTaskCreate(&task1, "task1", 2048, NULL, 5, NULL);
}
~~~

## Lab Template
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

/* Define pin 13 as a “BLINK_GPIO” */
#define BLINK_GPIO 13

/* Code for the hello_task task */
void hello_task(void *pvParameters)
{
    
}

/* Code for the blinky task */
void blinky_task(void *pvParameters)
{
    
}

void led_sequence_task(void *pvParameters){

}

void app_main()
{
    /* Create the task defined by xTaskCreate.*/
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);
    xTaskCreate(&blinky_task, "blinky_task", 512, NULL, 5, NULL);

    /*Create any additional tasks*/
    
}
~~~

## C helpful functions

For this lab, the most important function call is `xTaskCreate` which is a function in `task.h` file. This function has few parameters that must be pass on when calling the function. For instance, if we want to create a simple task that calls `example_task` which has a stack of `2048`, a priority of 5 and it will have no arguments or handle; it will be the following:
`xTaskCreate(` **&example_task**, **example task**, **2048**, **5**, **NULL**, **NULL** `);`

<div align='center'>
| Data type                 | Variable name     | Description                 |
| :---                      |      :---         | ---:                        |
| `TaskFunction_t`          |  *pvTaskCode*     | task function name          |
| `const char *`            |  *pcName*         | name to associate the task  |
| `configSTACK_DEPTH_TYPE`  |  *usStackDepth*   | stack size                  |
| `UBaseType_t`             |  *uxPriority*     | task priority               |
| `void *`                  |  *pvParameters*   | arguments                   |
| `TaskHandle_t *`          |  *pxCreatedTask*  | handle to store task        |
</div>

~~~c 
 BaseType_t xTaskCreate( 
                          TaskFunction_t pvTaskCode,
                          const char * const pcName,
                          configSTACK_DEPTH_TYPE usStackDepth,
                          void *pvParameters,
                          UBaseType_t uxPriority,
                          TaskHandle_t *pxCreatedTask
                       );
~~~

## Additional Links
* [FreeRTOS Espressif Documenation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html)
* [FreeRTOS Documenation](https://www.freertos.org/a00125.html)
* [Espressif GPIO Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#)

## Authors
* [**Jesus Minjares**](https://github.com/jminjares4)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesusminjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)
* [**Erick Baca**](https://github.com/eabaca2419)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/erick-baca/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/eabaca2419)

## GitHub
<div align='left'>
 <a href="https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_3">
 <img src="github.png">
 </a>
[Lab 3 Repository](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_3)
</div>

<span class="next_section_button">
Read Next: [Lab 4](@ref doc_pages/lab4.md)
</span>