/*
    Author:     Jesus Minjares and Erick Baca
                Master of Science in Computer Engineering
    Course:     EE 5190 Laboratory for Microprocessors Systems II
    Lab 10:      The purpose of this lab is to wifi module of the ESP32 and connect
                with the ESP32 as a http server from a client and control a servo motor.
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
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/api.h"
#include "driver/mcpwm.h"

//BONUS 
#define _BONUS_

const static char http_html_hdr[] = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
const static char http_html_hdr1[] = "HTTP/1.1 200 OK\r\nContent-type: text/plain\r\n\r\n";
#ifdef _BONUS_
    const static char http_index_hml[] = R"=====(<!DOCTYPE html><html> <head> <meta charset = UTF-8 name = "viewport" content = "width = device-width initial-scale = 1.0"> <title>Home Page</title> </head> <body> <div class="header"> <h1>Jesus Minjares</h1> </div> <input class = "btn" id = "btn0" type="button" value = "BTN0" onclick = "sendRequest()"> <input class = "btn" id = "btn1" type="button" value = "BTN1" onclick = "sendRequest1()"> <div class="JMINJARES4"> <p> JMINJARES4@MINERS.UTEP.EDU </p> <div id="sen"></div> </div> <style> *{margin:0; padding:0;} body {background-color: #D4DCE2;} .header { width:100%; height:55px; color: white; background-color: #04254D; padding: 0; text-align:center; } .header h1{ color:white; vertical-align:center; font-size:42px; } .btn { margin: 0; margin-top: .5%; background-color: #FB9541; width:48%; border: none; color: white; padding: 25px 38px; text-align: center; text-decoration: none; font-size: 16px; } .JMINJARES4 { margin: 0; margin-top: .5%; width:100%; height:70px; color: white; background-color: #04254D; padding: 0; text-align:center; } .sensorVal p{ color:white; vertical-align:center; font-size:38px; } </style> <script> function sendRequest(){ var http = new XMLHttpRequest(); http.onreadystatechange = (()=>{ if(http.readyState === 4){ if(http.status === 200){ var res = http.responseText; } } }); http.open("GET", "0", true); http.send(); } function sendRequest1(){ var http = new XMLHttpRequest(); http.onreadystatechange = (()=>{ if(http.readyState === 4){ if(http.status === 200){ var res = http.responseText; } } }); http.open("GET", "1", true); http.send(); } </script> </body></html>)=====";
#else
    const static char http_index_hml[] = R"=====(
<!DOCTYPE html>
<html>

<head>
    <meta charset=UTF-8 name="viewport" content="width = device-width initial-scale = 1.0">
    <title>Home Page</title>
</head>

<body>
    <div class="header">
        <h1>Home Page</h1>
    </div> <input class="btn" id="btn0" type="button" value="BTN0" onclick="sendRequest()"> <input class="btn" id="btn1"
        type="button" value="BTN1" onclick="sendRequest1()">
    <div class="sensorVal">
        <p>Sensor Value: </p>
        <div id="sen"></div>
    </div>
    <style>
        * {
            margin: 0;
            padding: 0;
        }

        body {
            background-color: #D4DCE2;
        }

        .header {
            width: 100%;
            height: 55px;
            color: white;
            background-color: #04254D;
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
            margin-top: .5%;
            background-color: #FB9541;
            width: 48%;
            border: none;
            color: white;
            padding: 25px 38px;
            text-align: center;
            text-decoration: none;
            font-size: 16px;
        }

        .sensorVal {
            margin: 0;
            margin-top: .5%;
            width: 100%;
            height: 70px;
            color: white;
            background-color: #04254D;
            padding: 0;
            text-align: center;
        }

        .sensorVal p {
            color: white;
            vertical-align: center;
            font-size: 38px;
        }
    </style>
    <script> function sendRequest() { 
                var http = new XMLHttpRequest(); 
                http.onreadystatechange = (() => { 
                    if (http.readyState === 4) { 
                        if (http.status === 200) { 
                            var res = http.responseText; 
                            } 
                    } 
                }); 
                http.open("GET", "0", true); 
                http.send(); 
            } 
            function sendRequest1() { 
                var http = new XMLHttpRequest(); 
                http.onreadystatechange = (() => { 
                    if (http.readyState === 4) { 
                        if (http.status === 200) { 
                            var res = http.responseText; 
                        } 
                    } 
                }); 
                http.open("GET", "1", true); 
                http.send(); 
            }
    </script>
</body>

</html>)=====";
#endif

#define EXAMPLE_ESP_WIFI_SSID "ESP32-WIFI-MODULE"
#define EXAMPLE_ESP_WIFI_PASS "12345678"
#define EXAMPLE_MAX_STA_CONN 3
static EventGroupHandle_t s_wifi_event_group;
static const char *TAG = "wifi softAP";

static const char *SERVO_TAG = "Servo SG90";
#define SERVO_MIN_PULSEWIDTH_US (1000) // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US (2000) // Maximum pulse width in microsecond
#define SERVO_MAX_DEGREE (90)          // Maximum angle in degree upto which servo can rotate
#define SERVO_PULSE_GPIO (23)          // GPIO connects to the PWM signal line

static inline uint32_t example_convert_servo_angle_to_duty_us(int angle)
{
    return (angle + SERVO_MAX_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (2 * SERVO_MAX_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

static esp_err_t event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    case SYSTEM_EVENT_AP_STACONNECTED:
        ESP_LOGI(TAG, "station:" MACSTR " join, AID=%d",
                 MAC2STR(event->event_info.sta_connected.mac),
                 event->event_info.sta_connected.aid);
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        ESP_LOGI(TAG, "station:" MACSTR "leave, AID=%d",
                 MAC2STR(event->event_info.sta_disconnected.mac),
                 event->event_info.sta_disconnected.aid);
        break;
    default:
        break;
    }
    return ESP_OK;
}
void wifi_init_softap()
{
    s_wifi_event_group = xEventGroupCreate();
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
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
    ESP_LOGI(TAG, "wifi_init_softap finished.SSID:%s password:%s", EXAMPLE_ESP_WIFI_SSID,
             EXAMPLE_ESP_WIFI_PASS);
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
            printf("%c\n", buf[5]);
            /* Send the HTML header
             * subtract 1 from the size, since we dont send the \0 in the string
             * NETCONN_NOCOPY: our data is const static, so no need to copy it
             */
            // command from btn0 = '0' command from btn1 = '1'
            if (buf[5] == '0')
            {
                netconn_write(conn, http_html_hdr1, sizeof(http_html_hdr1) - 1, NETCONN_NOCOPY);
                netconn_write(conn, "1", 1, NETCONN_NOCOPY);
               // for (int angle = -SERVO_MAX_DEGREE; angle < SERVO_MAX_DEGREE; angle++)
                //{
                    ESP_LOGI(TAG, "Angle of rotation: %d", 90);
                    ESP_ERROR_CHECK(mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, example_convert_servo_angle_to_duty_us(0)));
                    vTaskDelay(1000/portTICK_RATE_MS); // Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation under 5V power supply
                // }
            }
            if (buf[5] == '1')
            {
                netconn_write(conn, http_html_hdr1, sizeof(http_html_hdr1) - 1, NETCONN_NOCOPY);
                netconn_write(conn, "1", 1, NETCONN_NOCOPY);
            //    for (int angle = SERVO_MAX_DEGREE; angle > -SERVO_MAX_DEGREE; angle--)
              //  {
                    ESP_LOGI(TAG, "Angle of rotation: %d", -90);
                    ESP_ERROR_CHECK(mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, example_convert_servo_angle_to_duty_us(180)));
                    vTaskDelay(1000/portTICK_RATE_MS); // Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation under 5V power supply
                // }
            }
            else
            {
                netconn_write(conn, http_html_hdr, sizeof(http_html_hdr) - 1, NETCONN_NOCOPY);
                netconn_write(conn, http_index_hml, sizeof(http_index_hml) - 1, NETCONN_NOCOPY);
            }
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
void setUpPWM()
{
    // To drive a RC servo, one MCPWM generator is enough
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_PULSE_GPIO); 
    mcpwm_config_t pwm_config = {
        .frequency = 50, // frequency = 50H
        .cmpr_a = 0,     // duty cycle of PWMxA = 0
        .counter_mode = MCPWM_UP_COUNTER,
        .duty_mode = MCPWM_DUTY_MODE_0,
    };
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);
    ESP_LOGW(SERVO_TAG, "Servo motor has been intialize");
}
void app_main()
{
    nvs_flash_init();
    wifi_init_softap();
    setUpPWM();
    xTaskCreate(&http_server, "http_server", 2048, NULL, 5, NULL);
}