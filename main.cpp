#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <MQTT.h>
#include <WiFi.h>

#define Button_Pin   35
#define Vol     23
String height_msg;
boolean sent = false;

LiquidCrystal_I2C lcd(0x27, 16, 2); // set up LCD-I2C 16 Col 2 Row
WiFiClient net;
MQTTClient client;

// Config wifi path
const char ssid[] = "SSID";
const char pass[] = "WIFI_PASSWORD";

// Config sever path
const char mqtt_broker[]="test.mosquitto.org";
const char mqtt_topic[]="group/command";
const char mqtt_client_id[]="arduino_group_x"; // must change this string to a unique value
int MQTT_PORT=1883;

// Function Topic
void connect();
void messageReceived(String &topic, String &payload);
void IRAM_ATTR IO_INT_ISR();

void setup(){
  pinMode(Vol,INPUT);
  pinMode(Button_Pin,INPUT);
  attachInterrupt(Button_Pin, IO_INT_ISR, RISING);
	lcd.init();
	lcd.backlight();
  WiFi.begin(ssid, pass);
  client.begin(mqtt_broker, MQTT_PORT, net);
  client.onMessage(messageReceived);
  connect();
}

void loop(){
  // Interface running
  lcd.setCursor(3,0);
  lcd.print("PID Tuning");
  if(!sent){
    lcd.setCursor(0,1);
    lcd.print("Height(cm): ");
    lcd.setCursor(12,1);
    lcd.print(map(analogRead(27), 0, 4095, 0, 100));
  }
  delay(200);
  lcd.clear();
}

// Function detail
void connect()
{
  // Connect Wifi screen
  int i = 0;
  if(WiFi.status() != WL_CONNECTED){
    lcd.setCursor(1,0);
    lcd.print("ConnectingWIFI");
  }
  else if(WiFi.status() == WL_CONNECTED){
    lcd.clear();
    i = 0;
    lcd.setCursor(1,0);
    lcd.print("Connected WIFI");
    delay(500);
    lcd.clear();
  }
  while (WiFi.status() != WL_CONNECTED)
  {
    lcd.setCursor(i,1);
    lcd.print(".");
    i++;
    if(i>15){
      i = 0;
      lcd.clear();
    }
    delay(1000);
  }
  // Connect client
  if(!client.connect(mqtt_client_id)){
    lcd.setCursor(0,0);
    lcd.print("ConnectingClient");
  }
  else if(client.connect(mqtt_client_id)){
    lcd.clear();
    i = 0;
    lcd.setCursor(0,0);
    lcd.print("Connected Client");
    delay(500);
    lcd.clear();
  }
  while (!client.connect(mqtt_client_id))
  {
    lcd.setCursor(i,1);
    lcd.print(".");
    i++;
    if(i>15){
      i = 0;
      lcd.clear();
    }
    delay(1000);
  }
  client.subscribe(mqtt_topic);
}

void messageReceived(String &topic, String &payload)
{
  if(payload==height_msg){
    lcd.clear();
    lcd.setCursor(6,1);
    lcd.print("Done");
    sent = false;
  }
  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void IRAM_ATTR onTimer(){
  sent = true;
  height_msg = String(map(analogRead(27), 0, 4095, 0, 100));
  client.publish(mqtt_topic, height_msg);
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Tuning");
}