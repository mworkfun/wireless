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

int R = 1;           //1 is receive mode, 0 is transmission mode
int Pin = 5;         // pin of receiver
long rAddress = 0xff0ff; //address of receiver(0---0xfffff)

wireless R1(rAddress, R);  //create a recieve class.

void setup() {
  Serial.begin(9600);
  R1.receiveInit(Pin);  //initialize the receiver.
}

void loop() {
  int dat = 0;
  dat = R1.receiveData();  //The range of data sent is: 0x00 -- 0x0f
  Serial.println(dat);     //print the received data.
  delay(1000);
}
