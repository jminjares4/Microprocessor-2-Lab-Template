#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/esp_lcd.h"
/* Create Function to covert ADC Value to Voltage */

/* Create Function for ADC initialization */
void ADC_setup(void){

}
/* Function for sweeper */
void sweeper_function(uint32_t val){

}
/* LCD task */
void lcd_task(void *pvParameters){
    /* Setup LCD */
    
    while(1){
        /* Get ADC Raw Readings */

        /* Get Conversion */

        /* Display Voltage onto LCD */

        /* Change PWM output to match ADC readings */

    }
}
void app_main(void){
    /* Create LCD Task */
    
}