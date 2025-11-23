#include <Dynamixel2Arduino.h>  // 다이나믹셀 헤더파일

#if defined(ARDUINO_OpenCR)
//#define PI 3 <- 다음은 매크로의 예재로 같은 값을 여러번 사용할 때 사용함, 이는 PI를 3으로 대체하여 사용함
#define DXL_SERIAL Serial3    // DXL_SERIAL 매크로를 정의하고, 이를 Serial3로 설정합니다. 이것은 OpenCR 보드의 하드웨어 시리얼 통신 포트를 나타냅니다. Serial3는 OpenCR 보드의 세 번째 하드웨어 시리얼 포트를 가리킵니다.
// #define DEBUG_SERIAL Serial1  // DEBUG_SERIAL 매크로를 정의하고, 이를 Serial1로 설정합니다. 이것은 OpenCR 보드의 디버깅을 위한 하드웨어 시리얼 포트를 나타냅니다. 주석에는 해당 시리얼 포트의 핀 번호가 주석으로 표시되어 있습니다.tx0 rx1
#define DEBUG_SERIAL2 Serial  // OpenCR 보드의 USB 연결을 통한 시리얼 통신을 나타냅니다. pc와의 연결을 통해 확인해보기 위해서. 대회 때는 사용하지 않아도 됨
const int DXL_DIR_PIN = 84;   // OpenCR Board's DIR PIN.
const int DXL_DIR_PIN2 = 85;
const int DXL_DIR_PIN3 = 83;
#endif


#define DXL_46 46  //  집게
#define DXL_45 45  // 5번 관절
#define DXL_44 44  // 4번 관절
#define DXL_43 43  // 3번 관절
#define DXL_42 42  // 2번 관절
#define DXL_41 41  // 1번 관절(하단)

#define DXL_11 11  // 왼쪽 게이트
#define DXL_12 12  // 오른쪽 게이트
#define DXL_13 13  // 왼쪽 게이트 톱니바퀴
#define DXL_14 14  // 오른쪽 게이트 톱니바퀴

#define DXL_1 1  // 관창 상하조절부

// 모터 핀 정의
#define RPWM1 5   // 오른쪽 모터 PWM 핀 1
#define RPWM2 6   // 오른쪽 모터 PWM 핀 2
#define LPWM1 9   // 왼쪽 모터 PWM 핀 1
#define LPWM2 10  // 왼쪽 모터 PWM 핀 2

#define DXL_PROTOCOL_VERSION_1_0 1.0  // 프로토콜 1.0 정의
#define DXL_PROTOCOL_VERSION_2_0 2.0  // 프로토콜 2.0 정의 

const int DXL_ARM[] = { DXL_41, DXL_42, DXL_43, DXL_44, DXL_45, DXL_46 };  // 로봇팔의 다이나믹셀 배열 정의
const int DXL_ASS[] = { DXL_11, DXL_12, DXL_13, DXL_14 };                  // 관창 장착부의 다이나믹셀 배열 정의
const int WHEEL[] = { RPWM1, RPWM2, LPWM1, LPWM2 };

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);  // 다이나믹셀의 핀을 opencr과 통신하기 위한 정의 구문
using namespace ControlTableItem;                // c++를 간편하게 사용하기 위한 구문

void ready(){
  Serial.println("jo");
   // 프로그램이 시작되면 관창 제어부가 열리고, 로봇팔이 뒤로 돌아가고, 분사량 제어부가 원위치로 감
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION_1_0);  // 프로토콜 1.0 선택

  dxl.setGoalPosition(DXL_12, 35, UNIT_DEGREE);  // 관창 제어부가 열리는 부분
  dxl.setGoalPosition(DXL_11, 245, UNIT_DEGREE); // 이 부분에서 빠르게 관창을 원위치 시켜야함
  delay(2000);

  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION_2_0);  // 프로토콜 2.0 선택

  // 로봇팔이 관창을 잡기 위하여 뒤로 돌아가는 부분
  dxl.setGoalPosition(DXL_41, 170, UNIT_DEGREE); 
  delay(300);
  dxl.setGoalPosition(DXL_42, 10, UNIT_DEGREE); 
  delay(300);
  dxl.setGoalPosition(DXL_43, 270, UNIT_DEGREE); 
  delay(300);
  dxl.setGoalPosition(DXL_44, 200, UNIT_DEGREE);
  delay(300);
  dxl.setGoalPosition(DXL_45, 30, UNIT_DEGREE); 
  delay(300);

  // 분사량 제어부가 원위치로 돌아가는 부분
  dxl.setGoalPosition(DXL_13, 355, UNIT_DEGREE); 
  delay(6000); // 관창을 원위치 시키기 위하여 기다리는 부분
}

