#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "SimpleKalmanFilter.h"
#include <MQTT.h>
#include <WiFi.h>

#define Button_Pin   13
#define Vol     34
boolean sent = false;
boolean wait = false;
String msg;

SimpleKalmanFilter simple(2, 2, 0.01);
LiquidCrystal_I2C lcd(0x27, 16, 2); // set up LCD-I2C 16 Col 2 Row
WiFiClient net;
MQTTClient client;
MQTTClient client1;

// Config wifi path
const char ssid[] = "@JumboPlusIoT";
const char pass[] = "12345678";

// Config sever path
const char mqtt_broker[]="test.mosquitto.org";
const char mqtt_topic[]="project1.21/height";
const char mqtt_sub[]="project1.21/distance";
const char mqtt_client_id[]="arduino_project_1.21"; // must change this string to a unique value
int MQTT_PORT=1883;

// Function Topic
void connect();
void messageReceived(String &topic, String &payload);
void task_arduino(void *arg);

void setup(){
  Serial.begin(9600);
  pinMode(Vol,INPUT);
  pinMode(Button_Pin,INPUT);
	lcd.init();
	lcd.backlight();
  WiFi.begin(ssid, pass);
  client.begin(mqtt_broker, MQTT_PORT, net);
  client.onMessage(messageReceived);
  connect();
  xTaskCreatePinnedToCore(
      task_arduino,
      "Task1",
      8192,
      NULL,
      0,
      NULL,
      0);
}

void loop(){
  client.loop();
  if(!client.connected()){
    connect();
  }
  // Interface running
  if(!sent && !wait){
    lcd.setCursor(3,0);
    lcd.print("PID Tuning");
    lcd.setCursor(0,1);
    lcd.print("Height(cm): ");
    lcd.setCursor(12,1);
    lcd.print(String(int(simple.updateEstimate(map(analogRead(Vol), 0, 4095, 0, 80)))));
  }
  if(wait){
    lcd.setCursor(0,0);
    lcd.print("Tuning(cm): "+msg);
    lcd.setCursor(0,1);
    lcd.print("Height(cm): "+String(int(simple.updateEstimate(map(analogRead(Vol), 0, 4095, 0, 80)))));
  }
  delay(400);
  lcd.clear();
}

// Function detail
void connect()
{
  // Connect Wifi screen
  int i = 0;
  if(WiFi.status() == WL_CONNECTED){
    lcd.clear();
    i = 0;
    lcd.setCursor(1,0);
    lcd.print("Connected WIFI");
    delay(500);
    lcd.clear();
  }
  while (WiFi.status() != WL_CONNECTED)
  {
    lcd.setCursor(1,0);
    lcd.print("ConnectingWIFI");
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
  if(client.connect(mqtt_client_id)){
    lcd.clear();
    i = 0;
    lcd.setCursor(0,0);
    lcd.print("Connected Client");
    delay(500);
    lcd.clear();
  }
  while (!client.connect(mqtt_client_id))
  {
    lcd.setCursor(0,0);
    lcd.print("ConnectingClient");
    lcd.setCursor(i,1);
    lcd.print(".");
    i++;
    if(i>15){
      i = 0;
      lcd.clear();
    }
    delay(1000);
  }
  client.subscribe(mqtt_sub);
}

void messageReceived(String &topic, String &payload)
{
  Serial.println(payload);
  if(payload==msg){
    lcd.clear();
    lcd.setCursor(6,1);
    lcd.print("Done");
    delay(1000);
    wait = false;
  }
}

void task_arduino(void *arg)
{
  while (1)
  {
    Serial.println(digitalRead(Button_Pin));
    if(digitalRead(Button_Pin) && !sent && !wait){
      sent = true;
      msg = String(int(simple.updateEstimate(map(analogRead(Vol), 0, 4095, 0, 80))));
    }
    if(sent){
      client.publish(mqtt_topic, msg);
      sent = false;
      wait = true;
    }
  }
}