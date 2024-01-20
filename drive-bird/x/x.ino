#include <Servo.h> 
#include <FastLED.h>//灯带
#define LED_PIN     13
#define Crash_PIN   12
#define LIGHT_PIN   4
#define NUM_LEDS 12
CRGB leds[NUM_LEDS];
//pwm输出口 3、5、6、9、10、11
//int led_pin[2]={1,2};
//int led_len=sizeof(led_pin)/sizeof(led_pin[0]);
int pwm_pin[4]={3,5,6,9};   //
//热敏传感器
#define ad_right_up A0
#define ad_right_down A1
#define ad_left_up A2
#define ad_left_down A3
#define ad_middle 2
#define ad_motor A5
int AD_right_up = 0;
int AD_right_down = 0;
int AD_left_up = 0;
int AD_left_down = 0;
int AD_middle = 0;
int AD_motor=1;
int light_pin=0;
int time_count=0; //rgb计时
int motor_count=0;
int position=0;
int led_state=0;
int crash=1;
//舵机
Servo left_right;
Servo up_down;
// 动态变量
volatile float val;
volatile float angle;
void up_low(){ analogWrite(6,200);	analogWrite(11,0);	}
void up(){ analogWrite(6,210);	analogWrite(11,0);	}
void down(){analogWrite(6,0);	analogWrite(11,170); 	}
void stop(){analogWrite(6,0);	analogWrite(11,0);	}
void motor_down()
{
  motor_count=0;
  while(1)
  {
    crash=digitalRead(Crash_PIN);
    down();
    delay(100);
    motor_count+=1;
    if(motor_count>=55){break;}
    if(crash==0){break;}
  }
  stop();delay(600);
  up_low();delay(200);
  stop();delay(50);
}
void motor_up()
{
  motor_count=0;
  while(1)
  {
    AD_motor=digitalRead(ad_motor);
    up();
    delay(100);
    motor_count+=1;
    Serial.println(motor_count);
    if(motor_count>=40){break;}
  }
  stop();
}
void showpin()      //调试//存在时会存在极大延迟
{
  Serial.print("AD_right_up = ");
  Serial.println(AD_right_up);
  Serial.print("AD_right_down = ");
  Serial.println(AD_right_down);
  Serial.print("AD_left_up = ");
  Serial.println(AD_left_up);
  Serial.print("AD_left_down = ");
  Serial.println(AD_left_down);
  Serial.print("AD_middlw = ");
  Serial.println(AD_middle);
  Serial.print("led_pin = ");
  Serial.println(light_pin);
  Serial.print("crash = ");
  Serial.println(crash);

}
void red()
{
  for (int i = 0; i <= 11; i++) 
  {
    leds[i] = CRGB ( 0, 0, 255);
    FastLED.show();
  }
}
void blue()
{
  for (int i = 0; i <= 11; i++) 
  {
    leds[i] = CRGB ( 255, 0, 0);
    FastLED.show();
  }
}
void light()
{
    if(time_count>=500)
    {
      FastLED.clear();
      time_count=time_count-500;
      if(led_state=0)
      {
        delay(1);
        led_state=1;
        red();
      }
      else if(led_state==1)
      {
        delay(1);
        led_state=0;
        blue();
      }
    }
    else
    {
      if(led_state==1)
      {
        red();
      }
      else if(led_state==0)
      {
        blue();
      }
    }

}
void limit_servo()
{
  //限位
  if(val>=170){val=170;}
  if(val<=85){val=85;}
  if(angle>=175){angle=175;}
  if(angle<=5){angle=5;}

}
void yuntai()    //云台运动    //angel左右
{
  float temp_val=0,temp_angle=0;
  float servo_count=0;
  if(AD_right_down==0)
  {
    servo_count=servo_count+1;
    temp_val=temp_val+0.32;
    temp_angle=temp_angle+0.32;
  }
  if(AD_right_up==0)
  {
    servo_count=servo_count+1;
    temp_val=temp_val-0.32;
    temp_angle=temp_angle+0.32;
  }
  if(AD_left_down==0)
  {
    servo_count=servo_count+1;
    temp_val=temp_val+0.32;
    temp_angle=temp_angle-0.32;
  }
  if(AD_left_up==0)
  {
    servo_count=servo_count+1;
    temp_val=temp_val-0.32;
    temp_angle=temp_angle-0.32;
  }
  if(servo_count>0)
  {
    temp_val=(temp_val/servo_count)/2;
    temp_angle=(temp_angle/servo_count)/2;
  }
  val=val+temp_val;
  angle=angle+temp_angle;
  limit_servo();
  up_down.write(abs(val));
  left_right.write(abs(angle));
  delay(1);
  val=val+temp_val;
  angle=angle+temp_angle;
  limit_servo();
  up_down.write(abs(val));
  left_right.write(abs(angle));
  delay(1);

}
void update_sensor()
{
  AD_right_up = digitalRead(ad_right_up);
  AD_right_down = digitalRead(ad_right_down);
  AD_left_up = digitalRead(ad_left_up);
  AD_left_down = digitalRead(ad_left_down);
  AD_middle = digitalRead(ad_middle);
  light_pin=digitalRead(LIGHT_PIN);
  crash=digitalRead(Crash_PIN);
}
void servo_reset()
{
  while(((val<83)||(val>87))&&((angle<88)||(angle>92)))
  {
    if(val>87){val=val-0.1;}
    else if(val<83){val=val+0.1;}
    if(angle>92){angle=angle-0.1;}
    else if(angle<88){angle=angle+0.1;}
    up_down.write(abs(val));  
    left_right.write(abs(angle)); 
    delay(2);
  }
  angle=90;
  val=85;
  up_down.write(abs(val)); 
  left_right.write(abs(angle));  
}



