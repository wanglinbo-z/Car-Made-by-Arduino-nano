/*
名称：小车控制
作者：王淋博
时间：2024年1月
描述：超声波探路
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

const int trigPin = 12;
const int echoPin = 13;

NewPing sonar(12,13,200);

// 舵机相关
Servo servo; // 创建舵机对象
const int servoPin = A4; // 舵机的信号线连接到Arduino的模拟引脚A4


// 定义蓝牙控制引脚
SoftwareSerial BlueTooth (1, 0); // 连接到Rx , Tx引脚

void setup(){
  // 开放串口9600波特率
  Serial.begin(9600);

  // 设置蓝牙串口
  BlueTooth.begin(9600);

  // // 设置电机驱动板引脚为输出模式
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  // 超声模块
  pinMode(12, OUTPUT);
  pinMode(13, 0);

  digitalWrite(13, LOW);

  servo.attach(servoPin); // 附加舵机到指定的引脚
    
  Serial.println("Arduino is readly!");

//   pinMode(LED_BUILTIN, OUTPUT);
//   digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
//   delay(800);                      // wait for a second
//   digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
}

void loop(){
  int dAd[3] = {200,200,200};

  i_Ultrasound(dAd);

  // bz(dAd[0], dAd[1], dAd[2]);
  
  if (Serial.available()){
    BlueTooth_ctrl();
  }

  // 距离回传调试
  for (int i = 0;i < 3;i ++){
    Serial.print(dAd[i]);
    Serial.print(" ");
  }
  delay(800);

  // 发送超声波脉冲
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // 测量超声波回声的时间
  long duration = pulseIn(echoPin, HIGH);
  Serial.println((duration * 0.034 / 2));
  delay(800);
}

// 控制舵机旋转
// 参数：1——向右转90度
//       0——向左转90度
void Servo_degrees_60(bool Servo_direction){
  if (Servo_direction){
    servo.write(25);
    delay(152);
    servo.write(90);
  }else if (!Servo_direction){
    servo.write(158);
    delay(155);
    servo.write(90);
  }

}

// 蓝牙控制
void BlueTooth_ctrl(){
  switch(Serial.read()){
    case 1:
      break;
    case 's':
      delay(60000);
    default :
      break;
  }
}

// 判断如何避障
void bz(int q,int r,int l){
  if (q < 10 && r < 10 && l < 10){

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(200);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW

    // 三向都有障碍
    left_back();
    delay(500);
    DJ_reset();



  }else if (q < 10 && r < 10 && l < 10){
    // 左边有障碍
    right();

  }else if (q < 10 && r < 10 && l < 10){
    // 右边有障碍
    left();

  }else if (q < 10 && r < 10 && l < 10){

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(800);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW

    // 前方畅通
    march();


  }
}

// 超声读取三向距离
void i_Ultrasound(int* arr){

  int i = 0; // dAd数组的位次 0~2

  while(i <= 2) {

    // 控制舵机旋转到指定角度
    // if (temp_i == 0){
    //   void;
    // }else if (temp_i == 1){
    //   Servo_degrees_60(1);
    // }else if (temp_i == 2){
    //   Servo_degrees_60(0);
    //   Servo_degrees_60(0);
    // }

    int U_distance = sonar.ping_cm();

    // 将三向距离回传
    *(arr + i) = U_distance;
    i ++;
    delay(500);
  }

  // 舵机回正
  // Servo_degrees_60(1);
}

// 重置电机
void DJ_reset(){
    Serial.println("\nSTOP!");
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMA, 0);
    analogWrite(PWMB, 0);
}

// 前进
void march(){

  Serial.println("\nMove Foword");
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

  Serial.println("\nMove Back");
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

  Serial.println("\nTurn Left");
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

  Serial.println("\nTurn Right");
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