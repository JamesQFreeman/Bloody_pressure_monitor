#include <Adafruit_ADS1015.h>
#include <stdio.h>
#include <string.h>
#include <Arduino.h>
#include <U8g2lib.h>

/*
 * 定义测量过程中的各个参数
 *
 * ---------------以下参数可以修改---------------
 * DEFLATION_TIME 表示缓慢放气，也就是测量的时间
 * TARGET_PRESSURE 表示充气的时候的目标充气气压
 * QUICK_DEF_TIME 表示快速放气的时间 ms单位
 * 
 * ---------------以下参数不建议修改-------------
 * _SAMPLING_RATE 采样率 每秒采样次数
 * _STEP_TIME 每次充气时长 ms单位
 */ 
#define DEFLATION_TIME 10
#define TARGET_PRESSURE 140
#define _SAMPLING_RATE 100
#define _STEP_TIME 100
#define QUICK_DEF_TIME 20000
// 确定要采集几个数据点
#define DATA_ARRAY_SIZE (DEFLATION_TIME*_SAMPLING_RATE)
// 循环里每采样后的一次delay
#define SAMPLING_DELAY (1000/_SAMPLING_RATE)

// 开两个数组存储数据
int wave_data[DATA_ARRAY_SIZE];
float pressure_data[DATA_ARRAY_SIZE];


/*
 * OLED显示屏定义
 * 分辨率设置为128*64
 * clock口为17，数据口为16
 * 走I2C
 */ 
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R3, /* reset=*/ U8X8_PIN_NONE, /* clock=*/ 17, /* data=*/ 16);   // ESP32 Thing, HW I2C with pin remapping

/*
 * 定义了ADC
 * 走I2C
 * 16bit
 */ 
Adafruit_ADS1115 ads;

/*
 * 定义ADC的输入口
 * ADC 0口为波形
 * ADC 3口为压力
 */
const int WAVE_SIG = 0;
const int PRES_SIG = 3;

/*
 * 电机驱动板的定义
 * 电机驱动板的四个输入口
 */
int drive_input1 = 0;
int drive_input2 = 2;
int drive_input3 = 15;
int drive_input4 = 13;

void OLED_display_default(){
    u8g2.clearBuffer();         // clear the internal memory
    drawDateTime(0, 0, 0, 0);
    drawBP(0,0);
    u8g2.sendBuffer();          // transfer internal memory to the display
    delay(1000);
}
/*
 * OLED显示函数
 * 传入6个int参数，分别是月，日，小时，分钟
 * 还有收缩压和舒张压
 */
void OLED_display(int _mon, int _day, int _hour, int _min,
                  int systolic, int diastolic){
    u8g2.clearBuffer();         // clear the internal memory
    drawDateTime(_mon, _day, _hour, _min);
    drawBP(systolic,diastolic);
    u8g2.sendBuffer();          // transfer internal memory to the display
    delay(1000);
}

/*
 * 传入int的月份，日期，小时，分钟
 * 显示在显示屏顶上
 * 非常dirty，勿修改
 */
void drawDateTime(int _mon, int _day, int _hour, int _min){
    int xPosition = 0;
    String tempStr = "";
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    
    tempStr = tempStr + _mon + "-" + _day;
    xPosition = 1;
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()-2,tempStr.c_str());  // write something to the internal memory
    
    tempStr = "";
    tempStr = tempStr + _hour + ":" + _min;
    xPosition = 63-u8g2.getStrWidth(tempStr.c_str());
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()-2,tempStr.c_str());  // write something to the internal memory
  
    u8g2.setDrawColor(2);
    u8g2.drawBox(0,0,128,u8g2.getMaxCharHeight()-1);
}

/*
 * 显示血压的函数，传入两个int，（收缩压，舒张压）
 * 非常dirty，勿修改
 */ 
