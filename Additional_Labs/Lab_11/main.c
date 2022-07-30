/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering
    Course:     EE 4178/5190 Laboratory for Microprocessors Systems II
    Lab 11:      The purpose of this lab is to wifi module of the ESP32 and connect
                with the ESP32 as a http server from a client and control a LED and ADC readings.
    Date:       10-30-2021
    Pinout:
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
*/
#include <string.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/api.h"

#define onboardLED 2

const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
const static char http_txt_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\n";
const static char http_index_hml[] = R"=====(
<!DOCTYPE html>
<html>
    <head>
        <meta
            charset="UTF-8"
            name="viewport"
            content="width = device-width initial-scale = 1.0"
        />
        <title>Home Page</title>
    </head>
    <body>
        <div class="header"><h1>Home Page</h1></div>
        <input
            class="btn"
            id="btn0"
            type="button"
            value="BTN0"
            onclick="sendRequestLed()"
        />
        <input
            class="btn"
            id="btn1"
            type="button"
            value="get data"
            onclick="sendRequestData()"
        />
        <div class="sensorVal">
            <p>Sensor Value:</p>
            <p id="sen"></p>
        </div>
        <style>
            * {
                margin: 0;
                padding: 0;
            }

            body {
                background-color: #d4dce2;
            }

            .header {
                width: 100%;
                height: 55px;
                color: white;
                background-color: #04254d;
                padding: 0;
                text-align: center;
            }

            .header h1 {
                color: white;
                vertical-align: center;
                font-size: 42px;
            }

            .btn {
                margin: 0;
                margin-top: 0.5%;
                background-color: #fb9541;
                width: 48%;
                border: none;
                color: white;
                padding: 25px 38px;
                text-align: center;
                text-decoration: none;
                font-size: 16px;
                border-radius: 15%;
            }

            .btn:hover {
                cursor: pointer;
                background-color: #e27217;
            }

            .sensorVal {
                margin: 0;
                margin-top: 0.5%;
                width: 100%;
                height: 100px;
                color: white;
                background-color: #04254d;
                padding: 0;
                text-align: center;
            }

            .sensorVal p {
                color: white;
                vertical-align: center;
                font-size: 38px;
            }

        </style>
        <script>
            function changeButton(value) { 
                var btn = document.getElementById("btn0"); 
                if(value === "0"){ 
                    btn.value = "LED is OFF"; 
                } else { 
                    btn.value = "LED is ON"; 
                } 
            } 
            
            function sendRequestLed(){ 
                var http = new XMLHttpRequest(); 
                http.onreadystatechange = (()=>{
                    if(http.readyState === 4){ 
                        if(http.status === 200){ 
                            changeButton(http.responseText); 
                        } 
                    } 
                });
               http.open("GET", "0", true); 
               http.send(); 
            } 
            
            function sendRequestData(){ 
                var http = new XMLHttpRequest();
                http.onreadystatechange = (()=>{ 
                    if(http.readyState === 4){ 
                        if(http.status === 200){
                            document.getElementById("sen").innerHTML = http.responseText; 
                        } 
                    } 
                }); 
                http.open("GET", "1", true);
                http.send(); } 
        </script>
    </body>
</html>
)=====";

#define EXAMPLE_ESP_WIFI_SSID "Minjares"
#define EXAMPLE_ESP_WIFI_PASS "80582759"
#define EXAMPLE_MAX_STA_CONN 3

static EventGroupHandle_t s_wifi_event_group;

void wifi_init_softap()
{
    s_wifi_event_group = xEventGroupCreate();
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(NULL, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK},
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void onURL(struct netconn *conn, char command)
{
    if (command == '0') //If BTN0 is pressed
    {
        GPIO.out ^= BIT2;
        if((GPIO.out & BIT2) == BIT2) //If the ONBOARD LED is On, send a 1 to the webpage
        {
            printf("LED Current level: 1");
            netconn_write(conn, http_txt_hdr, sizeof(http_txt_hdr) - 1, NETCONN_NOCOPY);
            netconn_write(conn, "1", 1, NETCONN_NOCOPY);
        }
        else //If the ONBOARD LED is Off, send a 0 to the webpage
        {
            printf("LED Current level: 0");
            netconn_write(conn, http_txt_hdr, sizeof(http_txt_hdr) - 1, NETCONN_NOCOPY);
            netconn_write(conn, "0", 1, NETCONN_NOCOPY);
        }
        
    }
    else if (command == '1') //If the get data button is pressed
    {
        int adc = adc1_get_raw(ADC1_CHANNEL_6); //Get the ADC reading
        float voltage = (adc * 3.3)/4095;          //Multiply times the ratio so that our reading comes out from 0V to 3.3V (but in milivolts)
        char ans[24];                                //String that we're going to send back to the webpage
        memset(ans, 0, sizeof(ans));
        sprintf(ans,"%d ADC -> %.2f V", adc, voltage);
        netconn_write(conn, http_txt_hdr, sizeof(http_txt_hdr) - 1, NETCONN_NOCOPY);
        netconn_write(conn, ans, sizeof(ans), NETCONN_NOCOPY);
    }
    else
    {   
        netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
        netconn_write(conn, http_index_hml, sizeof(http_index_hml) - 1, NETCONN_NOCOPY);
    }
}

static void http_server_netconn_serve(struct netconn *conn)
{
    struct netbuf *inbuf;
    char *buf;
    u16_t buflen;
    err_t err;
    /* Read the data from the port, blocking if nothing yet there.
 We assume the request (the part we care about) is in one netbuf */
    err = netconn_recv(conn, &inbuf);
    if (err == ERR_OK)
    {
        netbuf_data(inbuf, (void **)&buf, &buflen);
        /* Is this an HTTP GET command? (only check the first 5 chars, since
 there are other formats for GET, and we're keeping it very simple )*/
        if (buflen >= 5 &&
            buf[0] == 'G' &&
            buf[1] == 'E' &&
            buf[2] == 'T' &&
            buf[3] == ' ' &&
            buf[4] == '/')
        {
            /* Send the HTML header
 * subtract 1 from the size, since we dont send the \0 in the string
 * NETCONN_NOCOPY: our data is const static, so no need to copy it
 */
            onURL(conn, buf[5]);
        }
    }
    netconn_close(conn);
    netbuf_delete(inbuf);
}

static void http_server(void *pvParameters)
{
    struct netconn *conn, *newconn;
    err_t err;
    conn = netconn_new(NETCONN_TCP);
    netconn_bind(conn, NULL, 80);
    netconn_listen(conn);
    do
    {
        err = netconn_accept(conn, &newconn);
        if (err == ERR_OK)
        {
            http_server_netconn_serve(newconn);
            netconn_delete(newconn);
        }
    } while (err == ERR_OK);
    netconn_close(conn);
    netconn_delete(conn);
}

void setADC()
{
    adc1_config_width(ADC_WIDTH_BIT_12);                         //Width of 12 bits, so we range from 0 to 4096
    adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);  //Attenuation to be able to read higher voltages
}

void setGPIO()
{
    esp_rom_gpio_pad_select_gpio(onboardLED);
    gpio_set_direction(onboardLED, GPIO_MODE_OUTPUT);
}

void app_main()
{
    nvs_flash_init();
    setADC();
    setGPIO();
    wifi_init_softap();
    xTaskCreate(&http_server, "http_server", 2048, NULL, 5, NULL);
}