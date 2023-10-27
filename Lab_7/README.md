<div align='center'>
  <a href="https://www.utep.edu/engineering/ece/">
    <img src="../images/university_of_texas_at_el_paso_logo.png" height="200">
    <h1>
      Electrical and Computer Engineering Department
    </h1>
  </a>
</div>

# **Lab 7 Peripherals and Queues: ADC and PWM (LEDC) :zap:**

## **Objective**

* The objective for this lab is to understand how use the Espressif [`LEDC`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html) and [`ADC`](https://docs.espressif.com/projects/esp-idf/en/v4.2/esp32/api-reference/peripherals/adc.html) drivers. In this lab, create 2 task: adc and pwm task. The `adc task` performs ADC readings every 100 millisecond (10 hertz). This task must store the adc readings and be sent through a queue. Lastly, `pwm task` should receive the queue data from the `adc task` and update `ledc` duty cycle. 

| Task          |  Description               | Queue        |
| :---          | :---                       | :---         |
| `adc_task`    | Read analog value @ 10 hz  | send data    |
| `pwm_task`    | Update duty cycle          | receive data |

## **Bonus**
- ***Undergrad Bonus***
  - Add `gpio` interrupt to start and stop **PWM** signal
- ***Grad Bonus***
  - Modifiy code to use **2** more PWM pins. 

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
Here is an example of a single ADC channel that will read and print value to the screen.
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/adc.h>

/* ADC task */
void ADCtask(void *pvParameters){
    while(1){
        /* Read adc value @ CHANNEL 6*/
        int val = adc1_get_raw(ADC1_CHANNEL_6);   
        vTaskDelay(100/portTICK_PERIOD_MS); /* 100 ms */
        
        /* Print reading */
        printf("ADC val: %d\n", val);
    }
}

/* Set ADC */
void adc_setup(void){

    /*Set the ADC with @ 12 bits -> 2^12 = 4096*/
    adc1_config_width(ADC_WIDTH_BIT_12);

    /*Set CHANNEL 6 @ 2600 mV*/
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11); 
    
}

void app_main(void){
    /* set ADC*/
    adc_setup();
    /* Create Tasks */
    xTaskCreate(&ADCtask, "ADCtask", 2048,NULL, 5, NULL);
}
~~~

## **Example 2**
Here is an example of a PWM signal using the LEDC API to dim an LED.
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <driver/ledc.h>

#define ONBOARD_LED 2  /* Onboard LED */

#define MAX_DUTY    4096 /* max duty cycle */

/* PWM task */
void PWMtask(void *pvParameters){
    int i = 0;
    while(1){
        /* Iterate over channel 1 duty cycle, increase */
        for (i = 0; i < MAX_DUTY; i += 10){ 
            /* set duty cycle */
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1,i);
            /* update duty cycle */
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1); //update duty 
            vTaskDelay(10/portTICK_PERIOD_MS); /* 10ms */ 
        }
        /* Iterate over channel 1 duty cycle, decrease */
        for (i = MAX_DUTY; i > 0; i -= 10){ 
            /* set duty cycle */
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1,i);
            /* update duty cycle */
            ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1); //update duty 
            vTaskDelay(10/portTICK_PERIOD_MS); /* 10ms */ 
        }
    }
}

/* Set PWM */
void pwm_setup(void){
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
                        INTERRUPT_DISABLE
                        0 duty
                        Max duty 
    */
    ledc_channel_config_t channelConfig;
    channelConfig.gpio_num = ONBOARD_LED;    
    channelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    channelConfig.channel = LEDC_CHANNEL_1;
    channelConfig.intr_type = LEDC_INTR_DISABLE;
    channelConfig.timer_sel = LEDC_TIMER_1;
    channelConfig.duty = 0;
    channelConfig.hpoint = MAX_DUTY;   
    ledc_channel_config(&channelConfig);
}

void app_main(void){

    /* set PWM*/
    pwm_setup();

    /* Create Tasks */
    xTaskCreate(&PWMtask, "PWMtask", 2048, NULL, 5, NULL);
    
}
~~~

## **Example 3**
This example can display the use of PWM on the ESP32 using a buzzer with a center frequency of 2.7k Hz. It will play the tune of mary had a little lamb changing the frequency to play the notes.
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <driver/ledc.h>

