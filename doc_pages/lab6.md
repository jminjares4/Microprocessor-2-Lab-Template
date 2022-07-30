# Lab 6 GPIO, Interrupts, and Queues

## Objective

* Understand how to use queues and interrupts with [`FreeRTOS Queues`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#queue-api). In this lab, students will learn how to send qyedata between tasks using interrupts. Student must create a program that uses `gpio` interrupts as in Lab 2, please feel free to use either configuration (Pull-up | Pull-down). The interrupt service routine (ISR) should send data (gpio pin number) to a queue and receive by task: `print_task`. The `print_task` must receive the data and display it. For instance, if the interrupt was trigger by pin **23**, the output would be: "GPIO 23 received!!!". 

<div align='center'>
| Task          |    Objective                               |
| :---          | :---                                       |
| print_task    | Receive data and print to terminal/monitor |
<br>
| Interrupt     | Objective                 |
| :---          | :---                      |
| gpio_isr      | Send data to `print_task` |
</div>
## Bonus
 - ***Undergrad Bonus***
    * Modify code to use two more interrupts. 
 - ***Grad Bonus***
    * Modify code to use two more interrupts and add two LEDs. Each interrupt must toggle its corresponding LED. 

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
Here is an example of a how to use ESP32 GPIO advance setup. The following code will toggle the onboard LED if button is pressed and have a external LED toggling every half second.
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h" 
#include "freertos/task.h" 
#include "driver/gpio.h"

#define ESP_INTR_FLAG_DEFAULT 0 /* Interrupt flag configuration */

#define LOW     0   /* low logic  */
#define HIGH    1   /* high logic */

#define ONBOARD_LED     2    /* onboard led pin  */
#define EXTERNAL_LED    22   /* external led pin  */
#define BUTTON          23   /* button pin  */

/**
 * @brief Toggle gpio pin
 * 
 * @param pin gpio pin
 * @return None
 */
void toggle(gpio_num_t pin){
    /* Get current level and toggle LED */
    gpio_get_level(pin) ? gpio_set_level(pin, LOW) : gpio_set_level(pin, HIGH);
}

/* GPIO interrupt handler */
static void IRAM_ATTR gpio_isr_handler(void* arg) {
    /* store argument */
    gpio_num_t gpio = (gpio_num_t) arg;

    /* toggle LED */
    toggle(gpio);

}

/* GPIO setup */
void gpio_setup(void){
    /* IO configuration */
    gpio_config_t io_conf;
    
    /* Input configuration */
    io_conf.intr_type = GPIO_INTR_POSEDGE;  /* Set up as Positive Edge */ 
    io_conf.mode = GPIO_MODE_INPUT;     /* Set pins as input */
    io_conf.pin_bit_mask = (1ULL << BUTTON);  /* Add input bit mask */
    io_conf.pull_down_en = 1;   /* Enable pulldown */
    io_conf.pull_up_en = 0;     /* Disable pullup */

    /* Set configuration */
    gpio_config(&io_conf);

    /* Output configuration */
    io_conf.intr_type = GPIO_INTR_DISABLE;  /* Disable interrupt */
    io_conf.mode = GPIO_MODE_OUTPUT;        /* Set as output*/
    io_conf.pin_bit_mask = (1ULL << ONBOARD_LED) | (1ULL << EXTERNAL_LED); /* Add output bit mask */
    io_conf.pull_down_en = 0;   /* Disable pulldown */
    io_conf.pull_up_en = 0;     /* Disable pullup */

    /* Set configuration */
    gpio_config(&io_conf);

    /* Set default interrupt flag */
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    /* Add ISR handler */
    gpio_isr_handler_add(BUTTON, gpio_isr_handler, (void*) BUTTON); 

}
void app_main() {

    /* Setup the GPIOs */
    gpio_setup();

    while(1){
         toggle(EXTERNAL_LED); /* Toggle external led */
         vTaskDelay(500/portTICK_PERIOD_MS); /* .5 second delay */
    }

}
~~~

## Lab Template
~~~c
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
~~~


## C helpful functions

We have use queues in the previous lab to send data from one task to another. However, it this lab we need to send data from an interrupt to a task. Therefore, a special function is need to send data to the queue: <strong>`xQueueSendFromISR()`</strong>. Similar to the previous lab functions that we have use to send data in a queue, `xQueueSendFromISR` is similar with a different parameter. To keep it simple, please set `BaseType_t *pxHigherPriorityTaskWoken` parameter set to <i>`NULL`</i>.
~~~c
 BaseType_t xQueueSendFromISR(
                                QueueHandle_t xQueue,
                                const void *pvItemToQueue,
                                BaseType_t *pxHigher PriorityTaskWoken
                             );
~~~

For more information of how to use [Queues](@ref doc_pages/lab5.md) and [Interrupts](@ref doc_pages/lab2.md), please see previous labs.

## Pull-up and Pull-down Configuration

<img width="413" alt="button configuation" src="button_config.png">

## Additional Links
* [Espressif GPIO Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#)
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
 <a href="https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_6">
 <img src="github.png">
 </a>
[Lab 6 Repository](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_6)
</div>

<span class="next_section_button">
Read Next: [Lab 7](@ref doc_pages/lab7.md)
</span>