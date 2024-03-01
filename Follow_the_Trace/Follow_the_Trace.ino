/*
名称：小车控制
作者：wanglinbo-z
时间：2024年3月
描述：红外循迹
*/

// HC-05 蓝牙控制库
#include <SoftwareSerial.h>

// 定义电机驱动板引脚
// 右轮
const int AIN1 = 9; // 连接到TB6612的AIN1引脚
const int AIN2 = 10; // 连接到TB6612的AIN2引脚
const int PWMA = 11; // 连接到TB6612的PWMA引脚
// 左轮
const int BIN1 = 6; // 连接到TB6612的BIN1引脚
const int BIN2 = 5; // 连接到TB6612的BIN2引脚
const int PWMB = 3; // 连接到TB6612的PWMB引脚

// 红外循迹模块引脚定义
const int leftTrackPin = 8;
const int rightTrackPin = 7;

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
  // 循迹
  pinMode(leftTrackPin, INPUT);
  pinMode(rightTrackPin, INPUT);
    
  Serial.println(＂Arduino is readly!＂);
}

void loop(){
  while (Follow_the_Trace()){
      
  }
  Serial.println(＂Reached the end point！＂)；

  delay(100);
}

bool Follow_the_Trace(){
  // 循迹检测
  if (digitalRead(leftTrackPin) == LOW && digitalRead(rightTrackPin) == LOW) {
    // Serial.println("\nBoth tracks on the line. Move forward.");
    // 向前运动
    march();
    return 1;
  } else if (digitalRead(leftTrackPin) == HIGH && digitalRead(rightTrackPin) == LOW) {
    // Serial.println("\nLeft track off the line. Turn right.");
    // 向右转
    right();
    return 1;
  } else if (digitalRead(leftTrackPin) == LOW && digitalRead(rightTrackPin) == HIGH) {
    // Serial.println("\nRight track off the line. Turn left.");
    // 向左转
    left();
    return 1;
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
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
}

// 前进
void march(){

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

  Serial.print("\nTurn Left");
  // 电机1全速前进
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  analogWrite(PWMA, 224);
  // 电机2后退
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  analogWrite(PWMB, 24);

  delay(10);
}

// 右转
void right(){

  Serial.print("\nTurn Right");
  // 电机1后退
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  analogWrite(PWMA, 24);
  // 电机2全速前进
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  analogWrite(PWMB, 224);

  delay(10);
}
 
 