void drawBP(int h,int l){
    int offset = 9;
    int xPosition = 0;
    String tempStr = "";
  
    u8g2.setFont(u8g2_font_helvR08_tr); // choose a suitable font
    tempStr = "Systolic";
    xPosition = 0;
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
    
    offset = u8g2.getMaxCharHeight()+offset-2;
    u8g2.setFont(u8g2_font_helvR24_tn ); // choose a suitable font
    tempStr = h;
    xPosition = 63-u8g2.getStrWidth(tempStr.c_str());
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
    
    offset = u8g2.getMaxCharHeight()+offset;
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    tempStr = "mmHg";
    xPosition = 63-u8g2.getStrWidth(tempStr.c_str());
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
    
    offset = u8g2.getMaxCharHeight()+offset+5;
    u8g2.setFont(u8g2_font_helvR08_tr); // choose a suitable font
    tempStr = "Diastolic";
    xPosition = 0;
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
    
    offset = u8g2.getMaxCharHeight()+offset-2;
    u8g2.setFont(u8g2_font_helvR24_tn ); // choose a suitable font
    tempStr = l;
    xPosition = 63-u8g2.getStrWidth(tempStr.c_str());
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
    
    offset = u8g2.getMaxCharHeight()+offset;
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    tempStr = "mmHg";
    xPosition = 63-u8g2.getStrWidth(tempStr.c_str());
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
  
    u8g2.drawBox(0,125,20,3);
    u8g2.drawBox(21,125,20,3);
    u8g2.drawBox(42,125,20,3);
    u8g2.setDrawColor(2);
    u8g2.drawHLine(1,126,18);
    u8g2.drawHLine(43,126,18);
}

/*
 * 显示心率
 * 非常dirty，勿修改
 */
void drawHR(){
    int offset = 9;
    int xPosition = 0;
    String tempStr = "";
  
    u8g2.setFont(u8g2_font_helvR08_tr); // choose a suitable font
    tempStr = "Heart Rate";
    xPosition = 0;
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
    
    offset = u8g2.getMaxCharHeight()+offset-2;
    u8g2.setFont(u8g2_font_helvR24_tn ); // choose a suitable font
    tempStr = "83";
    xPosition = 63-u8g2.getStrWidth(tempStr.c_str());
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
    
    offset = u8g2.getMaxCharHeight()+offset;
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    tempStr = "/min";
    xPosition = 63-u8g2.getStrWidth(tempStr.c_str());
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
  
    u8g2.drawBox(0,125,20,3);
    u8g2.drawBox(21,125,20,3);
    u8g2.drawBox(42,125,20,3);
    u8g2.setDrawColor(2);
    u8g2.drawHLine(1,126,18);
    u8g2.drawHLine(22,126,18);
}

/*
 *非常dirty，勿修改
 */
void drawHistory(){
    int offset = 9;
    int xPosition = 0;
    String tempStr = "";
  
    u8g2.setFont(u8g2_font_helvR08_tr); // choose a suitable font
    tempStr = "History";
    xPosition = 0;
    u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
    
    offset = u8g2.getMaxCharHeight()+offset-2;
    u8g2.drawHLine(0,offset+5,64);
    
    u8g2.setFont(u8g2_font_5x7_tf); // choose a suitable font
    offset = offset+7;
    for(int i=0;i<5;i++){
        tempStr = "06-25 19:44";
        xPosition = 0;
        u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
        
        offset = u8g2.getMaxCharHeight()+offset+1;
        tempStr = "188/88 120";
        xPosition = 63-u8g2.getStrWidth(tempStr.c_str());
        u8g2.drawStr(xPosition,u8g2.getMaxCharHeight()+offset,tempStr.c_str());  // write something to the internal memory
        
        offset = u8g2.getMaxCharHeight()+offset-2;
        u8g2.drawHLine(0,offset+5,64);
        offset = offset+7;
    }

    u8g2.drawBox(0,125,20,3);
    u8g2.drawBox(22,125,20,3);
    u8g2.drawBox(44,125,20,3);
    u8g2.setDrawColor(2);
    u8g2.drawHLine(45,126,18);
    u8g2.drawHLine(23,126,18);
}

/*
 * 输入压力和波形值
 * serial输出“压力，波形\n”
 */
void serial_display_pressure_and_wave(float pres, int wave){
    Serial.print("\n");
    Serial.print(pres);
    Serial.print(",");
    Serial.print(wave);
}

/*
 * 这个函数用来处理波形信号的神秘问题，并且进行缩放，固定在100-150左右
 * 问题：前几秒会变得很大有溢出
 * 把前几秒去掉，前_MAGIC_k个值去掉。
 * 缩小_MAGIC_NUM倍
 */
int pure_magic(int wave, int order){
    int _MAGIC_NUM = 50;
    int temp = 60;
    temp = wave/_MAGIC_NUM;
    if (temp>200){
      return 60;
    }
    else{
      return temp;
    }
}

/*
 * 充气函数
 * 充气保持time毫秒
 */
void inflate_for_x_ms(int time){
    openM1_closeM2();
    delay(time);
}

/*
 * 缓慢放气函数
 */
void deflate(){
    closeM1_closeM2();
}

/*
 * 快速放气函数
 * 快速放气保持time毫秒
 */
void quick_deflate_for_x_ms(int time){
    closeM1_openM2();
    delay(time);
}

