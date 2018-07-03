#include <Adafruit_ADS1015.h>
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

/*
 * _SAMPLING_RATE 采样率 每秒采样次数
 * 
 */
#define _SAMPLING_RATE 500
#define DATA_ARRAY_SIZE 1

#define _ADC_DELAY (1000/_SAMPLING_RATE)

/*
 * 服务器地址
 */
#define SERVER_ADDR "http://192.168.137.1:5000/data"

float pressure_data[DATA_ARRAY_SIZE];

/*
 * 定义了ADC
 * 走I2C
 * 16bit
 */ 
Adafruit_ADS1115 ads;

/*
 * 定义ADC的输入口
 * ADC 0口为
 * ADC 3口为
 */
#define SIG0 0
#define SIG1 1
#define SIG2 2
#define SIG3 3


float send_data_array(float data_array[], size_t array_size){
    WiFiMulti wifiMulti;
    // wifi setup
    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[WIFI SETUP] WAIT %d...\n", t);
        Serial.flush();
        delay(1000);
    }
    wifiMulti.addAP("hotspot", "whatapwd");
    while(wifiMulti.run() != WL_CONNECTED){
        Serial.println("TRYING TO CONNECT");
    }
    Serial.println("CONNECTED");
    Serial.println("Start to send the whole array");
    HTTPClient http;
    http.begin(SERVER_ADDR);
    
    DynamicJsonBuffer  jsonBuffer(4000);
    JsonObject& root = jsonBuffer.createObject();
    JsonArray& data = root.createNestedArray("data");
    for(int i = 0; i<array_size; i++){
        data.add(data_array[i]);
    }
    http.POST(root);
    http.end();
}


/*
 * 把ADC的int输入转换成电压信号
 * input: int 
 * output: float
 */ 
float transfer_adc2v(int n){
    float t = (n * 0.1875)/1000;
    return t;
}


void setup(void) {
    // serial的setup
    Serial.begin(115200);
    // ADC的setup
    ads.begin();
}



void loop(void){
    int16_t adc_input0;
    adc_input0 = ads.readADC_SingleEnded(SIG0);
    Serial.println(transfer_adc2v(adc_input0));
    delay(_ADC_DELAY);
}


