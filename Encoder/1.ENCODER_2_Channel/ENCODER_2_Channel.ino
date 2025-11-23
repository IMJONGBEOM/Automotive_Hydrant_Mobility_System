// 설정값
const int pinA = 2; // GPIO 2
const int pinB = 3; // GPIO 3

// 엔코더 값
int encoder_count = 0;

void GetEncoderCountA()
{
  bool pinA_state = digitalRead(pinA);
  bool pinB_state = digitalRead(pinB);

  // 회전 방향 판단 및 엔코더 값 증가
  if (pinA_state == HIGH && pinB_state == LOW) encoder_count++;
  else if (pinA_state == LOW && pinB_state == HIGH) encoder_count++;
  else if (pinA_state == HIGH && pinB_state == HIGH) encoder_count--;
  else if (pinA_state == LOW && pinB_state == LOW) encoder_count--;
}

void GetEncoderCountB()
{
  bool pinA_state = digitalRead(pinA);
  bool pinB_state = digitalRead(pinB);

  // 회전 방향 판단 및 엔코더 값 증가
  if (pinA_state == HIGH && pinB_state == HIGH) encoder_count++;
  else if (pinA_state == LOW && pinB_state == LOW) encoder_count++;
  else if (pinA_state == LOW && pinB_state == HIGH) encoder_count--;
  else if (pinA_state == HIGH && pinB_state == LOW) encoder_count--;
 }

void setup()
{
  // 핀 설정
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);

  // 인터럽트 설정
  // 0번 인터럽트 사용(GPIO 2)
  // pinA의 값이 변할 경우 GetEncoderCountA() 호출
  attachInterrupt(0, GetEncoderCountA, CHANGE);

  // 인터럽트 설정
  // 1번 인터럽트 사용(GPIO 3)
  // pinB의 값이 변할 경우 GetEncoderCountB() 호출
  attachInterrupt(1, GetEncoderCountB, CHANGE);
  
  // 시리얼 통신 초기화
  Serial.begin(115200);
}

void loop()
{
  // 엔코더 카운트 출력
  Serial.println(encoder_count);

  // 10ms 딜레이
  delay(10);  
}