//主函数
void setup() {
  // put your setup code here, to run once:
  left_right.attach(pwm_pin[0]);   //左右舵机 3号引脚
  up_down.attach(pwm_pin[1]);      //上下舵机 5号引脚
  angle = 90;
  val = 85;
  Serial.begin(9600);  //用于调试，

  //电机  pin 6 11
  pinMode(6,OUTPUT);		//IO输出
  pinMode(11,OUTPUT);		//IO输出
  //灯带
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();
  //光敏
  pinMode(LIGHT_PIN,INPUT_PULLUP);
  //舵机回到最初位置
  up_down.write(abs(val)); 
  left_right.write(abs(angle));  
  //电机初始
  motor_down();
  time_count=0;
}

void loop() {
  update_sensor();
  // put your main code here, to run repeatedly:
  int a=0;//容忍时间

      if(AD_right_up==1&&AD_right_down==1&&AD_left_up==1&&AD_left_down==1&&AD_middle==1)
      {
        a=0;
        while(AD_right_up==1&&AD_right_down==1&&AD_left_up==1&&AD_left_down==1&&AD_middle==1)
        {
          update_sensor();
          delay(100);
          a=a+1;
          if(a>10){break;}
        }
          if(AD_right_up==1&&AD_right_down==1&&AD_left_up==1&&AD_left_down==1&&AD_middle==1)
          {
          if(position==1)
          {
            servo_reset();
            delay(500);
            motor_down();
            delay(200);
            position=0;
          }
          else{}
          }
      }
      else
      {
        if(position==0)
        {
          position=1;
          motor_up();
          delay(250);
        }
        else
        {
          yuntai();
        }
      }

////////////////////
  //showpin();
}
/*
if(!digitalRead(11))
//暗环境
{

}
else
//光照充足
{
    if (AD_up>100) {
    angle -= 0.039;
    delay(1);
    left_right.write(abs(angle));
  }
  if (AD_down>100) {
    angle += 0.039;
    delay(1);
    left_right.write(abs(angle));
  }
  if (AD_left>100) {
    val -= 0.039;
    delay(1);
    up_down.write(abs(val));
  }
  if (AD_right>100) {
    val += 0.039;
    delay(1);
    up_down.write(abs(val));
  }

}
*/






/*
void loop() {
  if ((!digitalRead(11))) {
    angle -= 0.039;
    delay(1);
    servo_A1.write(abs(angle));
  }
  if ((!digitalRead(9))) {
    angle += 0.039;
    delay(1);
    servo_A1.write(abs(angle));
  }
  if ((!digitalRead(12))) {
    val -= 0.039;
    delay(1);
    servo_A0.write(abs(val));
  }
  if ((!digitalRead(10))) {
    val += 0.039;
    delay(1);
    servo_A0.write(abs(val));
  }
}
*/
