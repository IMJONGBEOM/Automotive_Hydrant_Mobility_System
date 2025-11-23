#include <DynamixelShield.h>
#include <actuator.h>  
#if defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_MEGA2560)
#elif defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAM_ZERO)
#define DEBUG_SERIAL SerialUSB
#else
#define DEBUG_SERIAL Serial
#endif

#define DXL_3 3                     // 문 개방 모터
#define DXL_4 4                   // 소화전 벨브 개방 모터
#define DXL_PROTOCOL_VERSION_1_0 1.0  // 프로토콜 1.0 정의

DynamixelShield dxl;

////////////////////////////////////////////////////

#include <SoftwareSerial.h>
#define rxPin 7
#define txPin 8
char fire_signal=NULL;    // Gas Sensor flag
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

int sensorPin=A0;     // MQ-6 (Gas Sensor)
int sensorData=0;

////////////////////////////////////////////////////

bool motor1Act = false;  // 모터 1 활성화 상태 변수
bool motor2Act = false;  // 모터 2 활성화 상태 변수

void setup() {

  dxl.begin(1000000);  // 모터 통신 속도 설정 (1Mbps)
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION_1_0);  // 프로토콜 버전 설정 (1.0)

  //문 개방 모터
  dxl.torqueOff(DXL_3);
  dxl.setOperatingMode(DXL_3, OP_POSITION);
  dxl.torqueOn(DXL_3);

  //소화전 벨브 개방 모터
  dxl.torqueOff(DXL_4);
  dxl.setOperatingMode(DXL_4, OP_VELOCITY);
  dxl.torqueOn(DXL_4);

  // Define pin modes for TX and RX
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // Set the baud rate for the SoftwareSerial object
  mySerial.begin(9600);
  
}

void loop() {

  sensorData = analogRead(sensorPin);
  if(sensorData>=200)
  {
    fire_signal='t';    //  Gas Sensor fire ture
  } 
  else 
  {
    fire_signal='f';
  }

  if (mySerial.available())
  {
    if('q'==mySerial.read()) // OpenCR에서 'q'를 전송받을때만 현재 화재 상태 값 출력 ( 연송 송수신 방지)
    {
      mySerial.print(fire_signal);

      if (fire_signal=='t') // 화재상태일때 소화전 밸브를 개방하도록 동작
      {
        dxl.torqueOn(DXL_3);
        dxl.setGoalPosition(DXL_3, 120, UNIT_DEGREE);                                   // setgoalpostion은 고정적인 값이므로 건들면 안됨.
        delay(1000);

        dxl.setGoalPosition(DXL_3, 360, UNIT_DEGREE);
        delay(1000);

        // 소화전 벨브 개방 모터 제어 코드 (휠 모드) -- 열림
        dxl.setGoalVelocity(DXL_4, 2000);  // RPM, 최대속도 2047
        delay(8400);                          // 충분한 시간 대기
        dxl.setGoalVelocity(DXL_4, 0);     // 모터 정지 (속도 0으로 설정)

        delay(60000); // 로봇이 동작할때 까지 대기

        // 소화전 벨브 개방 모터 제어 코드 (휠 모드) -- 닫힘
        dxl.setGoalVelocity(DXL_4, 977);  // RPM, 최대속도 2047
        delay(8400);                          // 충분한 시간 대기
        dxl.setGoalVelocity(DXL_4, 0);     // 모터 정지 (속도 0으로 설정)
      }
      else if(fire_signal=='f') // 화재 상태가 아닐 경우 모터를 정지하고 대기하도록 동작
      {
        // 소화전 벨브 개방 모터 제어 코드 (휠 모드) -- 닫힘
        dxl.setGoalVelocity(DXL_4, 977);  // RPM, 최대속도 2047
        delay(8400);                          // 충분한 시간 대기
        dxl.setGoalVelocity(DXL_4, 0);     // 모터 정지 (속도 0으로 설정)
      }
      else;

    }
    else;
  }
  else;
  
}