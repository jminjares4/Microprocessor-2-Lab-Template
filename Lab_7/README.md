# **LAB 7 Peripherals and Queues:ADC and PWM (LEDC) :zap:**

### **Objective**
*** 
* The objective for this lab is to understand how use the LEDC and ADC API's of espressif. In this lab, create 2 task: one that will initiallize the peripherals and perform ADC readings every 100 millisecond. The ADC input reading should come from a 10K potentiometer and store its information into a queue. For the second task, output a PWM signal which gets it's duty cycle updated based on the queue value send from the ADC task.

### **Bonus**
***
* Add a port interrupt to stop and start the PWM signal.+10
* Create another task with two more PWM pins using the LEDC driver and start on at the highest duty cycle and then drop down. When it reaches 0 start the next pin from 0 to the highest duty cycle and go back doing the same pattern. This will give the illusion of a wave.
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
Here is an example of a single ADC channel that will read information from a potentiometer and will print the information to the screen.
~~~c
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/adc.h>

void ADCtask(void *pvParameter){
    while(1){
        //every 100ms
        vTaskDelay(100/portTICK_PERIOD_MS);
        /*Read adc value @ CHANNEL 6*/
        int val = adc1_get_raw(ADC1_CHANNEL_6);   
        /*Print ADC reading*/

    }
}
/* Set ADC */
void setADC(){
    /*Set the ADC with @ 12 bits -> 2^12 = 4096*/
    adc1_config_width(ADC_WIDTH_BIT_12);
    /*Set CHANNEL 6 @ 2600 mV*/
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11); 
}
void app_main(void){
    /* set ADC*/
    setADC();
    /* Create Tasks */
    xTaskCreate(&ADCtask, "ADCtask", 2048,NULL, 5, NULL);
}
~~~
### **Example 2**
Here is an example of a PWM signal using the LEDC API to dim an LED.
~~~c
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <driver/ledc.h>

#define ONBOARD_LED 2               //ONBOARD GPIO LED

