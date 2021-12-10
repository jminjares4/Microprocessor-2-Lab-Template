# **LAB 8 DAC Peripheral :zap:**

### **Objective**
*** 
* The objective for this lab is to use and understand the DAC driver of espressif. The ESP32 DAC has a 8-bit resolution therefore the output ranges from 0-255 bits or 0.0-3.3V. Knowing this, for this lab use GPIO 25 to generate a sine wave and in GPIO 26 generate a triangle wave. 

### **Bonus**
***
* Make a Sawtooth wave in GPIO 25.
* Make the sine wave have a 10 hz frequency.

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
Here is an example of a single DAC channel doing a square wave.
~~~c
#include <stdio.h>
#include <math.h>
#include "sdkconfig.h"
#include <driver/dac.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void createSineWave(void *pvParameter){
    /* Enable DAC output */
    dac_output_enable(DAC_CHANNEL_1);
    while(1){
        /* Square wave */
            dac_output_voltage(DAC_CHANNEL_2, 255); //DAC output
            vTaskDelay(100/portTICK_PERIOD_MS); //every 100/1000 sec
            dac_output_voltage(DAC_CHANNEL_2, 0); //DAC output
            vTaskDelay(10/portTICK_PERIOD_MS); //every 100/1000 sec
    }
}
void app_main(void){
    //create task for DAC channel
    xTaskCreate(&createSquareWave, "createSquareWave", 4096, NULL, 5, NULL);
}
~~~
### **Template Code**
~~~c
#include <stdio.h>
#include <math.h>
#include "sdkconfig.h"
#include <driver/dac.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
void createTriangleWave(void *pvParameter)
{
    dac_output_enable(DAC_CHANNEL_1);
    static int i = 0;
    while (1)
    {
        dac_output_voltage(DAC_CHANNEL_1, i);
        // compute trialgular waveform value
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void createSineWave(void *pvParameter)
{
    dac_output_enable(DAC_CHANNEL_2);
    static int i = 0;
    float val;
    int n;
    while (1)
    {
        // compute sine waveform value
        dac_output_voltage(DAC_CHANNEL_2, n);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void app_main()
{
    xTaskCreate(&createTriangleWave, "createTriangleWave", 4096, NULL, 5, NULL);
    xTaskCreate(&createSineWave, "createSineWave", 4096, NULL, 5, NULL);
}
~~~
## **C helpful functions**

For this Lab, there is two additional function from ESPRESSIF that are important while using DAC. The function needed to enamble the DAC `dac_output_enable(dac_channel_t channel)`, the channel is is sepcific for 2 channels which are declare on the structure below.
~~~c
typedef enum {
    DAC_CHANNEL_1 = 0,    /*!< DAC channel 1 is GPIO25(ESP32) / GPIO17(ESP32S2) */
    DAC_CHANNEL_2 = 1,    /*!< DAC channel 2 is GPIO26(ESP32) / GPIO18(ESP32S2) */
    DAC_CHANNEL_MAX,
} dac_channel_t;
~~~
Finally, the last function needed in order to produce a DAC output will be `dac_output_voltage(dac_channel_t channel, uint8_t dac_value)` which you need to put the DAC enabled pin and the actual DAC value. Note: The DAC output value has a 8-bit resolution which its max ouput is a 255 value which represents 3.3V.

### **Additional Links**
* [Espressif GPIO Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#)
* [Espressif DAC Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/dac.html)
---
### **Author**
* [`Jesus Minjares`](https://github.com/jminjares4)
  * Master of Science in Computer Engineering
* [`Erick Baca`](https://github.com/eabaca2419)
  * Master of Science in Computer Engineering