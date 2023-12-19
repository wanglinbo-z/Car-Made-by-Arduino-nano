# Car-Made-by-Arduino-nano

Car-Made-by-Arduino-nano是一个使用Arduino nano和HC-05蓝牙模块制作的遥控小车的项目。

## 功能

- 可以使用Android手机上的应用程序通过蓝牙控制小车的前进，后退，左转，右转。
- 可以使用手机上的按钮控制小车的喇叭和灯光。

## 安装

你需要准备以下的硬件组件：

- Arduino nano
- HC-05蓝牙模块
- TB6612FNG电机驱动模块
- 两个带轮子的电机
- 一个360度旋转的轮子（非驱动）
- 一个9伏特的电池
- 两个电池盒
- 一些导线和面包板

你需要按照以下的电路图连接各个组件：

![电路图](https://srituhobby.com/how-to-build-a-bluetooth-control-car-with-arduino-nano-and-hc-05-module/)

你还需要在你的Android手机上安装[这个应用程序](https://play.google.com/store/apps/details?id=com.lekpkd.duinojoy)，它可以发送蓝牙信号给Arduino nano。

## 使用

首先，你需要将Arduino nano连接到电脑，并使用Arduino IDE将[这段代码](https://play.google.com/store/apps/details?id=com)上传到Arduino nano上。然后，你需要将电池连接到Arduino nano和电机驱动模块上，给它们供电。接着，你需要打开你的手机上的蓝牙，并搜索HC-05模块，将它们配对。最后，你需要打开你的手机上的应用程序，并选择HC-05模块作为蓝牙设备，然后你就可以使用手机上的按钮控制小车了。

## 贡献

欢迎任何人对本项目进行贡献，无论是提出问题，建议，还是提交代码。如果你想进行重大的改变，请先开启一个issue，讨论你想要改变的内容。请确保更新测试和文档。
感谢SparkFun提供了tb6612fng驱动的库和教程[TB6612FNG驱动库](https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library)，让我能够更容易地控制电机。

## 许可

本项目使用[MIT许可证](https://github.com/Knock5700/Remote-Start.git)。
