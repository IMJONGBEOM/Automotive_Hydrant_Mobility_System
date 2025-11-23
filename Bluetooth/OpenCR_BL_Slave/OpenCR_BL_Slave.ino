
char fire_signal=NULL;
int q_flag=0;   //  0: not   5 : serrail ready

void fire(void)
{
  Serial.println("fire");
  delay(4000);
}

void jo(void)
{
  Serial.println("jo");
  delay(4000);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);   // USB Serial
  SerialBT2.begin(9600);  // UART2 (Blut in uart)

}


void loop() {

  if(q_flag==0)
  {
    SerialBT2.print('q'); // 'q' 를 소화전 Arduino Uno에 전송
    q_flag=1;             // OpenCR  동작 끝내기 전까지 통신금지 플래그(연송 송수신 방지)
  }
  else;

  if((SerialBT2.available()>0)&&(q_flag==1)) // OpenCR이 수신 준비상태 일때
  {
    fire_signal=SerialBT2.read();
    //Serial.println(fire_signal);
    if(fire_signal=='t') fire();
    else if(fire_signal=='f') jo();
    else;
    q_flag=0;
  }
  else;


}
