
#include <SoftwareSerial.h>

#define rxPin 7
#define txPin 8
// Set up a new SoftwareSerial object
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup()  {

    // Define pin modes for TX and RX
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
    Serial.begin(9600);
    // Set the baud rate for the SoftwareSerial object
    mySerial.begin(9600);

}

void loop() {
  // Keep reading from HC‐06 and send to Arduino Serial Monitor
  if (mySerial.available())
  {
  Serial.write(mySerial.read());
  }
  // Keep reading from Arduino Serial Monitor and send to HC‐06
  if (Serial.available())
  {
  mySerial.write(Serial.read());
  }

}