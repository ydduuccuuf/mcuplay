
#define BLINKER_WIFI
#include <Blinker.h>
#include <ESP8266WiFi.h>        
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);



char auth[] = "97aa659f1b2c";
char ssid[] = "yyy";
char pswd[] = "123456789";
int cnt=0;
// 新建组件对象
BlinkerButton Button1("btn-abc");
BlinkerNumber Number1("num-abc");
BlinkerButton Button2("btn-open");
BlinkerButton Button3("btn-close");

int counter = 0;

// 按下按键即会执行该函数
void button1_callback(const String & state)
{
    BLINKER_LOG("get button state: ", state);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
void button2_callback(const String & state)
{
    BLINKER_LOG("open_door");
    Serial.print("3"); 
}
void button3_callback(const String & state)
{
    BLINKER_LOG("close_door");
    Serial.print("4"); 
}
// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String & data)
{
    BLINKER_LOG("Blinker readString: ", data);
    counter++;
    Number1.print(counter);
}

void setup()
{
    // 初始化串口
    Serial.begin(115200);
    // BLINKER_DEBUG.stream(Serial);
    // BLINKER_DEBUG.debugAll();
  //  sensors.begin();
    // 初始化有LED的IO
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    Button3.attach(button3_callback);
}

void loop() 
{
  
  cnt++;
  Blinker.run();
//   sensors.requestTemperatures(); // 发送命令获取温度
//   Serial.print("Temperature for the device 1 (index 0) is: ");
//   Serial.println(sensors.getTempCByIndex(0)); 
// delay(200);
  if (Serial.available() > 0) 
  {
  //
  int data = Serial.read();
  
  if (data == 49) 
  {
    Blinker.wechat("警告", "警告", "有人试图闯入");
  }
  }
  if(cnt>200)
  {
  if(WiFi.status() != WL_CONNECTED) 
  {       
    Serial.print("1");
    Blinker.begin(auth, ssid, pswd);                             
  }
  else
  {
    Serial.print("2");                                     
  }
  cnt=0; 
  }       
}
