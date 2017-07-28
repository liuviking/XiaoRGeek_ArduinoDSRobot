/*
版权声明：
深圳市小二极客科技（小R科技）旗下：WIFI机器人网·机器人创意工作室版权所有www.wifi-robots.com
您可以任意修改本程序，并应用于自行研发的智能小车机器人及其他电子产品上，但是禁止用于商业牟利。
小R科技保留付诸法律起诉侵权的权利！
* 文件名称：Motor
* 文件标识：
* 摘    要：WiFi机器人智能小车控制
* 描    述：电机控制相关文件
* 当前版本：Arduino UNO R3 V2.5
* 作    者：BY WIFI机器人网·机器人创意工作室
* 完成日期：2017年6月
*/
/*
通过校准值校准小车方向
*/ 
void forward(int adjust)
{
  switch (adjust)
  {
    case 1: MOTOR_GO_FORWARD; return;
    case 2: MOTOR_GO_FORWARD; return;
    case 3: MOTOR_GO_BACK; return;
    case 4: MOTOR_GO_BACK; return;
    case 5: MOTOR_GO_LEFT; return;
    case 6: MOTOR_GO_LEFT; return;
    case 7: MOTOR_GO_RIGHT; return;
    case 8: MOTOR_GO_RIGHT; return;
    default: return;
  }
}
/*
通过校准值校准小车方向
*/ 
void back(int adjust)
{
  switch (adjust)
  {
    case 1: MOTOR_GO_BACK; return;
    case 2: MOTOR_GO_BACK; return;
    case 3: MOTOR_GO_FORWARD; return;
    case 4: MOTOR_GO_FORWARD; return;
    case 5: MOTOR_GO_RIGHT; return;
    case 6: MOTOR_GO_RIGHT; return;
    case 7: MOTOR_GO_LEFT; return;
    case 8: MOTOR_GO_LEFT; return;
    default: return;
  }
}
/*
通过校准值校准小车方向
*/ 
void left(int adjust)
{
  switch (adjust)
  {
    case 1: MOTOR_GO_LEFT; return;
    case 2: MOTOR_GO_RIGHT; return;
    case 3: MOTOR_GO_LEFT; return;
    case 4: MOTOR_GO_RIGHT; return;
    case 5: MOTOR_GO_FORWARD; return;
    case 6: MOTOR_GO_BACK; return;
    case 7: MOTOR_GO_FORWARD; return;
    case 8: MOTOR_GO_BACK; return;
    default: return;
  }
}
/*
通过校准值校准小车方向
*/ 
void right(int adjust)
{
  switch (adjust)
  {
    case 1: MOTOR_GO_RIGHT; return;
    case 2: MOTOR_GO_LEFT; return;
    case 3: MOTOR_GO_RIGHT; return;
    case 4: MOTOR_GO_LEFT; return;
    case 5: MOTOR_GO_BACK; return;
    case 6: MOTOR_GO_FORWARD; return;
    case 7: MOTOR_GO_BACK; return;
    case 8: MOTOR_GO_FORWARD; return;
    default: return;
  }
}


/*
*********************************************************************************************************
** 函数名称 ：Avoiding
** 函数功能 ：检测在车体前面中间方位红外前有无障碍，有的话小车停止
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/  
void  Avoiding()//红外避障函数
{
  IR = digitalRead(Input_Detect);
  if ((IR == LOW))
  {
    MOTOR_GO_STOP;//停止
  }
}
 
/*
*********************************************************************************************************
** 函数名称 ：TrackLine
** 函数功能 ：检测黑线在俩个红外之间的位置，通过逻辑判断再做出小车的方向改变
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/ 
void TrackLine()   // 巡线模式
{
  IR_L = digitalRead(Input_Detect_LEFT);//读取左边传感器数值
  IR_R = digitalRead(Input_Detect_RIGHT);//读取右边传感器数值

  if ((IR_L == LOW) && (IR_R == LOW)) //两边同时探测到障碍物
  {
    forward(adjust);//直行
    return;
  }
  
  if ((IR_L == LOW) && (IR_R == HIGH)) //右侧遇到障碍
  {
    left(adjust);//左转
    return;
  }
  
  if ((IR_L == HIGH) && ( IR_R == LOW)) //左侧遇到障碍
  {
    right(adjust);//右转
    return;
  }
  
  if ((IR_L == HIGH) && (IR_R == HIGH)) //左右都检测到，就如视频中的那样遇到一道横的胶带
  {
    MOTOR_GO_STOP;//停止
    return;
  }
}

/*
*********************************************************************************************************
** 函数名称 ：AvoidByRadar
** 函数功能 ：当超声波检测到的距离小于distance(单位cm)时，小车停下来
** 入口参数 ：无
** 出口参数 ：无
*********************************************************************************************************
*/
void AvoidByRadar(int distance)//超声波避障函数
{
  int leng = Get_Distance();
  if(distance<10)distance=10;    //限定最小避障距离为10cm
  if((leng>1)&&(leng < distance))//避障距离值(单位cm)，大于1是为了避免超声波的盲区
  {
      MOTOR_GO_STOP;
  }
}