/*
 * 打开OUT1和OUT2之间的电机M1，在此项目中为充气电机
 * 关闭OUT3和OUT4之间的电机M2，在此项目中为放气气阀
 */ 
void openM1_closeM2(){
    digitalWrite(drive_input1,HIGH);
    digitalWrite(drive_input2,LOW);
    digitalWrite(drive_input3,LOW);
    digitalWrite(drive_input4,LOW);
}

/*
 * 关闭OUT1和OUT2之间的电机M1，在此项目中为充气电机
 * 关闭OUT3和OUT4之间的电机M2，在此项目中为放气气阀
 */ 
void closeM1_closeM2(){
    digitalWrite(drive_input1,LOW);
    digitalWrite(drive_input2,LOW);
    digitalWrite(drive_input3,LOW);
    digitalWrite(drive_input4,LOW);
}

/*
 * 关闭OUT1和OUT2之间的电机M1，在此项目中为充气电机
 * 打开OUT3和OUT4之间的电机M2，在此项目中为放气气阀
 */ 
void closeM1_openM2(){
    digitalWrite(drive_input1,LOW);
    digitalWrite(drive_input2,LOW);
    digitalWrite(drive_input3,HIGH);
    digitalWrite(drive_input4,LOW);
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

/*
 * 把压力传感器的电压信号转换成以mmHg为单位的压强信号
 * input: float
 * output: float
 */ 
float transfer_v2p(float v){
    float v1 = v/5.0;
    float v2 = v1-0.04;
    float kpa = v2/0.018;
    float mmhg = 7.5*kpa;
    return mmhg;
}

/*
 * 获得输入两个数组：波形数组和压力数组
 * 输出平均压
 */
float average_pressure(float pres[], int wave[], size_t size){
    int max_wave = 0;
    float avg_pres = 0;
    for(int counter=0; counter < size; counter++){
        if(wave[counter]>max_wave){
            max_wave = wave[counter];
            avg_pres = pres[counter];
        }
    }
    return avg_pres;
}


void setup(void) {
    //显示屏的setup
    u8g2.begin(0,23,2,3,4,5);
    //serial的setup
    Serial.begin(115200);
    //ADC的setup
    ads.begin();
    //电机驱动板的setup
    pinMode(drive_input1,OUTPUT);
    pinMode(drive_input2,OUTPUT);
    pinMode(drive_input3,OUTPUT);
    pinMode(drive_input4,OUTPUT);
}

void loop(void){
    // ADC的两个个输入
    int16_t adc_wave, adc_pressure;
    //从adc中读取压力信号
    adc_pressure = ads.readADC_SingleEnded(PRES_SIG);

    /*
     * 定义pres为此时的压强值
     * 显示的数据为
     * “压强值,0\n”
     */ 
    float pres = transfer_v2p(transfer_adc2v(adc_pressure));
    serial_display_pressure_and_wave(pres,0);

    OLED_display_default();
    /*
     * 打气功能
     * 打到TARGET_PRESSURE就停下
     * _STEP_TIME控制每一次打气时间，ms单位
     */
    if(pres<TARGET_PRESSURE){
        inflate_for_x_ms(_STEP_TIME);
    }
    /*
     * 完成打气
     * 并在完成打气之后完成显示功能和记录功能
     */
    else{
        closeM1_closeM2();
        // 获取数据DATA_ARRAY_SIZE次
        int magic_adc_wave;
        for(int counter = 0; counter < DATA_ARRAY_SIZE; counter++){
            int adc_wave = ads.readADC_SingleEnded(WAVE_SIG);
            adc_pressure = ads.readADC_SingleEnded(PRES_SIG);
            pres = transfer_v2p(transfer_adc2v(adc_pressure));
            // 处理原来的脉搏波信号，使其好看一些。
            magic_adc_wave = pure_magic(adc_wave,counter);
            // 把信号存在两个数组中。
            wave_data[counter] = magic_adc_wave;
            pressure_data[counter] = pres;
            //显示 pressure和wave，中间用逗号隔开，显示两条曲线
            serial_display_pressure_and_wave(pres,magic_adc_wave);
            delay(SAMPLING_DELAY);
        }
        float avg_pres = average_pressure(pressure_data, wave_data, DATA_ARRAY_SIZE);
        float fake_sys_pres = avg_pres*1.2;
        float fake_dia_pres = avg_pres*0.7;
        int sys_pres = (int)fake_sys_pres;
        int dia_pres = (int)fake_dia_pres;
        OLED_display(6,30,13,5,sys_pres/2,dia_pres/2);
        quick_deflate_for_x_ms(QUICK_DEF_TIME);
  }
}


