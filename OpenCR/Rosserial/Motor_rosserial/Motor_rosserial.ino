#include <ros.h>
#include <Arduino.h>
#include <pmod_msgs/Position.h>  // Define a custom message for position
#include <pmod_msgs/Motor.h>

pmod_msgs::Position position_msg;
pmod_msgs::Motor motor_msg;

void MotorCallback(const pmod_msgs::Motor& msg) {
  motor_msg = msg;
}
// rosserial
ros::NodeHandle nh;
ros::Publisher position_pub("position", &position_msg);
ros::Subscriber<pmod_msgs::Motor> motor_sub("motor", &MotorCallback); //10hz로 MotorCallback 함수 호출

// 모터 핀 정의
#define RPWM1 6    // 오른쪽 모터 PWM 핀 1 blue
#define RPWM2 5    // 오른쪽 모터 PWM 핀 2
#define LPWM1 9    // 왼쪽 모터 PWM 핀 1 
#define LPWM2 10   // 왼쪽 모터 PWM 핀 2 blue

// 엔코더 핀 정의
const int pinC = 7; // GPIO 7 (INT3) right yellow
const int pinD = 8; // GPIO 8 (INT4)
const int pinA = 2; // GPIO 2 (INT0) left
const int pinB = 3; // GPIO 3 (INT1) yellow

// 엔코더 값

int encoder_count_left = 0;
int encoder_count_right = 0;

// 이전 엔코더 값
int prev_count_left = 0;
int prev_count_right = 0;

// 로봇 파라미터
const float wheel_radius = 94.0; // 바퀴 둘레(cm)
const float wheel_distance = 15.6; // 바퀴 사이 거리 (cm)
const float counts_per_revolution = 360.0; // 엔코더 카운트 수 (한 바퀴당)

// 현재 위치 및 방향
float x = 0; // x 좌표 (cm)
float y = 0; // y 좌표 (cm)
float theta = 0; // 방향 (라디안)


// speed : 모터 신호 값 -100 ~ 100
//           - 음수 : 후진
//           - 0    : 정지
//           - 양수  : 전진
// motor_number : 
//        - 모터 번호 1 : 오른쪽
//        - 모터 번호 2 : 왼쪽
// max_pwm : pwm 최댓값
// min_pwm : pwm 최솟값

void RunMotor(int speed, int motor_number, double max_pwm, double min_pwm)
{

  double unit_value = (max_pwm - min_pwm) / 100.0;

  if((speed > 0) && (motor_number == 1))
  {
    drv_pwm_set_duty(RPWM2,8,0);
    //digitalWrite(RPWM2, LOW);
    //analogWrite(RPWM1, (int)((unit_value*speed)+min_pwm));
    drv_pwm_set_duty(RPWM1, 8, (int)((unit_value*speed)+min_pwm)); // 1. pin number   2. 8(bit) is resolution  3. 0~255 
  }
  else if((speed < 0) && (motor_number == 1))
  {
    drv_pwm_set_duty(RPWM1,8,0);
    //digitalWrite(RPWM1, LOW);
    //analogWrite(RPWM2, (int)((unit_value*(-1.0)*speed)+min_pwm));
    drv_pwm_set_duty(RPWM2, 8, (int)((unit_value*(-1.0)*speed)+min_pwm));
  }
  else if((speed == 0) && (motor_number == 1))
  {
    drv_pwm_set_duty(RPWM1,8,0);
    drv_pwm_set_duty(RPWM2,8,0);    
    //digitalWrite(RPWM1, LOW);
    //digitalWrite(RPWM2, LOW);
  }

  if((speed > 0) && (motor_number == 2))
  {
    drv_pwm_set_duty(LPWM2,8,0);
    //digitalWrite(LPWM2, LOW);
    //analogWrite(LPWM1, (int)((unit_value*speed)+min_pwm));
    drv_pwm_set_duty(LPWM1, 8, (int)((unit_value*speed)+min_pwm));
  }
  else if((speed < 0) && (motor_number == 2))
  {
    drv_pwm_set_duty(LPWM1,8,0);
    //digitalWrite(LPWM1, LOW);
    //analogWrite(LPWM2, (int)((unit_value*(-1.0)*speed)+min_pwm));
    drv_pwm_set_duty(LPWM2, 8, (int)((unit_value*(-1.0)*speed)+min_pwm));
  }
  else if((speed == 0) && (motor_number == 2))
  {
    drv_pwm_set_duty(LPWM1,8,0);
    drv_pwm_set_duty(LPWM2,8,0);    
  }
}

// 엔코더 값을 이용한 오도메트리 업데이트 함수
void updateOdometry() {

  // 왼쪽 모터 엔코더 값 변화
  int delta_count_left = encoder_count_left - prev_count_left;
  prev_count_left = encoder_count_left;

  // 오른쪽 모터 엔코더 값 변화
  int delta_count_right = encoder_count_right - prev_count_right;
  prev_count_right = encoder_count_right;

  // 바퀴 회전 거리 계산
  float delta_distance_left = (delta_count_left / counts_per_revolution) * (wheel_radius);
  float delta_distance_right = (delta_count_right / counts_per_revolution) * (wheel_radius);

  // 로봇 이동 거리 계산
  float delta_distance = (delta_distance_left + delta_distance_right) / 2.0;

  // 로봇 방향 갱신
  float delta_theta = (delta_distance_right - delta_distance_left) / wheel_distance;
  theta += delta_theta;

  // x, y 좌표 갱신
  float delta_x = delta_distance * cos(theta);
  float delta_y = delta_distance * sin(theta);
  x += delta_x;
  y += delta_y;
}


