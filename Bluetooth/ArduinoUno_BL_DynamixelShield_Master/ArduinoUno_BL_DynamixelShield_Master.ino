#include <DynamixelShield.h>
#include <SoftwareSerial.h>

#define rxPin 7
#define txPin 8
char fire_signal=NULL;

SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);


const uint8_t DXL_ID = 1;
const float DXL_PROTOCOL_VERSION = 2.0;
DynamixelShield dxl;
//This namespace is required to use Control table item names
using namespace ControlTableItem;

int sensorPin=A0;
int sensorData=0;


void setup()  {

  pinMode(sensorPin,INPUT);  


  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(57600);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID);


    // Define pin modes for TX and RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    // Set the baud rate for the SoftwareSerial object
    mySerial.begin(9600);
    Serial.begin(9600);

}

void loop() {

  
  sensorData = analogRead(sensorPin);

  if(sensorData>=200)
  {
    fire_signal='t'; 
  } 
  else 
  {
    fire_signal='f';
  }

  if (mySerial.available())
  {
    if('q'==mySerial.read()) mySerial.print(fire_signal);
  }
  else;

}