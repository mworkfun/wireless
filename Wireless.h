/*
 * author: jieliang mo
 * github: https://github.com/mworkfun
 * product link: https://github.com/mworkfun/wireless.git
 * This library use for far-T2 and far-R1
 * communication protocol: EV1527
 * this library use the timer1
 * version: 1.0.0
 * date: 2020-6-9
 */

#ifndef WIRELESS_H_
#define WIRELESS_H_ 
#include <Arduino.h>

#define start_us_min 160
#define start_us_max 200
#define num0_us_min 10
#define num0_us_max 20
#define num1_us_min 0
#define num1_us_max 8

void softDecode(void);

class wireless
{
  public:
  wireless(long address, int mode); //transmit: mode = 0
                                    //receive:  mode = 1
  void transmitInit(int pin);
  void receiveInit(int pin);
  
  void sendData(int dat);
  int receiveData(void);

  void TranSynchronousCode(void);
  void Bit(int b);
  
  private:
    int tPin;
    long transmitAddress;
    long receiveAddress;
};

#endif