void PWMtask(void *pvParameter){
    int i = 0;
    while(1){
        for (i = 0; i < MAX_DUTY; i += 10){ //Iterrate the duty cycle of channel 1 to its highest point
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1,i); //set duty 
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1); //update duty 
             vTaskDelay(10/portTICK_PERIOD_MS); //100ms to avoid WDT errors
        } 
        for (i = MAX_DUTY; i > 0; i -= 10){ //Iterrate the duty cycle of channel 1 to 0
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1,i); //set duty 
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1); //update duty 
             vTaskDelay(10/portTICK_PERIOD_MS); //100ms to avoid WDT errors
        }
    }
}
/* Set PWM */
void setPWM(){
    /*
        Set LEDC Timer:
                        5000hz
                        AUTO_CLK
                        TIMER_1
                        LOW_SPEED
                        13 BIT
    */
    ledc_timer_config_t timerConfig;
    timerConfig.duty_resolution = LEDC_TIMER_13_BIT; 
    timerConfig.timer_num = LEDC_TIMER_1;
    timerConfig.freq_hz = 5000; 
    timerConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    timerConfig.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timerConfig);

    /*
        Set LEDC Channel:
                        GPIO_21
                        LOW_SPEED
                        TIMER_1
                        LOW_SPEED
                        0 duty
    */
    ledc_channel_config_t tChaConfig;
    tChaConfig.gpio_num = ONBOARD_LED;    
    tChaConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    tChaConfig.channel = LEDC_CHANNEL_1;
    tChaConfig.intr_type = LEDC_INTR_DISABLE;
    tChaConfig.timer_sel = LEDC_TIMER_1;
    tChaConfig.duty = 0;
    tChaConfig.hpoint = 4096;   //Highest point for the PWM is at 4096
    ledc_channel_config(&tChaConfig);
}
void app_main(void){
    /* set PWM*/
    setPWM();
    /* Create Tasks */
    xTaskCreate(&PWMtask, "PWMtask", 2048, NULL, 5, NULL);
}
~~~
### **Template Code**
~~~c
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <driver/ledc.h>
#include <driver/adc.h>
static xQueueHandle duty_queue = NULL;
void ADCtask(void *pvParameter)
{
    while (1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
void PWMtask(void *pvParameter)
{
    while (1)
    {
        ledc_set_duty();
        ledc_update_duty();
    }
}
void setADC()
{
    adc1_config_width();
    adc1_config_channel_atten(, );
}
void setPWM()
{
    ledc_timer_config_t timerConfig;
    timerConfig.duty_resolution = ;
    timerConfig.timer_num = ;
    timerConfig.freq_hz = ;
    timerConfig.speed_mode = ;
    ledc_timer_config(&timerConfig);
    ledc_channel_config_t tChaConfig;
    tChaConfig.gpio_num = ;
    tChaConfig.speed_mode = ;
    tChaConfig.channel = ;
    tChaConfig.intr_type = ;
    tChaConfig.timer_sel = ;
    tChaConfig.duty = ;
    ledc_channel_config(&tChaConfig);
}
void app_main()
{
    setADC();
    setPWM();
    duty_queue = xQueueCreate(10, sizeof(int));
    xTaskCreate(&ADCtask, "ADCtask", 2048, NULL, 5, NULL);
    xTaskCreate(&PWMtask, "PWMtask", 2048, NULL, 5, NULL);
}
~~~

---
## **C helpful functions**

For this Lab, there are couple additional functions from ESPRESSIF that are important for using ADC. First is understanding what bit width are you planning to read using the function `adc1_config_width(adc_bits_width_t width_bit)`. The higher the bit width, the accurate the ADC reading will be.
~~~c
typedef enum {
#if CONFIG_IDF_TARGET_ESP32
    ADC_WIDTH_BIT_9  = 0, /*!< ADC capture width is 9Bit. */
    ADC_WIDTH_BIT_10 = 1, /*!< ADC capture width is 10Bit. */
    ADC_WIDTH_BIT_11 = 2, /*!< ADC capture width is 11Bit. */
    ADC_WIDTH_BIT_12 = 3, /*!< ADC capture width is 12Bit. */
#elif SOC_ADC_MAX_BITWIDTH == 12
    ADC_WIDTH_BIT_12 = 3, /*!< ADC capture width is 12Bit. */
#elif SOC_ADC_MAX_BITWIDTH == 13
    ADC_WIDTH_BIT_13 = 4, /*!< ADC capture width is 13Bit. */
#endif
    ADC_WIDTH_MAX,
} adc_bits_width_t;
~~~
Next, you need to take into consideration that the ESP32 has specific pin for each channel for ADC1. `adc1_config_channel_atten(adc1_channel_t channel, adc_atten_t atten)` this functions is being use to delcare which channel will you be reading from. Below you will see the channel numbers with its designated GPIO pins.
~~~c
typedef enum {
    ADC1_CHANNEL_0 = 0, /*!< ADC1 channel 0 is GPIO36 */
    ADC1_CHANNEL_1,     /*!< ADC1 channel 1 is GPIO37 */
    ADC1_CHANNEL_2,     /*!< ADC1 channel 2 is GPIO38 */
    ADC1_CHANNEL_3,     /*!< ADC1 channel 3 is GPIO39 */
    ADC1_CHANNEL_4,     /*!< ADC1 channel 4 is GPIO32 */
    ADC1_CHANNEL_5,     /*!< ADC1 channel 5 is GPIO33 */
    ADC1_CHANNEL_6,     /*!< ADC1 channel 6 is GPIO34 */
    ADC1_CHANNEL_7,     /*!< ADC1 channel 7 is GPIO35 */
    ADC1_CHANNEL_MAX,
} adc1_channel_t;
~~~
Lastly, in order to aquire the ADC reading you will use the function `int adc1_get_raw(adc1_channel_t channel)`.

Now for the the LEDC API to create a PWM signal, in order to set up this function you will need to fill two structures. First you need to setup the structure `ledc_timer_config_t` where you put the duty resolution, timer, frequency and clock source. 
~~~c
typedef struct {
    ledc_mode_t speed_mode;                /*!< LEDC speed speed_mode, high-speed mode or low-speed mode */
    union {
        ledc_timer_bit_t duty_resolution;  /*!< LEDC channel duty resolution */
        ledc_timer_bit_t bit_num __attribute__((deprecated)); /*!< Deprecated in ESP-IDF 3.0. This is an alias to 'duty_resolution' for backward compatibility with ESP-IDF 2.1 */
    };
    ledc_timer_t  timer_num;               /*!< The timer source of channel (0 - 3) */
    uint32_t freq_hz;                      /*!< LEDC timer frequency (Hz) */
    ledc_clk_cfg_t clk_cfg;                /*!< Configure LEDC source clock.
                                                For low speed channels and high speed channels, you can specify the source clock using LEDC_USE_REF_TICK, LEDC_USE_APB_CLK or LEDC_AUTO_CLK.
                                                For low speed channels, you can also specify the source clock using LEDC_USE_RTC8M_CLK, in this case, all low speed channel's source clock must be RTC8M_CLK*/
} ledc_timer_config_t;
~~~
Next, you will need to fill out the channel structure which is `ledc_channel_config_t` where you will select the pin number, speed mode, channel (which is the one we setup before), intr type, timer select (which use for the previuos timer structure), duty cycle and hpoint.
~~~c
typedef struct {
    int gpio_num;                   /*!< the LEDC output gpio_num, if you want to use gpio16, gpio_num = 16 */
    ledc_mode_t speed_mode;         /*!< LEDC speed speed_mode, high-speed mode or low-speed mode */
    ledc_channel_t channel;         /*!< LEDC channel (0 - 7) */
    ledc_intr_type_t intr_type;     /*!< configure interrupt, Fade interrupt enable  or Fade interrupt disable */
    ledc_timer_t timer_sel;         /*!< Select the timer source of channel (0 - 3) */
    uint32_t duty;                  /*!< LEDC channel duty, the range of duty setting is [0, (2**duty_resolution)] */
    int hpoint;                     /*!< LEDC channel hpoint value, the max value is 0xfffff */
    struct {
        unsigned int output_invert: 1;/*!< Enable (1) or disable (0) gpio output invert */
    } flags;                        /*!< LEDC flags */

} ledc_channel_config_t;
~~~
Then the functions to change the duty cycle is `ledc_set_duty(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty)` and to actually update the change you need the function ` ledc_update_duty(ledc_mode_t speed_mode, ledc_channel_t channel)`. 

Finally the last important function while using the LEDC API is a way to pause the timer `ledc_timer_pause(ledc_mode_t speed_mode, ledc_timer_t timer_sel)` and to resume the timer `ledc_timer_resume(ledc_mode_t speed_mode, ledc_timer_t timer_sel)`.

### **Additional Links**
* [Espressif GPIO Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#)
* [Espressif ADC Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html)
* [Espressif LEDC Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html)
---
### **Authors**
* [***Jesus Minjares***](https://github.com/jminjares4)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesusminjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)
* [***Erick Baca***](https://github.com/eabaca2419)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/erick-baca-484bbb215/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/eabaca2419)


