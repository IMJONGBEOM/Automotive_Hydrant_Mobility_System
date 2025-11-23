// 모터 핀 정의
#define RPWM1 6    // 오른쪽 모터 PWM 핀 1 보라
#define RPWM2 5    // 오른쪽 모터 PWM 핀 2 파랑
#define LPWM1 9    // 왼쪽 모터 PWM 핀 1 파랑
#define LPWM2 10   // 왼쪽 모터 PWM 핀 2   보라
// 엔코더 핀 정의
const int pinC = 7; // GPIO 7 (INT3) // RIGHT
const int pinD = 8; // GPIO 8 (INT4) // RIGHT
const int pinA = 2; // GPIO 2 (INT0) 
const int pinB = 3; // GPIO 3 (INT1)

// 엔코더 값
int encoder_count_left = 0;
int encoder_count_right = 0;

// 이전 엔코더 값
int prev_count_left = 0;
int prev_count_right = 0;

// 로봇 파라미터
const float wheel_radius = 3.5; // 바퀴 반지름 (cm)
const float wheel_distance = 10.0; // 바퀴 사이 거리 (cm)
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

  if((speed> 0) && (motor_number == 1))
  {
    digitalWrite(RPWM2, LOW);
    analogWrite(RPWM1, (int)((unit_value*speed)+min_pwm));
  }
  else if((speed < 0) && (motor_number == 1))
  {
    digitalWrite(RPWM1, LOW);
    analogWrite(RPWM2, (int)((unit_value*(-1.0)*speed)+min_pwm));
  }
  else if((speed == 0) && (motor_number == 1))
  {
    digitalWrite(RPWM1, LOW);
    digitalWrite(RPWM2, LOW);
  }

  if((speed > 0) && (motor_number == 2))
  {
    digitalWrite(LPWM2, LOW);
    analogWrite(LPWM1, (int)((unit_value*speed)+min_pwm));
  }
  else if((speed < 0) && (motor_number == 2))
  {
    digitalWrite(LPWM1, LOW);
    analogWrite(LPWM2, (int)((unit_value*(-1.0)*speed)+min_pwm));
  }
  else if((speed == 0) && (motor_number == 2))
  {
    digitalWrite(LPWM1, LOW);
    digitalWrite(LPWM2, LOW);
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
  float delta_distance_left = (delta_count_left / counts_per_revolution) * (2 * 3.14159265 * wheel_radius);
  float delta_distance_right = (delta_count_right / counts_per_revolution) * (2 * 3.14159265 * wheel_radius);

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

  // 엔코더 핀 설정
     pinMode(RPWM1, OUTPUT);
  pinMode(RPWM2, OUTPUT);
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

  // 모터 정지(초기값 세팅)
  analogWrite(RPWM1, 0);
  analogWrite(RPWM2, 0);
  analogWrite(LPWM1, 0);
  analogWrite(LPWM2, 0);

  Serial.begin(115200);
}

void loop() {

  //RunMotor(100, 1, 255, 70); // speed(speed) = -100 ~ 100,  motor_number = 1(오른쪽 모터) 2(왼쪽 모터)
  RunMotor(30, 2, 255, 70);

  updateOdometry();

  // 오도메트리 정보 출력
  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" cm, Y: ");
  Serial.print(y);
  Serial.print(" cm, Theta: ");
  Serial.println(theta);
  //Serial.println(" radians");

  // 엔코더 카운트 출력
  Serial.println(encoder_count_left);
  Serial.println(encoder_count_right);

  // 10ms 딜레이
  delay(10);  
}