void GetEncoderCountA()
{
  bool pinA_state = digitalRead(pinA);
  bool pinB_state = digitalRead(pinB);

  // 회전 방향 판단 및 엔코더 값 증가
  if (pinA_state == HIGH && pinB_state == LOW) encoder_count_left++;
  else if (pinA_state == LOW && pinB_state == HIGH) encoder_count_left++;
  else if (pinA_state == HIGH && pinB_state == HIGH) encoder_count_left--;
  else if (pinA_state == LOW && pinB_state == LOW) encoder_count_left--;
}

void GetEncoderCountB()
{
  bool pinA_state = digitalRead(pinA);
  bool pinB_state = digitalRead(pinB);

  // 회전 방향 판단 및 엔코더 값 증가
  if (pinA_state == HIGH && pinB_state == HIGH) encoder_count_left++;
  else if (pinA_state == LOW && pinB_state == LOW) encoder_count_left++;
  else if (pinA_state == LOW && pinB_state == HIGH) encoder_count_left--;
  else if (pinA_state == HIGH && pinB_state == LOW) encoder_count_left--;
 }

void GetEncoderCountA_right()
{
  bool pinA_state = digitalRead(pinC);
  bool pinB_state = digitalRead(pinD);

  // 회전 방향 판단 및 엔코더 값 증가
  if (pinA_state == HIGH && pinB_state == LOW) encoder_count_right++;
  else if (pinA_state == LOW && pinB_state == HIGH) encoder_count_right++;
  else if (pinA_state == HIGH && pinB_state == HIGH) encoder_count_right--;
  else if (pinA_state == LOW && pinB_state == LOW) encoder_count_right--;

  // encoder_count_right = encoder_count_right * (-1);
}

void GetEncoderCountB_right()
{
  bool pinA_state = digitalRead(pinC);
  bool pinB_state = digitalRead(pinD);

  // 회전 방향 판단 및 엔코더 값 증가
  if (pinA_state == HIGH && pinB_state == HIGH) encoder_count_right++;
  else if (pinA_state == LOW && pinB_state == LOW) encoder_count_right++;
  else if (pinA_state == LOW && pinB_state == HIGH) encoder_count_right--;
  else if (pinA_state == HIGH && pinB_state == LOW) encoder_count_right--;

  // encoder_count_right = encoder_count_right * (-1);
}


void setup() {
  // 모터 핀모드 설정
  pinMode(RPWM1, OUTPUT);
  pinMode(RPWM2, OUTPUT);
  pinMode(LPWM1, OUTPUT);
  pinMode(LPWM2, OUTPUT);

  // pwm setting
  drv_pwm_set_freq(RPWM1, 20000); // It is constrained to frequencies from 1Hz to 1MHz.
  drv_pwm_setup(RPWM1);
  drv_pwm_set_freq(RPWM2, 20000); 
  drv_pwm_setup(RPWM2);
  drv_pwm_set_freq(LPWM1, 20000);
  drv_pwm_setup(LPWM1);
  drv_pwm_set_freq(LPWM2, 20000); 
  drv_pwm_setup(LPWM2);


  // 엔코더 핀 설정
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinMode(pinC, INPUT);
  pinMode(pinD, INPUT);

  // 인터럽트 설정
  // 0번 인터럽트 사용(GPIO 2)
  // pinA의 값이 변할 경우 GetEncoderCountA() 호출
  attachInterrupt(0, GetEncoderCountA, CHANGE);

  // 인터럽트 설정
  // 1번 인터럽트 사용(GPIO 3)
  // pinB의 값이 변할 경우 GetEncoderCountB() 호출
  attachInterrupt(1, GetEncoderCountB, CHANGE);

  // 인터럽트 설정
  // 3번 인터럽트 사용(GPIO 7)
  // pinC의 값이 변할 경우 GetEncoderCountA_right() 호출
  attachInterrupt(3, GetEncoderCountA_right, CHANGE);

  // 인터럽트 설정
  // 4번 인터럽트 사용(GPIO 8)
  // pinD의 값이 변할 경우 GetEncoderCountB_right() 호출
  attachInterrupt(4, GetEncoderCountB_right, CHANGE);

  // ros setup
  nh.initNode();
  nh.advertise(position_pub);
  nh.subscribe(motor_sub);

  Serial.begin(115200);

  // motor_msg.right=0;
  // motor_msg.left=0;
}

// int a = 0;

void loop() {

  // if(a < 50)
  // {
  //   motor_msg.right = 30;
  //   motor_msg.left = 30;
  // }
  // else if((a>50) && (a < 100))
  // {
  //   motor_msg.right = 0;
  //   motor_msg.left = 0;
  // }
  // else if((a>100) && (a < 150))
  // {
  //   motor_msg.right = -30;
  //   motor_msg.left = -30;
  // }
  // if(a > 150) a = 0;

  // Serial.println(a);
  // Serial.println(motor_msg.right);
  // Serial.println(motor_msg.left);
  // Serial.println(" ");

  RunMotor(motor_msg.right, 1, 255, 80); // 1 right 2 left
  RunMotor(motor_msg.left, 2, 255, 70);
  
  updateOdometry();

  // 오도메트리 정보 출력
  // Serial.print("X: ");
  // Serial.print(x);
  // Serial.print(" cm, Y: ");
  // Serial.print(y);
  // Serial.print(" cm, Theta: ");
  // Serial.println(theta);
  // Serial.println(" radians");

  // 엔코더 카운트 출력
  // Serial.println(encoder_count_left);
  // Serial.println(encoder_count_right);

  // rosserial 값 전송
  position_msg.x = x;
  position_msg.y = y;
  position_msg.theta = theta;
  position_pub.publish(&position_msg);

  // 10ms 딜레이
  delay(100);
  nh.spinOnce();
  // a++;
}