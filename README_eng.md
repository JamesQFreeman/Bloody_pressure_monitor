# Bloody_pressure_monitor -- a project of blood pressure monitor
This is a open source project of electronical blood pressure monitor.

## MCU system model
This monitor is running on ESP32.

## Processing Clock Signal (aborted) 
At first, We applied DS1302 as RTC chip and tried many libraries and related codes, but two series of problems happened:

![alt text](images/error1.jpg  "Title")

![alt text](images/error2.jpg  "Title")

In the end, this scheme has been aborted and its job is done by using Internet.

## Inflating & Deflating air
The deflating device has two components. The first one is a fast-vent valve, which only opens after the measurement ended; the second one is a slow-vent valve, which only closes when monitor is inflating air. 

This procedure has 2 steps:
1. Inflating air: we use an air pipe to blow the air into cuff, at the same time, a pressure sensor is monitoring air pressure of the cuff. The MCU will shutt down air pipe after air pressure reached 160mmHg, then, the slow-vent valve will be opened and the MCU will measure blood pressure by using oscillometric method.

2. Deflating air: The fast-vent valve will open after the MCU gained enough datas.

## Blood pressure measurement
The algorithm is Oscillametric method.

![alt text](images/微信图片_20180629215533.jpg  "Title")

![alt text](images/原理.png  "Title")

## Problems
Unfinished...