void start(){
  Serial.println("sex");
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION_2_0);  // 프로토콜 2.0 선택

  dxl.setGoalPosition(DXL_41, 170, UNIT_DEGREE); 
  delay(300);
  dxl.setGoalPosition(DXL_42, 10, UNIT_DEGREE); 
  delay(300);
  dxl.setGoalPosition(DXL_43, 270, UNIT_DEGREE); 
  delay(300);
  dxl.setGoalPosition(DXL_44, 200, UNIT_DEGREE);
  delay(300);
  dxl.setGoalPosition(DXL_45, 30, UNIT_DEGREE); 
  delay(2300);

  dxl.setGoalPosition(DXL_46, 300 , UNIT_DEGREE);  // 집게를 최대로 벌림(프로그램이 시작된다는 뜻)
  delay(1000);
  // 관창을 잡는 위치까지 로봇팔이 이동함
  dxl.setGoalPosition(DXL_44, 120, UNIT_DEGREE);
  dxl.setGoalPosition(DXL_43, 215, UNIT_DEGREE); 
  dxl.setGoalPosition(DXL_42, 115, UNIT_DEGREE); 
  dxl.setGoalPosition(DXL_41, 165, UNIT_DEGREE); 
  delay(3000);
  dxl.setGoalPosition(DXL_46, 345, UNIT_DEGREE);  // 관창을 꽉 잡음
  delay(2000);
  // 로봇팔이 소화전안에서 밖으로 빠지는 부분
  dxl.setGoalPosition(DXL_41, 165, UNIT_DEGREE); 
  dxl.setGoalPosition(DXL_42, 75, UNIT_DEGREE); 
  dxl.setGoalPosition(DXL_43, 212, UNIT_DEGREE); 
  dxl.setGoalPosition(DXL_44, 180, UNIT_DEGREE);
  delay(3000);

  dxl.setGoalPosition(DXL_45, 220, UNIT_DEGREE); // 관창을 결합하기 위하여 45번 모터 180도 회전
  delay(3000);
 
  dxl.setGoalPosition(DXL_42, 5, UNIT_DEGREE); 
  dxl.setGoalPosition(DXL_43, 275, UNIT_DEGREE); 
  dxl.setGoalPosition(DXL_44, 200, UNIT_DEGREE);

  dxl.setGoalPosition(DXL_41, 90, UNIT_DEGREE);
  delay(2000);
  dxl.setGoalPosition(DXL_45, 110, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(2000);
  
  // 로봇팔을 세밀하게 조정하기 전 최종 모습
  dxl.setGoalPosition(DXL_42, 73, UNIT_DEGREE);  // 보라섹 다이나믹셀 어깨
  dxl.setGoalPosition(DXL_43, 280, UNIT_DEGREE);  // 관절 팔꿈치
  dxl.setGoalPosition(DXL_44, 192, UNIT_DEGREE);  // 관절 손목
  dxl.setGoalPosition(DXL_45, 110, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(2000);

  // 세밀하게 관창제어부로 진입하는 모습
  dxl.setGoalPosition(DXL_45, 140, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_41, 70, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(2000);
  dxl.setGoalPosition(DXL_45, 130, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_41, 60, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(1500);
  dxl.setGoalPosition(DXL_45, 120, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_41, 50, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(1500);
  dxl.setGoalPosition(DXL_45, 115, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_41, 45, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(1500);
  dxl.setGoalPosition(DXL_45, 105, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_41, 40, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(1500);
  dxl.setGoalPosition(DXL_45, 100, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_41, 35, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(1500);
  dxl.setGoalPosition(DXL_45, 85, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_41, 30, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(1500);
  dxl.setGoalPosition(DXL_45, 75, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_41, 25, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(1500);
  dxl.setGoalPosition(DXL_45, 70, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(1500);

  // 로봇팔을 관창제어부에 맞게 최종 조정하는 부분
  dxl.setGoalPosition(DXL_41, 18, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_42, 123, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_43, 208, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_44, 215, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  dxl.setGoalPosition(DXL_45, 63, UNIT_DEGREE);  // 손목 회전 다이나믹셀
  delay(2000);

  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION_1_0);  // 프로토콜 1.0 선택

  dxl.setGoalPosition(DXL_12, 245, UNIT_DEGREE);  // 관창 장착부가 관창을 꽉 잡는 구문
  dxl.setGoalPosition(DXL_11, 50 , UNIT_DEGREE);
  delay(3000);

  // 카메라를 전방으로 향하게 하기 위하여 로봇팔을 고정시킴
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION_2_0);  // 프로토콜 2.0 선택

  dxl.setGoalPosition(DXL_46, 300 , UNIT_DEGREE);  // 집게를 최대로 벌림
  delay(2000);
  
  dxl.setGoalPosition(DXL_42, 100, UNIT_DEGREE); // 카메라를 회전시키기 위하여 로봇팔을 기상시키는 부분
  dxl.setGoalPosition(DXL_43, 240, UNIT_DEGREE);
  delay(1000);
  
  dxl.setGoalPosition(DXL_42, 40, UNIT_DEGREE); // 카메라를 정면을 향하게 하기 위하여 로봇팔을 정면을 향하게 하는 부분
  delay(2000);
  dxl.setGoalPosition(DXL_41, 350, UNIT_DEGREE); 
  delay(300);
  dxl.setGoalPosition(DXL_42, 10, UNIT_DEGREE); 
  delay(300);
  dxl.setGoalPosition(DXL_43, 270, UNIT_DEGREE); 
  delay(300);
  dxl.setGoalPosition(DXL_44, 190, UNIT_DEGREE); 
  delay(300);
  dxl.setGoalPosition(DXL_45, 30, UNIT_DEGREE); 
  delay(300);
  dxl.setGoalPosition(DXL_46, 350, UNIT_DEGREE);  // 관창을 꽉 잡음
  delay(3000);

  // 관창이 장착되었음. (소화전으로 이동 가능)

  // 이동하면서 분사량 조절부가 기능하는 부분.
  dxl.setGoalPosition(DXL_13, 50, UNIT_DEGREE); 
  delay(2000);
  dxl.setGoalPosition(DXL_13, 350, UNIT_DEGREE); 
  delay(3000);
}


char fire_signal=NULL;
int q_flag=0;   


void setup() {
  // put your setup code here, to run once:
  SerialBT2.begin(9600);  // UART2 (Blut in uart)

  DEBUG_SERIAL2.begin(9600); // USB Serial 

  dxl.begin(1000000);                                    // opencr과 다이나믹셀 사이의 통신 속도를 1M으로 설정
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION_2_0);  // 초기 프로토콜 2.0 선택
  //------------------------------------------------------------------------------------------------------------------------------------------------
  //다이나믹셀 설정
  //관절, 집게 설정
  for (int i = 0; i < 6; i++) {
    dxl.torqueOff(DXL_ARM[i]);
    dxl.setOperatingMode(DXL_ARM[i], OP_POSITION);
    dxl.torqueOn(DXL_ARM[i]);
    dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ARM[i], 1500);  // 원하는 속도 값 설정
  }
  // 왜 45번만 낮은 값에서 느리게 동작하는지 모르겠지만 문제 해결한 코드
  // 나머지는 높은 값에서 느리게 동작함
  dxl.torqueOff(DXL_45);
  dxl.setOperatingMode(DXL_45, OP_POSITION);
  dxl.torqueOn(DXL_45);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_45, 50);  // 원하는 속도 값 설정

  dxl.torqueOff(DXL_46);
  dxl.setOperatingMode(DXL_46, OP_POSITION);
  dxl.torqueOn(DXL_46);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_46, 50); 

  // 관창 장착부 초기 설정
  // 여기도 마찬가지, 낮은 값에서 느리게 동작함
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION_1_0);  // 프로토콜 2.0 선택

  dxl.torqueOff(DXL_11); 
  dxl.setOperatingMode(DXL_11, OP_POSITION);
  dxl.torqueOn(DXL_11);
  dxl.writeControlTableItem(MOVING_SPEED, DXL_11, 180); 

  dxl.torqueOff(DXL_12);
  dxl.setOperatingMode(DXL_12, OP_POSITION);
  dxl.torqueOn(DXL_12);
  dxl.writeControlTableItem(MOVING_SPEED, DXL_12, 180); 

  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION_2_0);  // 프로토콜 2.0 선택

  for (int i = 2; i < 4; i++) {
    dxl.torqueOff(DXL_ASS[i]);
    dxl.setOperatingMode(DXL_ASS[i], OP_POSITION);  // op position모드로 설정
    dxl.torqueOn(DXL_ASS[i]);
    dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ASS[i], 50);  //position 속도 설정,
  } 

  // 물 분사 각도조절부 초기 설정
  dxl.torqueOff(DXL_1);
  dxl.setOperatingMode(DXL_1, OP_POSITION);  // op position모드로 설정
  dxl.torqueOn(DXL_1);
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_1, 20);  //position 속도 설정,
  //-----------------------------------------------------------------------------------------------------------------------------------------------------
  ready();
  start();
  delay(2000);
}


void loop() {

  if(q_flag==0)
  {
    SerialBT2.print('q');     // 'q' 를 소화전 Arduino Uno에 전송
    q_flag=1;     // OpenCR  동작 끝내기 전까지 통신금지 플래그(연송 송수신 방지)
  }
  else;

  if((SerialBT2.available()>0)&&(q_flag==1)) // OpenCR이 수신 준비상태 일때
  {
    fire_signal=SerialBT2.read();
    //Serial.println(fire_signal);
    if(fire_signal=='t') start();       // 로봇팔이 소화전에서 관창을 끌고와서 로봇에 장착시키는 동작
    else if(fire_signal=='f') ready();  // 로봇팔의 평상시 동작이며 앞을 향해 보고있는 동작
    else;
    q_flag=0;
  }
  else;


}
