/*
 * author: jieliang mo
 * github: https://github.com/mworkfun/wireless.git
 * product link: http://www.fengniaorf.com/
 * This library use for far-T2 and far-R1
 * communication protocol: EV1527
 * this library use the timer1
 * version: 1.0.0
 * date: 2020-6-9
 */
 
#include<Wireless.h>

int tPin = 6;        //transmitter use pin6
int T = 0;           //1 is receive mode, 0 is transmission mode
long tAddress = 0xff0ff; //address of transmitter(0---0xfffff)

wireless T2(tAddress, T); //create a send class

void setup() {
  Serial.begin(9600);
  T2.transmitInit(tPin);  //define the pin of transmitter.
}

void loop() {
  T2.sendData(0x0A);  //The range of data sent is: 0x00 -- 0x0f
  delay(1000);
}
