# Lab 0 LED Lightshow

## Objective
* Understand how to use the gpio driver library from [`Espressif`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#). The lab will consist of creating a `sweeper` and a `led chaser`. The `sweeper` will iterate over mutiple LEDs by turning from the lowest to highest bit and then most significant bit to lowest bit. While `led chaser` will have a single led iterating from the lowest to most significant bit. For both the `sweeper` and `led chaser` use up to 6 GPIOs.

## Bonus
- ***Undergrad Bonus:***
  * Do the `led chaser`
- ***Grad Bonus:***
  * Create a light show with different sequences and patterns as you like. Have fun!

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
Here is an example of a how to use ESP32 GPIO function calls. The program toggles ESP onboard LED every other second.
~~~c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LOW  0  /* LOGIC LOW*/
#define HIGH 1  /* LOGIC HIGH*/
#define ONBOARD_LED 2 /* ONBOARD LED GPIO Pin*/

void app_main(void){
  esp_rom_gpio_pad_select_gpio(ONBOARD_LED); // select the GPIO pins
  gpio_set_direction(ONBOARD_LED, GPIO_MODE_OUTPUT); // set as output

  while(1){
    gpio_set_level(ONBOARD_LED, HIGH); // set high
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    gpio_set_level(ONBOARD_LED, LOW); //set low
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
  }  
}
~~~

## Lab Template
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LOW  0  /* LOGIC LOW*/
#define HIGH 1  /* LOGIC HIGH*/

/**
 * @brief setOutputs will initialize uint8_t array as outputs
 *
 * @param out uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None
 * @note initialize gpio before using them
 */
void setOutputs(uint8_t *out, int size)
{
    for (int i = 0; i < size; i++) // itierate over the size of the array
    {
        esp_rom_gpio_pad_select_gpio(out[i]);                 // select the GPIO pins
        gpio_set_direction(out[i], GPIO_MODE_OUTPUT); // set direction as outputs
    }
    return;
}

/**
 * @brief sweep function will sweep among the GPIOs 
 * 
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None
 */
void sweep(uint8_t *led, int size)
{
    for ( int i = 0; i < size; i++) //iterate over the size of the array
    {
        
    }
    for (int i = size - 1; i >= 0; i--) //iterate over the size of hte array 
    {
      
    }
}

/**
 * @brief led_chaser led chaser will make a single led to iterate over the array 
 * 
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None
 */
void led_chaser(uint8_t *led, int size)
{
    for (int i = 0; i < size; i++) //iterate over the size of the array
    {
      
    }
    for (int i = size - 1; i >= 0; i--) //iterate over the size of hte array 
    {
      
    }
}

/**
 * @brief lightShow user define light show 
 * 
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None
 */
void lightShow(uint8_t *led, int size)
{
    /*Use all prior functions to create a custom light show */
}

void app_main(void)
{
    /*GPIOs pins*/
    uint8_t led[] = {2}; //create an array of single led 
    int size = sizeof(led)/sizeof(uint8_t); //get size of the array, use sizeof() to allow scalabilty 
    setOutputs(led,size); //intialize GPIOs pins
    while (1)
    {
      // sweep(led, size); //sweep function
      //led_chaser(led,size);
      //lightShow(led,size); //bonus 

      // Toggle LED using index of your array
      gpio_set_level(led[0], LOW);
      vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
      gpio_set_level(led[0], HIGH);
      vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    }
}
~~~

## C helpful functions
For this lab, there are three main function from Espressif that are important. First, is to select which GPIO pin is going to be used by the following function: `esp_rom_gpio_pad_select_gpio(gpio_num_t gpio_num)`. The first parameter `gpio_num` is the GPIO pin that is going to be used, for instance if we want to use the ***onboard led*** we should put a 2 on `gpio_num` such as `esp_rom_gpio_pad_select_gpio(2)`.
~~~c
esp_err_t esp_rom_gpio_pad_select_gpio(gpio_num_t gpio_num);             
~~~
Next, `gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode)` allow the user to set the direction of the GPIO pin. For this lab we only going to use <i>`GPIO_MODE_OUTPUT`</i> for the mode.
~~~c 
esp_err_t gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode);
~~~
Furthermore, `gpio_set_level(gpio_num_t gpio_num, uint32_t level)` sets the logic level of the GPIO pin that we pass throught the function. <br>
| Value | Logic |
|:---   | :---  |
| **0** | low   |
| **1** | high  |
~~~c
esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level);
~~~

Lastly, the following function  `vTaskDelay( const TickType_t xTicksToDelay)` is use to generate delay with ESP32 and it part of FreeRTOS, you will learn more of it in later labs. Therefore for this lab just use it to generate delay in milliseconds
~~~c
void vTaskDelay(const TickType_t xTicksToDelay);
~~~

## Warning!
Depending of the version of **ESP-IDF** that you may have, some libraries and function calls that may be **deprecate**! If you get an warning message when using `esp_rom_gpio_pad_select_gpio` please use `gpio_pad_select_gpio`. 

## Additional Links
* [Espressif GPIO Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#)

## Authors
* [***Jesus Minjares***](https://github.com/jminjares4)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesusminjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)
* [***Erick Baca***](https://github.com/eabaca2419)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/erick-baca/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/eabaca2419)

## GitHub
<div align='left'>
 <a href="https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_0">
 <img src="github.png">
 </a>
[Lab 0 Repository](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_0)
</div>

<span class="next_section_button">
Read Next: [Lab 1](@ref doc_pages/lab1.md)
</span>