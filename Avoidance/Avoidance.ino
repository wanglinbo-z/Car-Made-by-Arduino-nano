/*
名称：小车控制
作者：王淋博
时间：2024年1月
描述：电机控制
      红外避障
*/

// 定义电机驱动板引脚
// 右轮
const int AIN1 = 9; // 连接到TB6612的AIN1引脚
const int AIN2 = 10; // 连接到TB6612的AIN2引脚
const int PWMA = 11; // 连接到TB6612的PWMA引脚
// 左轮
const int BIN1 = 6; // 连接到TB6612的BIN1引脚
const int BIN2 = 5; // 连接到TB6612的BIN2引脚
const int PWMB = 3; // 连接到TB6612的PWMB引脚


// 红外避障模块引脚定义
const int leftObstaclePin = A0;
const int rightObstaclePin = A1;


void setup (){
  Serial.begin(9600);

  // 设置红外引脚为输入模式
  // 避障
  pinMode(leftObstaclePin, INPUT);
  pinMode(rightObstaclePin, INPUT);
  
  // 设置电机驱动板引脚为输出模式
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  Serial.println("Arduino is readly!");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(800);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}

void loop(){
  while (Avoidance()){

  }
}

// 避障检测
bool Avoidance(){
  int leftSensorValue = digitalRead(leftObstaclePin);
  int rightSensorValue = digitalRead(rightObstaclePin);
  if (leftSensorValue == LOW && rightSensorValue == LOW) {
    // 左右传感器都检测到障碍物
    DJ_reset();
    Serial.println("障碍物在左右两侧");
    return 1;
  } else if (leftSensorValue == LOW) {
    // 只有左侧传感器检测到障碍物
    Serial.println("障碍物在左侧");
    right();
    return 1;
  } else if (rightSensorValue == LOW) {
    // 只有右侧传感器检测到障碍物
    Serial.println("障碍物在右侧");
    left();
    return 1;
  } else {
    // 没有检测到障碍物
    march();
    return 1;
    Serial.println("无障碍物");
  }
  delay(100); // 延迟以稳定读数
}

// 重置电机
void DJ_reset(){
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
}

// 前进
void march(){

  Serial.print("\nMove Foword");
  // 电机1前进
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  for (int i = 0;i <+ 128;i +=5){
    analogWrite(PWMA, i);
  }

  // 电机2前进
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  for (int i = 0;i <= 128;i += 5){
    analogWrite(PWMB, i);
  }
  delay(10);
}

// 左后退
void left_back(){

  Serial.print("\nMove Back");
  // 电机1后退
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  for (int i = 0;i <+ 128;i +=20){
    analogWrite(PWMA, i);
  }

  // 电机2后退
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  for (int i = 0;i <= 90;i += 5){
    analogWrite(PWMB, i);
  }
  delay(10);
}

// 左转
void left(){

  Serial.print("\nTurn Left");
  // 电机1全速前进
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 128);
  // 电机2后退
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 48);

  delay(10);
}

// 右转
void right(){

  Serial.print("\nTurn Right");
  // 电机1后退
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 48);
  // 电机2全速前进
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 128);

  delay(10);
}
