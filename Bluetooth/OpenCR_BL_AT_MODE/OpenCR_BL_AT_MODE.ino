
void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);   // USB Serial
  SerialBT2.begin(38400);  // UART2 (Blut in uart)
}

void loop() {
  // Keep reading from HC‐06 and send to Arduino Serial Monitor
  if (SerialBT2.available())
  {
  Serial.write(SerialBT2.read());
  }
  // Keep reading from Arduino Serial Monitor and send to HC‐06
  if (Serial.available())
  {
  SerialBT2.write(Serial.read());
  }

}