#define BUZZ 21  /* Buzzer */

#define MAX_DUTY    4096 /* max duty cycle */
#define DUTY50	2048 //50% duty cycle

// Notes for octave meeting the buzzer's central frequency
#define E0	    2637
#define D0	    2349
#define C0	    2093
#define F0	    2793
#define G0	    3126
#define A0	    3520
#define B0	    3951
#define REST    0

/* PWM task */
void PWMtask(void *pvParameters){
    //Notes of Mary had a little lamb
    //The 5 at the end is used as an indicator for the end of the array, it's what allows for the song to run as a while loop instead of a for loop
    int freq[] = {E0, D0, C0, D0, E0, E0, E0, REST, D0, D0, D0, REST, E0, E0, E0, REST, E0, D0, C0, D0, E0, E0, E0, REST, D0, D0, E0, D0, C0, REST};
    int total_notes = sizeof(freq)/sizeof(int);
    //Set duty cycle to 50%
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1,DUTY50);
	ledc_update_duty(LEDC_LOW_SPEED_MODE,LEDC_CHANNEL_1);
    while(1){
        // loop through the song
        for(int note = 0; note < total_notes; notes++){
            if(freq[note] != REST){
		        ledc_timer_resume(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
				ledc_set_freq(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1,freq[note]);
				vTaskDelay(500/portTICK_PERIOD_MS);
				ledc_timer_pause(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
				vTaskDelay(50/portTICK_PERIOD_MS);
            }else{
                ledc_timer_pause(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
				vTaskDelay(550/portTICK_PERIOD_MS);
            }
        }
    }
}

/* Set PWM */
void pwm_setup(void){
    /*
        Set LEDC Timer:
                        262hz
                        AUTO_CLK
                        TIMER_1
                        LOW_SPEED
                        13 BIT
    */
    ledc_timer_config_t timerConfig;
    timerConfig.duty_resolution = LEDC_TIMER_13_BIT;
    timerConfig.timer_num = LEDC_TIMER_1;
    //Initialized frequency wont be important for PWM with buzzer, since it will be modified to play different notes
    timerConfig.freq_hz = 262;
    timerConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    timerConfig.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timerConfig);

    /*
        Set LEDC Channel:
                        GPIO_21
                        LOW_SPEED
                        TIMER_1
                        INTERRUPT_DISABLE
                        0 duty
                        Max duty
    */
    ledc_channel_config_t channelConfig;
    channelConfig.gpio_num = BUZZ;
    channelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    channelConfig.channel = LEDC_CHANNEL_1;
    channelConfig.intr_type = LEDC_INTR_DISABLE;
    channelConfig.timer_sel = LEDC_TIMER_1;
    channelConfig.duty = 0;
    channelConfig.hpoint = MAX_DUTY;
    ledc_channel_config(&channelConfig);
}

void app_main(void){

    /* set PWM*/
    pwm_setup();

    /* Create Tasks */
    xTaskCreate(&PWMtask, "PWMtask", 2048, NULL, 5, NULL);

}
~~~
> Written by [Jose Granados](https://github.com/JosGranados)

## **Lab Template**
~~~c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <driver/ledc.h>
#include <driver/adc.h>

/* Global queue handle */
QueueHandle_t duty_queue = NULL;

/* ADC task */
void ADCtask(void *pvParameters)
{
    while (1)
    {
       /* Read adc channel*/

       /* Send data to queue */
    }
}

/* PWM task */
void PWMtask(void *pvParameters)
{
    while (1)
    {
        /* Receive data */

        /* Update duty cycle */

    }
}

/* ADC setup */
void adc_setup(void)
{
    /* Set adc */
}

/* PWM setup */ 
void pwm_setup(void)
{

}

void app_main()
{
    /* Setup ADC and PWM */

    /* Create queue */

    /* Create tasks */
    xTaskCreate(&ADCtask, "ADCtask", 2048, NULL, 5, NULL);
    xTaskCreate(&PWMtask, "PWMtask", 2048, NULL, 5, NULL);
}
~~~

## **C helpful functions**

Espressif ADC driver requires the following structures and enumerations in order to set the correct pin and operation. There are a few GPIOs that are avaiable to use analog converter. Depending on the ESP32, there are slight difference between the pinout so please verify the hardware. 
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
~~~c
typedef enum {
    ADC2_CHANNEL_0 = 0, /*!< ADC2 channel 0 is GPIO4  (ESP32), GPIO11 (ESP32-S2) */
    ADC2_CHANNEL_1,     /*!< ADC2 channel 1 is GPIO0  (ESP32), GPIO12 (ESP32-S2) */
    ADC2_CHANNEL_2,     /*!< ADC2 channel 2 is GPIO2  (ESP32), GPIO13 (ESP32-S2) */
    ADC2_CHANNEL_3,     /*!< ADC2 channel 3 is GPIO15 (ESP32), GPIO14 (ESP32-S2) */
    ADC2_CHANNEL_4,     /*!< ADC2 channel 4 is GPIO13 (ESP32), GPIO15 (ESP32-S2) */
    ADC2_CHANNEL_5,     /*!< ADC2 channel 5 is GPIO12 (ESP32), GPIO16 (ESP32-S2) */
    ADC2_CHANNEL_6,     /*!< ADC2 channel 6 is GPIO14 (ESP32), GPIO17 (ESP32-S2) */
    ADC2_CHANNEL_7,     /*!< ADC2 channel 7 is GPIO27 (ESP32), GPIO18 (ESP32-S2) */
    ADC2_CHANNEL_8,     /*!< ADC2 channel 8 is GPIO25 (ESP32), GPIO19 (ESP32-S2) */
    ADC2_CHANNEL_9,     /*!< ADC2 channel 9 is GPIO26 (ESP32), GPIO20 (ESP32-S2) */
    ADC2_CHANNEL_MAX,
} adc2_channel_t;
~~~
The ESP has default 12 bit max width conversion but depending on the hardware, it can go up to 13 bit. To keep everything simple, please do not exceed 12 bit width. Espressif ADC width is selected by `adc_bit_width_t` enumeration down below. 
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

Espressif ADC driver has an attenuation feature to limit voltage reading to a fix range. The `adc_atten_t` enumeration allows to select the range of the adc channel. Please go over the example program to get an idea of how the **ADC** driver works. 
~~~c
typedef enum {
    ADC_ATTEN_DB_0   = 0,  ///<No input attenumation, ADC can measure up to approx. 800 mV
    ADC_ATTEN_DB_2_5 = 1,  ///<The input voltage of ADC will be attenuated extending the range of measurement by about 2.5 dB (1.33 x)
    ADC_ATTEN_DB_6   = 2,  ///<The input voltage of ADC will be attenuated extending the range of measurement by about 6 dB (2 x)
    ADC_ATTEN_DB_11  = 3,  ///<The input voltage of ADC will be attenuated extending the range of measurement by about 11 dB (3.55 x)
} adc_atten_t;
~~~

Next, Espressif LEDC driver requires to use the following structures. `ledc_timer_config_t` structure set
the duty resolution, timer, frequency and clock source. 
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
Furthermore, `ledc_channel_config_t` structure allows to select the pin number, speed mode, channel, intr type, timer select, duty cycle and hpoint. For more details, please see Espressif documentation and go over the example programs to understand how to use it properly.
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
Additionally, `ledc_set_duty(ledc_mode_t speed_mode, ledc_channel_t channel, uint32_t duty)` is use to set the duty cycle  and to actually update the duty you need to use ` ledc_update_duty(ledc_mode_t speed_mode, ledc_channel_t channel)`. 
Lastly, `LEDC` driver allows users to control the operation of the timer: `ledc_timer_pause(ledc_mode_t speed_mode, ledc_timer_t timer_sel)` and `ledc_timer_resume(ledc_mode_t speed_mode, ledc_timer_t timer_sel)`.

## **Additional Links**
* [Espressif GPIO Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#)
* [Espressif ADC Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/adc.html)
* [Espressif LEDC Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/ledc.html)

## **Authors**
* [**Jesus Minjares**](https://github.com/jminjares4)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesusminjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)
* [**Erick Baca**](https://github.com/eabaca2419)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/erick-baca/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/eabaca2419)


