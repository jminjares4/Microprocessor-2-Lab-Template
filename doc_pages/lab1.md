# Lab 1 LED Controller

## Objective
* Understand how to use the gpio driver library from [`Espressif`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#). The lab will consist of using a `sweeper`, `led chaser` and any additionals led sequence that the student developed and be selected via an input. Students must have a total of three buttons. First button must start `sweeper` function which student must have complete from the previous lab. Next, the second button must start the `led chaser` function. Lastly, the third button must toggle the
state of the onboard led.

<div align='center'>
| Button | Function     | Mode      |
| :---   |  :---        | :---      |
| B0     | `Sweeper`    | Pull-down |
| B1     | `Led chaser` | Pull-down |
| B2     | `Toggle`     | Pull-down |
</div>

## Bonus 
- ***Undergrad Bonus:***
  * Create another `LED` sequence and add another **button**, as `pull-up`. 
- ***Grad Bonus:***
  * Add **button** to start lightshow from previous lab, as `pull-up`
  * Do a **reset button** which will turns off all LEDs, as `pull-up`

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
Here is an example of how to use ESP32 GPIO function calls for inputs. The following code will turn the onboard LED as long as the button is pressed.

~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LOW  0  /* LOGIC LOW*/
#define HIGH 1  /* LOGIC HIGH*/
#define ONBOARD_LED 2 /* ONBOARD LED GPIO Pin*/

#define BUTTON_0 23 /* Button 1 */

void app_main(void){

  /* Initialize OUTPUTs */
  esp_rom_gpio_pad_select_gpio(ONBOARD_LED); /* select GPIO pins */
  gpio_set_direction(ONBOARD_LED, GPIO_MODE_OUTPUT); /* set as output */

  /* Intialize INPUTs */
  esp_rom_gpio_pad_select_gpio(BUTTON_1); /* select GPIO pin */
  gpio_set_direction(BUTTON_0, GPIO_MODE_INPUT); /* set as input */
  gpio_set_pull_mode(BUTTON_0, GPIO_PULLDOWN_ONLY); /* set as pull-down */

  while(1){
    /* read gpio level */
    int res = gpio_get_level(BUTTON_0); 
    vTaskDelay(10/portTICK_PERIOD_MS); /* debounce button */
    
    /* Check return value */
    if(res == 1){
        gpio_set_level(ONBOARD_LED, HIGH); /* set high */
    }else{
        gpio_set_level(ONBOARD_LED, LOW); /* set low */
    }
    
    /* ternary operator */
    //res ? gpio_set_level(ONBOARD_LED, HIGH) : gpio_set_level(ONBOARD_LED, LOW);
    
    vTaskDelay(100/portTICK_PERIOD_MS); /* avoid WDT trigger */
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
 * @brief setInputs will initialize uint8_t array as inputs
 *
 * @param led uint8_t * array which hold GPIOs pins
 * @param size size of the array
 * @return None
 * @note initialize gpio before using them
 */
void setInputs(uint8_t *in, int size)
{
    /* iterate over the size of the array */
    for (int i = 0; i < size; i++)
    {
        /* select the GPIO pins */

        /* set direction as inputs */

        /* set as pull-down */

    }
    return;
}
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
    /* iterate over the size of the array */
    for (int i = 0; i < size; i++) 
    {
       
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
   
}

/*Replace with yours*/
void light_show(){

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

}

void app_main(void)
{
    /* Select GPIOs output and store in led array */
    uint8_t led[] = {13}; /*replace with your GPIO pins */
    
    /* Select GPIOs inputs and store in button array */
    uint8_t button[] = {22, 23};

    /* Use sizeof() to get the size of the arrays */
    int led_size = sizeof(led) / sizeof(uint8_t);
    int button_size = sizeof(button) / sizeof(uint8_t);

    /* Initialize Inputs */
    setInputs(button, button_size);

    /* Initialize Outputs */
    setOutputs(led, led_size);

    while (1)
    {
        vTaskDelay(5/portTICK_PERIOD_MS); /* debounce button */
        int button0 = gpio_get_level(button[0]); /* read button 0 */
        /* add additonal buttons */ 
        if(button0 == 1){
         
        }
        else{
          vTaskDelay(100/portTICK_PERIOD_MS); /* 100ms to avoid WDT errors */
        }
    }
}
~~~

## C helpful functions
For this lab, there are additional functions from Espressif that are important for using inputs. As the previous lab, we must set the direction of the GPIO pin by using `gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode)`. Previously we used <strong>`GPIO_MODE_OUTPUT`</strong> as we using outputs, however now that we will be using inputs, we need <strong>`GPIO_MODE_INPUT`</strong>.
~~~c 
esp_err_t gpio_set_direction(gpio_num_t gpio_num, gpio_mode_t mode);
~~~

Next, as the name states `gpio_get_level(gpio_num_tgpio_num)` returns the current level of the GPIO pin whether it high and low. This function is use to get the current state of the input. 
~~~c
int gpio_get_level(gpio_num_ tgpio_num);
~~~
<div align='center'>
| Value | Logic |
|:---   | :---  |
| **0** | low   |
| **1** | high  |
</div>

Lastly, there are two additional functions that are crucial when using inputs. There are two configurations for inputs either pull-up or pull-down: `gpio_pulldown_en(gpio_num_t gpio_num)` and `gpio_pullup_en(gpio_num_t gpio_num)`. Down below are the configuration for both pull-up and pull-down.
~~~c
esp_err_t gpio_pullup_en(gpio_num_t gpio_num);
~~~
~~~c
esp_err_t gpio_pulldown_en(gpio_num_t gpio_num);
~~~

## Pull-up and Pull-down Configuration

<img width="413" alt="button configuation" src="button_config.png">

## Additional Links
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
 <a href="https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_1">
 <img src="github.png">
 </a>
[Lab 1 Repository](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_1)
</div>

<span class="next_section_button">
Read Next: [Lab 2](@ref doc_pages/lab2.md)
</span>