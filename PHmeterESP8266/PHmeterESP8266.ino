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
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>
//#include <SoftwareSerial.h>

// Create aREST instance
aREST_UI rest = aREST_UI();

//SoftwareSerial PhSerial(13, 12); // RX, TX

// WiFi parameters
const char* ssid = "513570";
const char* password = "H21208269B2B";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
float Ph=0;
String command;
int indice=0;

void setup()
{
  //Change Serial Pins GPIO 15 and GPIO3
  //Serial.swap()
  
  // Open serial communications with PC:
  Serial.begin(9600);

  //Serial1.begin(115200);
  //Serial1.setDebugOutput(true);

  //Serial.println("Ph Meter Ready!!");

  // set the data rate for the SoftwareSerial port PH Meter
  //PhSerial.begin(9600);

  // Create UI
  rest.title("Ph Meter 1.0v");
  
   // Init variables and expose them to REST API
  Ph = 0;
  rest.variable("Ph",&Ph);
  
  // Labels
  rest.label("Ph");

  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
  Serial.flush(); 
  //delay(2000);

  //Change Serial Pins GPIO15(TX) and GPIO13(RX) communications with Ph Sensor:
  Serial.swap();
  //delay(100); 
  
}

void loop() // run over and over
{
  
  Serial.write("P");
  
  if (Serial.available()){
    char c = Serial.read();
    
    if(indice>=7){
      
      parseCommand(command);
      command = "";
      indice=0;
    }
    else{
      command += c;
      indice++;
    }
  }

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
}

void parseCommand(String com)
{
  String etiqueta;
  String VPh;
  etiqueta = com.substring(0,com.indexOf(":")+1);
  VPh =com.substring(com.indexOf(":")+1);
  Ph= VPh.toFloat();
}

