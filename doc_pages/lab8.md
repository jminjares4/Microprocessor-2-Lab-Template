# Lab 8 DAC Peripheral

## Objective

* The objective for this lab is to understand how to use Espressif [`DAC`](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/dac.html) driver. Digital to Analog Converter (DAC) is a peripheral that converts digital signals to analog signals. The ESP32 has *DAC* of 8-bit resolution ranging from 0-255 bit value or 0.0-3.3V. ESP32 has **only** two DAC channels: channel 1 (GPIO 25) and channel 2 (GPIO 26). Students must generate a triangle wave with `channel 1` and sine wave with `channel 2`.

<div align='center'>
| Tasks         |   Description                       |
| :---          |   :---                              |
| Triangle Wave | create triangle wave with `GPIO 25` |
| Sine Wave     | create sine wave with `GPIO 26`     |
<br>
| GPIO Pin | DAC Channel   |
| :---     | :---          |
| `GPIO 25`  | Channel 1   |
| `GPIO 26`  | Channel 2   | 
<br>
| Digital   | Analog    |
| :---      | :---      |
| 0         | 0.0 v     |
| ...       | ...       |
| 255       | 3.3 v     |
</div>

## Bonus
- ***Undergrad Bonus***
  - Replace `triangle` wave with a `saw-tooth` wave
- ***Grad Bonus***
  - Modify `sine` wave to operate @ **10** hertz

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
Here is an example of a single DAC channel doing a square wave.
~~~c
#include <stdio.h>
#include <math.h>
#include <driver/dac.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Create square wave */
void createSquareWave(void *pvParameters){

    /* Enable DAC output through channel 1 */
    dac_output_enable(DAC_CHANNEL_1);
    while(1){
        /* Generate square wave */
        dac_output_voltage(DAC_CHANNEL_2, 255); /* DAC max output: 3.3v */
        vTaskDelay(100/portTICK_PERIOD_MS); /* 100ms */
        dac_output_voltage(DAC_CHANNEL_2, 0);   /* DAC min output: 0v*/
        vTaskDelay(10/portTICK_PERIOD_MS); /* 100ms */
    }
}

void app_main(void){
    /* Create task */
    xTaskCreate(&createSquareWave, "createSquareWave", 4096, NULL, 5, NULL);
}
~~~

## Lab Template
~~~c
#include <stdio.h>
#include <math.h>
#include <driver/dac.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Triangle wave task */
void createTriangleWave(void *pvParameters)
{
    /* Enable DAC channel 1 */
    dac_output_enable(DAC_CHANNEL_1);
    /* create variable to store value */
    static int i = 0;

    while (1)
    {
        /* Generate triangle wave */

        /* output voltage through channel 1 */
        dac_output_voltage(DAC_CHANNEL_1, i);

        vTaskDelay(10 / portTICK_PERIOD_MS); /* 10 ms*/
    }
}

/* Sine wave task */
void createSineWave(void *pvParameters)
{
    /* Enable DAC channel 2 */
    dac_output_enable(DAC_CHANNEL_2);
    /* Variables to compute sine wave */
    static int i = 0;
    float val;
    int n = 0;

    while (1)
    {
        /* Compute sine wave */
        

        /* output voltage through channel 2 */
        dac_output_voltage(DAC_CHANNEL_2, n);

        vTaskDelay(10 / portTICK_PERIOD_MS);  /* 10 ms*/
    }
}

void app_main()
{
    /* Create tasks */
    xTaskCreate(&createTriangleWave, "createTriangleWave", 4096, NULL, 5, NULL);
    xTaskCreate(&createSineWave, "createSineWave", 4096, NULL, 5, NULL);
}
~~~

## C helpful functions

For this Lab, there is two additional functions from Espressif that are important to use DAC peripheral. 
As previously mentioned, ESP32 has **2** DAC channels. The channel is enable by using: `dac_output_enable(dac_channel_t channel)`. `dac_channel_t` is an enumeration that is provided down below. Please select the correct channel based on the GPIO pin and hardware. 
~~~c
typedef enum {
    DAC_CHANNEL_1 = 0,    /*!< DAC channel 1 is GPIO25(ESP32) / GPIO17(ESP32S2) */
    DAC_CHANNEL_2 = 1,    /*!< DAC channel 2 is GPIO26(ESP32) / GPIO18(ESP32S2) */
    DAC_CHANNEL_MAX,
} dac_channel_t;
~~~
Finally, the following function is use to produce a DAC output: `dac_output_voltage(dac_channel_t channel, uint8_t dac_value)`. The DAC output value has a 8-bit resolution, please see the table provide in *Objective* section. 

## Additional Links
* [Espressif GPIO Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/gpio.html#)
* [Espressif DAC Driver API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/dac.html)

## Authors
* [**Jesus Minjares**](https://github.com/jminjares4)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/jesusminjares/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/jminjares4)
* [**Erick Baca**](https://github.com/eabaca2419)
  * **Master of Science in Computer Engineering** <br>
    [![LinkedIn](https://img.shields.io/badge/LinkedIn-0077B5?style=for-the-badge&logo=linkedin&logoColor=white&style=flat)](https://www.linkedin.com/in/erick-baca/) [![GitHub](https://img.shields.io/badge/GitHub-100000?style=for-the-badge&logo=github&logoColor=white&style=flat)](https://github.com/eabaca2419)


## GitHub
<div align='left'>
 <a href="https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_8">
 <img src="github.png">
 </a>
[Lab 8 Repository](https://github.com/jminjares4/Microprocessor-2-Lab-Template/tree/main/Lab_8)
</div>

<span class="next_section_button">
Read Next: [Additional Labs](@ref doc_pages/additional_labs.md)
</span>