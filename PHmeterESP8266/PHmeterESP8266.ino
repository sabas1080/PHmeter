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
#include <SoftwareSerial.h>

// Create aREST instance
aREST_UI rest = aREST_UI();

SoftwareSerial PhSerial(10, 11); // RX, TX

// WiFi parameters
const char* ssid = "0EF623";
const char* password = "SABAS1080*_";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

// Variables to be exposed to the API
int Ph;
String command;
int indice=0;

void setup()
{
  // Open serial communications with PC:
  Serial.begin(115200);

  Serial.println("Ph Meter Ready!!");

  // set the data rate for the SoftwareSerial port PH Meter
  PhSerial.begin(9600);

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
}

void loop() // run over and over
{
  Ph = 0;
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
  
  if (PhSerial.available()){
    char c = PhSerial.read();
    
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
  
  if (Serial.available())
    PhSerial.write(Serial.read());
}

void parseCommand(String com)
{
  String etiqueta;
  String VPh;
  etiqueta = com.substring(0,com.indexOf(":")+1);
  VPh =com.substring(com.indexOf(":")+1);
  Ph= VPh.toInt();
  Serial.println(etiqueta);
  Serial.println(VPh);
}

