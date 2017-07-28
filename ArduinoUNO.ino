/*
版权声明：
深圳市小二极客科技（小R科技）旗下：WIFI机器人网·机器人创意工作室版权所有www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
小R科技保留付诸法律起诉侵权的权利！
* 文件名称：ArduinoUNO
* 文件标识：
* 摘    要：WiFi机器人智能小车控制
* 描    述：程序主函数文件
* 当前版本：Arduino UNO R3 V2.5
* 作    者：BY WIFI机器人网·机器人创意工作室
* 完成日期：2017年6月
*/
#include <Servo.h>
#include <EEPROM.h> 

int ledpin = A0;                    //设置系统启动指示灯
int ENA = 5;                        //L298使能A
int ENB = 6;                        //L298使能B
int INPUT2 = 7;                     //电机接口1
int INPUT1 = 8;                     //电机接口2
int INPUT3 = 12;                    //电机接口3
int INPUT4 = 13;                    //电机接口4
int adjust = 1;                     //定义电机标志位
int Echo = A5;                      // 定义超声波信号接收脚位  
int Trig = A4;                      // 定义超声波信号发射脚位
int Input_Detect_LEFT = A3;         //定义小车左侧红外
int Input_Detect_RIGHT = A2;        //定义小车右侧红外
int Input_Detect = A1;              //定义小车前方红外
int Carled = A0;                    //定义小车车灯接口
int Cruising_Flag = 0;              //模式切换标志
int Pre_Cruising_Flag = 0 ;         //记录上次模式
int Left_Speed_Hold = 255;          //定义左侧速度变量
int Right_Speed_Hold = 255;         //定义右侧速度变量

Servo servo1;                       // 创建舵机#1号
Servo servo2;                       // 创建舵机#2号
Servo servo3;                       // 创建舵机#3号
Servo servo4;                       // 创建舵机#4号
//Servo servo5;                      // 创建舵机#5号
//Servo servo6;                      // 创建舵机#6号
Servo servo7;                       // 创建舵机#7号
Servo servo8;                       // 创建舵机#8号

byte angle1 = 70;                    //舵机#1初始值
byte angle2 = 60;                    //舵机#2初始值
byte angle3 = 60;                    //舵机#3初始值
byte angle4 = 60;                    //舵机#4初始值
//byte angle5 = 60;                  //舵机#5初始值
//byte angle6 = 60;                  //舵机#6初始值
byte angle7 = 60;                    //舵机#7初始值
byte angle8 = 60;                    //舵机#8初始值

int buffer[3];                       //串口接收数据缓存
int rec_flag;                        //串口接收标志位
int serial_data;                     //串口数据零时存储
unsigned long Costtime;              //串口超时计数
int IR_R;                            //巡线右侧红外状态标志
int IR_L;                            //巡线左侧红外状态标志
int IR;                              //中间红外状态标志

/*宏定义小车转向方向*/
#define MOTOR_GO_FORWARD  {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}    //车体前进                          
#define MOTOR_GO_BACK      {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}   //车体后退
#define MOTOR_GO_RIGHT    {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}    //车体右转
#define MOTOR_GO_LEFT     {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}    //车体左转
#define MOTOR_GO_STOP     {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,LOW);}      //车体停止


/*
*********************************************************************************************************
** 函数名称 ：Cruising_Mod()
** 函数功能 ：模式切换函数
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void Cruising_Mod()//模式功能切换函数
{
  if (Pre_Cruising_Flag != Cruising_Flag)
  {
    if (Pre_Cruising_Flag != 0)
    {
      MOTOR_GO_STOP;
    }
    Pre_Cruising_Flag =  Cruising_Flag;
  }
  switch (Cruising_Flag)
  {
    case 2: TrackLine(); return;            //巡线模式
    case 3: Avoiding(); return;             //避障模式
    case 4: AvoidByRadar(15); return;       //超声波避障模式
    case 5: Send_Distance(); return;        //超声波距离PC端显示
    default: return;
  }
}

/*
*********************************************************************************************************
** 函数名称 ：setup()
** 函数功能 ：设置函数
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void setup()
{
    pinMode(ledpin,OUTPUT); 
    pinMode(ENA,OUTPUT); 
    pinMode(ENB,OUTPUT); 
    pinMode(INPUT1,OUTPUT); 
    pinMode(INPUT2,OUTPUT); 
    pinMode(INPUT3,OUTPUT); 
    pinMode(INPUT4,OUTPUT); 
    pinMode(Input_Detect_LEFT,INPUT);
    pinMode(Input_Detect_RIGHT,INPUT);
    pinMode(Carled, OUTPUT);
    pinMode(Input_Detect,INPUT);
    pinMode(Echo,INPUT);
    pinMode(Trig,OUTPUT);
    Delayed();                              //延迟50秒等WIFI模块启动完毕
    servo1.attach(3);                       //定义舵机1控制口
    servo2.attach(4);                       //定义舵机2控制口
    servo3.attach(2);                       //定义舵机3控制口
    servo4.attach(11);                      //定义舵机4控制口
    //servo5.attach(SDA);                   //定义舵机5控制口
    //servo6.attach(SCL);                   //定义舵机6控制口
    servo7.attach(9);                       //定义舵机7控制口
    servo8.attach(10);                      //定义舵机8控制口
    USART_init();                           //串口初始化波特率设置为9600 bps
    Init_Steer();                           //舵机角度电机速度初始化
}

/*
*********************************************************************************************************
** 函数名称 ：loop()
** 函数功能 ：主函数
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void loop()
{  
  while (1)
  {
    UartTimeoutCheck();                   //串口超时检测
    Cruising_Mod();                       //模式切换
  }
}

