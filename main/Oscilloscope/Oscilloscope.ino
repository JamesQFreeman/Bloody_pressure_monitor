#include <Adafruit_ADS1015.h>
#include <Arduino.h>

/*
 * _SAMPLING_RATE 采样率 每秒采样次数
 * 
 */
#define _SAMPLING_RATE 500
#define DATA_ARRAY_SIZE 1

#define _ADC_DELAY (1000/_SAMPLING_RATE)

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


