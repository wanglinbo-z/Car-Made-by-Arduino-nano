/*
名称：小车控制
作者：王淋博
时间：2024年3月
描述：红外循迹
*/

// NewPing 超声波测距库
// #include <NewPing.h>

// HC-05 蓝牙控制库
#include <SoftwareSerial.h>

// 舵机控制
// #include <Servo.h>

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

// 红外避障模块引脚定义
const int leftObstaclePin = A0;
const int rightObstaclePin = A1;

// 红外循迹模块引脚定义
const int leftTrackPin = 8;
const int rightTrackPin = 7;

// 舵机相关
// Servo servo; // 创建舵机对象
// const int servoPin = A4; // 舵机的信号线连接到Arduino的模拟引脚A4

// 定义蓝牙控制引脚
SoftwareSerial BlueTooth (1, 0); // 连接到Rx , Tx引脚

void setup(){
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
  // 避障
  pinMode(leftObstaclePin, INPUT);
  pinMode(rightObstaclePin, INPUT);
  // 循迹
  pinMode(leftTrackPin, INPUT);
  pinMode(rightTrackPin, INPUT);

  delay(2000);
}

void loop(){
  Follow_the_Trace();
  delay(500);
}

int Follow_the_Trace(){
  // 循迹检测
  if (digitalRead(leftTrackPin) == HIGH && digitalRead(rightTrackPin) == HIGH) {
    // Serial.println("\nBoth tracks on the line. Move forward.");
    // 向前运动
    march();
  } else if (digitalRead(leftTrackPin) == LOW && digitalRead(rightTrackPin) == HIGH) {
    // Serial.println("\nLeft track off the line. Turn right.");
    // 向右转
    right();
  } else if (digitalRead(leftTrackPin) == HIGH && digitalRead(rightTrackPin) == LOW) {
    // Serial.println("\nRight track off the line. Turn left.");
    // 向左转
    left();
  } else {
    Serial.println("\nLost the line. Stop!");
    // 停止运动
    DJ_reset();
    return 0;
  }
}

// 重置电机
void DJ_reset(){
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
}

// 前进
void march(){
  // 重置电机
  DJ_reset();

  Serial.print("\nMove Foword");
  // 电机1前进
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 154);
  // 电机2前进
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 154);
  delay(10);
}

// 左转
void left(){
  // 重置电机
  DJ_reset();
  delay(10);

  Serial.print("\nTurn Left");
  // 电机1全速前进
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 224);
  // 电机2后退
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 124);

  delay(10);
}

// 右转
void right(){
  // 重置电机
  DJ_reset();
  delay(10);

  Serial.print("\nTurn Right");
  // 电机1后退
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 124);
  // 电机2全速前进
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 224);

  delay(10);
}

// 后退
void back(){
  // 重置电机
  DJ_reset();
  delay(10);

  Serial.print("Back Forword");
  // 电机1后退
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 152);
  // 电机2后退
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 152);

  delay(10);
}
 
 