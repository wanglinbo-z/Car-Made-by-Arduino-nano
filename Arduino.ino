/*
名称：小车控制
作者：王淋博
时间：2024年1月
描述：包含蓝牙控制：
        w：前进，a：左转，d：右转，s：后退
      
*/

// NewPing 超声波测距库
#include <NewPing.h>

// HC-05 蓝牙控制库
#include <SoftwareSerial.h>

// 舵机控制
#include <Servo.h>

// 定义电机驱动板引脚
// 右轮
const int AIN1 = 9; // 连接到TB6612的AIN1引脚
const int AIN2 = 10; // 连接到TB6612的AIN2引脚
const int PWMA = 11; // 连接到TB6612的PWMA引脚
// 左轮
const int BIN1 = 6; // 连接到TB6612的BIN1引脚
const int BIN2 = 5; // 连接到TB6612的BIN2引脚
const int PWMB = 3; // 连接到TB6612的PWMB引脚

// 超声波模块引脚
const int trigPin = 12; // 超声波模块的Trig引脚连接到Arduino的数字引脚12
const int echoPin = 13; // 超声波模块的Echo引脚连接到Arduino的数字引脚13
const int maxDistance = 200;

NewPing sonar(trigPin, echoPin, maxDistance);// 创建NewPing对象

// 红外避障模块引脚定义
const int leftObstaclePin = A0;
const int rightObstaclePin = A1;

// 红外循迹模块引脚定义
const int leftTrackPin = 8;
const int rightTrackPin = 7;

// 舵机相关
Servo servo; // 创建舵机对象
const int servoPin = A4; // 舵机的信号线连接到Arduino的模拟引脚A4

// 定义蓝牙控制引脚
SoftwareSerial BlueTooth (1, 0); // 连接到Rx , Tx引脚

void setup() {
  // 开放串口9600波特率
  Serial.begin(9600);

  // 设置蓝牙串口
  BlueTooth.begin(9600);

  // 设置电机驱动板引脚为输出模式
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  
  // 设置红外引脚为输入模式
  pinMode(leftObstaclePin, INPUT);
  pinMode(rightObstaclePin, INPUT);
  pinMode(leftTrackPin, INPUT);
  pinMode(rightTrackPin, INPUT);

  // 舵机
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(servoPin); // 附加舵机到指定的引脚
  servo.write(90); // 舵机初始位置
  delay(2000);
}

void loop() {
  // while (Serial.available()){
  //   // 蓝牙控制
  //   bluetooth(); 
  //   delay(500);
  //   // 重置电机状态
  //   digitalWrite(AIN1, LOW);
  //   digitalWrite(AIN2, LOW);
  //   digitalWrite(BIN1, LOW);
  //   digitalWrite(BIN2, LOW);
  // }
  if (digitalRead(leftTrackPin) == HIGH){
      Serial.print("leftTrackPin: HIGH\n");
  }
  else {
      Serial.print("leftTrackPin: LOW\n");
  }
  delay(500);
}

// 超声波
void Ultrasound(){
  for (int angle = 0; angle <= 180; angle += 30) {
    // 控制舵机旋转到指定角度
    servo.write(angle);
    delay(500); // 等待舵机稳定

    // 发送超声波脉冲
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // 测量超声波回声的时间
    long duration = pulseIn(echoPin, HIGH);

    // 将时间转换为距离
    int distance = duration * 0.034 / 2;

    Serial.print("Angle: ");
    Serial.println(angle);
    Serial.print("  Distance: ");
    Serial.println(distance);
  }

  delay(1000); // 旋转一轮后等待1秒再重复
}

// 避障检测
void Avoidance(){
  int leftSensorValue = digitalRead(leftObstaclePin);
  int rightSensorValue = digitalRead(rightTrackPin);
  if (leftSensorValue == LOW && rightSensorValue == LOW) {
    // 左右传感器都检测到障碍物
    // 做相应的动作，比如停止
    Serial.println("障碍物在左右两侧");
  } else if (leftSensorValue == LOW) {
    // 只有左侧传感器检测到障碍物
    // 做相应的动作，比如向右转
    Serial.println("障碍物在左侧");
  } else if (rightSensorValue == LOW) {
    // 只有右侧传感器检测到障碍物
    // 做相应的动作，比如向左转
    Serial.println("障碍物在右侧");
  } else {
    // 没有检测到障碍物
    // 继续前进
    Serial.println("无障碍物");
  }
  delay(100); // 延迟以稳定读数
}
}

// 循迹检测
int Follow_the_Trace(){
  // 循迹检测
  if (digitalRead(leftTrackPin) == LOW && digitalRead(rightTrackPin) == LOW) {
    Serial.println("Both tracks on the line. Move forward.");
    // 向前运动
    march();
  } else if (digitalRead(leftTrackPin) == HIGH && digitalRead(rightTrackPin) == LOW) {
    Serial.println("Left track off the line. Turn right.");
    // 向右转
    right();
  } else if (digitalRead(leftTrackPin) == LOW && digitalRead(rightTrackPin) == HIGH) {
    Serial.println("Right track off the line. Turn left.");
    // 向左转
    left();
  } else {
    Serial.println("Lost the line. Stop!");
    // 停止运动
    return 0;
  }
}

// 蓝牙控制
void bluetooth(){
    // 读取蓝牙指令
    int BTin = Serial.read();
    Serial.println("Received BTmessage:");
    Serial.print(BTin);
    // 判断指令
    if (BTin == 'w' || BTin == 'F'){
      // 前进
      Serial.print("前进\n");
      march();
    }else if (BTin == 'a' || BTin == 'L'){
      // 左转
      Serial.print("左转\n");
      left();  
    }else if (BTin == 'd'|| BTin == 'R'){
      // 右转
      Serial.print("右转\n");
      right(); 
    }else if (BTin == 's' || BTin == 'G'){
      // 后退
      Serial.print("后退\n");
      back();  
    }else if (BTin == 10){

    }else {
      Serial.print("无效指令\n");
    }
}

// 前进
void march(){
  // 电机1前进
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 128);
  // 电机2前进
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 128);
  delay(10);
}

// 左转
void left(){
  // 电机1全速前进
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 255);
  // 电机2后退
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 128);

  delay(10);
}

// 右转
void right(){
  // 电机1后退
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 128);
  // 电机2全速前进
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN1, LOW);
  analogWrite(PWMB, 255);
  delay(10);
}

// 后退
void back(){
  // 电机1后退
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 128);
  // 电机2后退
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 128);
  delay(10);
}
 
  
