/*
版权声明：
深圳市小二极客科技（小R科技）旗下：WIFI机器人网·机器人创意工作室版权所有www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
小R科技保留付诸法律起诉侵权的权利！
* 文件名称：Init
* 文件标识：
* 摘    要：WiFi机器人智能小车控制
* 描    述：初始化文件
* 当前版本：Arduino UNO R3 V2.5
* 作    者：BY WIFI机器人网·机器人创意工作室
* 完成日期：2017年6月
*/

/*
*********************************************************************************************************
** 函数名称 ：Delayed()
** 函数功能 ：延时程序
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void  Delayed()    //延迟50秒等WIFI模块启动完毕
{
    int i;
    for(i=0;i<28;i++)
    {
        digitalWrite(ledpin,LOW);
        delay(1000);
        digitalWrite(ledpin,HIGH);
        delay(1000);
    }
    for(i=0;i<10;i++)
    {
        digitalWrite(ledpin,LOW);
        delay(500);
        digitalWrite(ledpin,HIGH);
        delay(500);
    }
    digitalWrite(ledpin,LOW);
    MOTOR_GO_STOP;
}


/*
*********************************************************************************************************
** 函数名称 ：setup().Init_Steer()
** 函数功能 ：系统初始化（串口、电机、舵机、指示灯初始化）。
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void Init_Steer()//舵机初始化(角度为上次保存数值)
{
    angle1 = EEPROM.read(0x01);//读取寄存器0x01里面的值
    angle2 = EEPROM.read(0x02);//读取寄存器0x02里面的值
    angle3 = EEPROM.read(0x03);//读取寄存器0x03里面的值
    angle4 = EEPROM.read(0x04);//读取寄存器0x04里面的值
    //angle5 = EEPROM.read(0x05);//读取寄存器0x05里面的值
    //angle6 = EEPROM.read(0x06);//读取寄存器0x06里面的值
    angle7 = EEPROM.read(0x07);//读取寄存器0x07里面的值
    angle8 = EEPROM.read(0x08);//读取寄存器0x08里面的值
    
    if(angle7 == 255 && angle8 == 255)
    {
        EEPROM.write(0x01,60);//把初始角度存入地址0x01里面
        EEPROM.write(0x02,60);//把初始角度存入地址0x02里面
        EEPROM.write(0x03,60);//把初始角度存入地址0x03里面
        EEPROM.write(0x04,60);//把初始角度存入地址0x04里面
        //EEPROM.write(0x05,60);//把初始角度存入地址0x05里面
        //EEPROM.write(0x06,120);//把初始角度存入地址0x06里面
        EEPROM.write(0x07,60);//把初始角度存入地址0x07里面
        EEPROM.write(0x08,60);//把初始角度存入地址0x08里面
        return;
    }
 
    servo1.write(angle1);//把保存角度赋值给舵机1
    servo2.write(angle2);//把保存角度赋值给舵机2
    servo3.write(angle3);//把保存角度赋值给舵机3
    servo4.write(angle4);//把保存角度赋值给舵机4
    //servo5.write(angle5);//把保存角度赋值给舵机5
    //servo6.write(angle6);//把保存角度赋值给舵机6
    servo7.write(angle7);//把保存角度赋值给舵机7
    servo8.write(angle8);//把保存角度赋值给舵机8
    adjust = EEPROM.read(0x10);//读取寄存器0x10里面的值
    if(adjust==0xff)EEPROM.write(0x10,1);
    
    Left_Speed_Hold = EEPROM.read(0x09);//读取寄存器0x03里面的值
    Right_Speed_Hold = EEPROM.read(0x0A);//读取寄存器0x04里面的值
    if(Left_Speed_Hold<55|Right_Speed_Hold<55)
    {
      Left_Speed_Hold=255;
      Right_Speed_Hold=255;
    }
    analogWrite(ENB,Left_Speed_Hold);//给L298使能端B赋值
    analogWrite(ENA,Right_Speed_Hold);//给L298使能端A赋值
    MOTOR_GO_STOP;
}
