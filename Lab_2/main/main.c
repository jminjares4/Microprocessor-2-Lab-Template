/*
    Author:     Jesus Minjares
                Master of Science in Computer Engineering   

    Course:     EE 4178/5190 Laboratory for Microprocessors Systems II    
     
    Lab 2:
        Objective:
                Understand how to use the Espressif GPIO interrupts. This lab will consist
                of simulating your own fire alarm! There is an additional driver that was developed 
                to facilitate the use of LEDs. Student must use 2 external interrupts either pull-up or pull-down
                configuration.

        Bonus:
            Undergrad Bonus:
                Display fire alarm status in terminal whenever it is active
            Grad Bonus:
                Add another LED to indicate when fire alarm is disable
                
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/led.h"


#define ESP_INTR_FLAG_DEFAULT   0   /* Interrupt flag configuration */

#define ONBOARD_LED             2   /* Onboard led */

/* Update with custom pins */
#define SMOKE_DETECTOR_PIN      0  /* Smoke detector pin */
#define DISABLE_ALARM_PIN       0  /* Disable alarm pin */

/**
 * \struct fire_alarm_t
 * \brief Fire alarm object
 * @see led.h 
 */
typedef struct{
    led_t indicator;            /*!< led indicator */
    gpio_num_t  smokeDetector;  /*!< smoke detector pin */
    gpio_num_t  disableAlarm;   /*!< disable alarm pin */
    bool active;                /*!< fire alarm state */
}fire_alarm_t;

/* Global fire alarm object */
fire_alarm_t utep_alarm;

/* GPIO interrupt handler */
static void IRAM_ATTR gpio_interrupt_handler(void * arg) {
    /* Typecast argument */
    gpio_num_t gpio = (gpio_num_t) arg;

    /* Check if smoke detector pin has been pressed or disable alarm */
    
}

/**
 * @brief Set up fire alarm interrupt
 * 
 * @param alarm pointer to fire alarm object
 * @return None
 */
void fire_alarm_interrupt_setup(fire_alarm_t * const alarm){

    /* IO configuration */
    gpio_config_t io_conf;

    /* Interrupt configuration */
    io_conf.intr_type =  ;
    io_conf.mode = ;
    io_conf.pin_bit_mask = ;
    io_conf.pull_down_en = ;
    io_conf.pull_up_en = ;
    
    /* Set configuration */
    gpio_config(&io_conf);

    /* Set default interrupt flag */
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);    
   
    /* Add ISR handler */
    gpio_isr_handler_add(alarm->smokeDetector, gpio_interrupt_handler, (void *) alarm->smokeDetector);
    /* Add missing interrupt */

}

void app_main(void)
{
    /* Initialize fire alarm object */

    /* Enable led*/
    
    while(1){
        /* Check if alarm has been activate */
        if(utep_alarm.active){
            /* Toggle indicator led @ 500 ms */
           
        }else{
            /* 100 ms delay */
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
    }

}