#include <SoftwareSerial.h>

int dataIn = 6; //DATA PIN FOR ARDUINO MICRO
int clockIn = 3;//CLOCK PIN FOR ARDUINO MICRO (this is default)
SoftwareSerial ble(11,12);



int isin=0; //inch = 1 & mm = 0
int isfs=0; //- = 1
int index;

unsigned long xData,oData;

int ledPin = 13; // the number of the LED pin
int ledState = LOW; // ledState used to set the LED
long previousMillis = 0; // will store last time LED was updated
long interval = 500; // interval at which to blink (milliseconds)

long previousGetMillis = 0;
long Timeout = 8; //8ms

void setup(){
  ble.begin(57600);
  digitalWrite(dataIn, 1);
  digitalWrite( clockIn, 1);
  pinMode(dataIn, INPUT);
  pinMode(clockIn, INPUT);
  Serial.begin(9600);
  delay(500);
  attachInterrupt(0,getBit,RISING);
  index =0;
  xData=0;
  oData=999;
}

void loop(){
  if ((index !=0) && (millis() - previousGetMillis > Timeout) ) {
  index=0;
  xData=0;
  };
  if (index >23) {
    if (oData !=xData) {
      if (isfs==1) {
        Serial.print('-');
        ble.print('-');
      }
      if (isin==1){ //reading in inches
        xData *=5;
        Serial.print(xData/10000);
        Serial.print('.');
        ble.print(xData/10000);
        ble.print('.');
          if ((xData % 10000)<1000){
            if ((xData % 10000)<100){
              if ((xData % 10000)<10){
                Serial.print('0');
                ble.print('0');
              };
              Serial.print('0');
              ble.print('0');
            };
            Serial.print('0');
            ble.print('0');
          };
        Serial.println(xData % 10000);
        ble.println(xData % 10000);
      } else { //reading in mm
        Serial.print(xData/100);
        ble.print(xData/100);
        Serial.print('.');
        ble.print('.');
        if ((xData % 100)<10) { 
          Serial.print('0');
          ble.print('0');
        }
        Serial.println(xData % 100);
        ble.println(xData % 100);
      };
    };
    oData =xData;
    index=0;
    xData=0;
  };
  if (millis() - previousMillis > interval) {
    previousMillis = millis();
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      digitalWrite(ledPin, ledState);
  }

}

void getBit(){
  previousGetMillis=millis();
  if(index < 20){
    if(digitalRead(dataIn)==1){
      xData|= 1<<index;
    };
  } else {
  if (index==20) {
    isfs=digitalRead(dataIn);
  }
  if (index==23) {
    isin=digitalRead(dataIn);
  }
  };
  index++;
}

