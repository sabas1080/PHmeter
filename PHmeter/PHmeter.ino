/*
  communications Arduino with PH Meter 1.0 Serial

 Receives from the PC, sends to PH Meter.
 Receives from PH Meter, sends to PC.

 The circuit:
 * RX is digital pin 10 (connect to TX of PH Meter)
 * TX is digital pin 11 (connect to RX of PH Meter)
 * 3.3v of Arduino (connect to 3.3v of PH Meter)
 * 5v of Arduino (connect to 5v of PH Meter)
 * GND of Arduino (connect to GND of PH Meter)
 
 Created by
 Andrés Sabas
 From The Inventor's House Hackerspace
 8 Dec 2015

 This example code is in the public domain.

 */
 
#include <SoftwareSerial.h>

SoftwareSerial PhSerial(10, 11); // RX, TX

void setup()
{
  // Open serial communications with PC:
  Serial.begin(115200);

  Serial.println("Ph Meter Ready!!");

  // set the data rate for the SoftwareSerial port PH Meter
  PhSerial.begin(9600);
}

String command;
int index=0;

void loop() // run over and over
{
  if (PhSerial.available()){
    char c = PhSerial.read();
    
    if(index>=7){
      
      parseCommand(command);
      command = "";
      index=0;
    }
    else{
      command += c;
      index++;
    }
  }
  
  if (Serial.available())
    PhSerial.write(Serial.read());
}

void parseCommand(String com)
{
  String PH;
  String VPh;
  PH = com.substring(0,com.indexOf(":")+1);
  VPh =com.substring(com.indexOf(":")+1);
  Serial.println(PH);
  Serial.println(VPh);
}

