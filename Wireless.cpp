#include"Wireless.h"
int rPin;
boolean IRover = false;
int Low = 0;
int LevelFlag;
int startFlag = 0;
int bitCnt,IRkey;
unsigned long IRdata;
unsigned long tempAddr;

ISR(TIMER1_COMPA_vect){    //Interrupt function of timer1. Record the time every 50 microseconds 
  if(digitalRead(rPin) == LOW){   //Record the time at low levels.
    Low += 1;
    if(LevelFlag == 1) 
      LevelFlag = 0;
  }
  else if(digitalRead(rPin) == HIGH){  //Data processing at high levels 
    if(LevelFlag == 0){
      LevelFlag = 1;
      softDecode();
      Low = 0;
    }
  }
}

/////////////////////////////////////////////////////
void softDecode(){
  if(startFlag == 0){
    if((start_us_min < Low) && (Low < start_us_max)){  //Judge guidance signal
      startFlag = 1;
      bitCnt = 0;
      IRdata = 0;
    }
  }
  else if((startFlag==1) && (bitCnt<24)){
	//Determine the Numbers 0 and 1 by comparing the time of the low level.
    if((num0_us_min<Low) && (Low<num0_us_max)){  //judge the number 0
      IRdata = IRdata<<1;
      bitCnt +=1;
	  //Serial.println(0);
    }
    else if((num1_us_min<Low) && (Low<num1_us_max)){  //judge the number 1
      IRdata = IRdata<<1;
      IRdata |=1;
      bitCnt +=1;
	  //Serial.println(1);
    }
    else{
      startFlag = 0;
      bitCnt = 0;
    }
  }
  if(bitCnt == 24){  //processing data
    bitCnt = 0;
    startFlag = 0;
    IRkey = IRdata&0x0000000f;  //The key value
    tempAddr = (IRdata>>4) & 0x000fffff;  //The address code received
    IRover = true;
  }
}

/////////////////////////////////////////////////////
int wireless::receiveData(void){
  if(IRover == true){
	IRover = false;
	if(tempAddr == receiveAddress)   //check address code
		return IRkey;                //return the key value
	else
		return -1;
  }
  else
    return 0;
}

/////////////////////////////////////////////////////
wireless::wireless(long address, int mode){
  if(mode == 0){
    transmitAddress = address;
    }
  if(mode == 1){
    receiveAddress = address;
    }
}

/////////////////////////////////////////////////////
void wireless::receiveInit(int pin){
  rPin = pin;
  pinMode(rPin, INPUT);

  //Set the frequency of timer1 to 20kHz.
  TCCR1A = 0;  //Set the TCCR1A to 0.
  TCCR1B = 0;  //Set the TCCR1B to 0.
  TCNT1  = 0;  //Initialize the TCNT1 counter value to 0.
  //Set the counter to 20KHz(50us).
  OCR1A = 99;  // OCR1A = (16*10^6)/(1000*8) - 1 (must be <65536)
  TCCR1B |= (1 << WGM12);  //open CTC mode
  TCCR1B |= (1 << CS11);   //Set CS11 to 1 (CLKio/8)
  TIMSK1 |= (1 << OCIE1A);
}

/////////////////////////////////////////////////////
void wireless::transmitInit(int pin){
  tPin = pin;
  pinMode(tPin, OUTPUT);
}

/////////////////////////////////////////////////////
void wireless::TranSynchronousCode(void){
  digitalWrite(tPin, HIGH);
  delayMicroseconds(400);   
  digitalWrite(tPin, LOW);
  delay(9); 
}

/////////////////////////////////////////////////////
void wireless::Bit(int b){
  if(b == 0){
    digitalWrite(tPin, HIGH);
    delayMicroseconds(400);
    digitalWrite(tPin, LOW);
    delayMicroseconds(800);
  }
  if(b == 1){
    digitalWrite(tPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(tPin, LOW);
    delayMicroseconds(200);    
  }
}

/////////////////////////////////////////////////////
void wireless::sendData(int data){
  long tempAddress = transmitAddress;
  TranSynchronousCode();
  for(char i=0; i<20; i++){       //transmit address
    if((tempAddress & 0x80000) == 0x80000){
      Bit(1);
    }
    else{
      Bit(0);
    }
    tempAddress = tempAddress << 1;
  }
  for(char i=0; i<4; i++){       //transmit the key value
    if((data & 0x08) == 0x08){
      Bit(1);
    }
    else{
      Bit(0);
    }
    data = data << 1;
  }
  //It is necessary to emit an end pulse, 
  //otherwise the low level of the last bit of the key value will fail.
  digitalWrite(tPin, HIGH);  
  delayMicroseconds(200);
  digitalWrite(tPin, LOW);
}
