# **Lab 0 LED Lightshow :zap:**
## **Objective:**
---
* Understand how to use the gpio driver library from [`Espressif`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#). The lab will consist of creating a `sweeper` and also `led chaser`. The `sweeper` will iterate over mutiple LEDs by turning from the lowest to highest bit and then turn off the most significant bit to lowest bit. While `led chaser` as the name states will haven a single led iterating from the lowest to most significant bit. For both the `sweeper` and `led chaser` use up to 6 GPIOs.

- ***Undergrad Bonus:***
  * Do the `led chaser`
- ***Grad Bonus:***
  * Create a light show with different sequences and patterns as you like. Have fun!
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
Here is an example of a single GPIO set as ouptut and toggling the onboard led.
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define HIGH 1
#define LOW  0
#define ONBOARD_LED 2

void app_main(void){
  esp_rom_gpio_pad_select_gpio(ONBOARD_LED); // select the GPIO pins
  gpio_set_direction(ONBOARD_LED, GPIO_MODE_OUTPUT); // set as output

  while(1){
    gpio_set_level(ONBOARD_LED, HIGH); // set high
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 seccond delay
    gpio_set_level(ONBOARD_LED, LOW); //set low
    vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 seccond delay
  }  
}
~~~

### **Lab Template**
~~~c
/*
    Author:     Jesus Minjares and Erick A. Baca
                Master of Science in Computer Engineering   

    Course:     EE 5190 Laboratory for Microprocessors Systems II    
     
    Lab 0:
        Objective:
                Understand how to use the gpio driver library from ESPRESSIF. The lab will consist of creating 
                a `sweeper` and also `led chaser`. The `sweeper` will iterate over mutiple LEDs by turning from 
                the lowest to highest bit and then turn off the most significant bit to lowest bit. While `led chaser` 
                as the name states will haven a single led iterating from the lowest to most significant bit. 
        Undergrad Bonus:
                The first bonus deals with having an LED chaser. The 
                LED should turn ON in sequential order; however, only
                one LED should be ON at a time.
        Grad Bonus:
                For the Graduate Bonus, it is expected to be creative 
                and original. The bonus consist in create a light show
                with more that 4 distinct sequences or patterns that
                the student likes or can do.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define HIGH    1
#define LOW     0

/**
 * @brief setOutputs will initialize uint8_t array as outputs
 *
 * @param out uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None void
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
      gpio_set_level(led[0]);
      vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
      gpio_set_evel(led[0];)
      vTaskDelay(1000 / portTICK_PERIOD_MS); // 1 second delay
    }
}

~~~

---
## **C helpful functions**

For this lab, there are three main function from ESPRESSIF that are important. First is to select which GPIO pin is going to be used by the following function. `gpio_pad_select(gpio_num_t gpio_num)`. The first parameter `gpio_num` is the GPIO pin that is going to be used, for instance if we want to use the ***onboard led*** we should put a 2 on `gpio_num` such as `gpio_pad_select(2)`.
~~~c
esp_err_t esp_rom_gpio_pad_select_gpio(gpio_num_t gpio_num);             
~~~
Next, another important function is the selection of the direction GPIO pin. Therefore we must use `gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode)` and for this lab we only care of *`GPIO_MODE_OUTPUT`* as we will be using outputs.
~~~c 
esp_err_t gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode);
~~~
Lastly, `gpio_set_level(gpio_num_t gpio_num, uint32_t level)` as the name states set the level of the GPIO pin that we pass throught the function. Passing a **0** is set to low and **1** is set to high respectively.
~~~c
esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level); // set LOGIC high
~~~

The following function  `vTaskDelay( const TickType_t xTicksToDelay)` is use to generate delay with ESP32 and it part of FreeRTOS, we will learn more of it in later labs. Therefore for this lab just use it to generate delay in milliseconds
~~~c
void vTaskDelay(const TickType_t xTicksToDelay);
~~~
### **Additional Links**
* [Espressif GPIO Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#)
---
### **Authors**
* [***Jesus Minjares***](https://github.com/jminjares4)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesusminjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)
* [***Erick Baca***](https://github.com/eabaca2419)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/erick-baca/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/eabaca2419)


