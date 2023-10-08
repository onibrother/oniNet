#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

#define azul 14
#define vermelho 33
#define verde 32
#define lampada 25
#define porta 26

const char* ssid = "Skynet";
const char* password = "Uzumakiwshbr1";

const char* mqtt_server = "192.168.1.45";

WiFiClient espClient;
PubSubClient client(espClient);

void blink_led(unsigned int times, unsigned int duration,unsigned int led){
  for (int i = 0; i < times; i++) {
    digitalWrite(led, HIGH);
    delay(duration);
    digitalWrite(led, LOW); 
    delay(200);
  }
}
void setup_wifi(){
  delay(50);
  Serial.println();
  Serial.print("estabelecendo conexao wifi com ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int c=0;
  while(WiFi.status() != WL_CONNECTED){
    blink_led(2, 200,azul);
    delay(1000);
    Serial.print(".");
    c=c+1;
    if(c>10){
      ESP.restart();
    }
  }
  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}
void connect_mqttServer() {
  // Loop until we're reconnected
  while (!client.connected()) {

        //first check if connected to wifi
        if(WiFi.status() != WL_CONNECTED){
          //if not connected, then first connect to wifi
          setup_wifi();
        }

        //now attemt to connect to MQTT server
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("trancaEletronica")) { // Change the name of client here if multiple ESP32 are connected
          //attempt successful
          Serial.println("connected");
          // Subscribe to topics here
          client.subscribe("tranca");
          //client.subscribe("rpi/xyz"); //subscribe more topics here
          
        } 
        else {
          //attempt not successful
          Serial.print("failed, rc=");
          Serial.print(client.state());
          Serial.println(" trying again in 2 seconds");
    
          blink_led(3,200,vermelho); //blink LED three times (200ms on duration) to show that MQTT server connection attempt failed
          // Wait 2 seconds before retrying
          delay(2000);
        }
  }
  
}
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];

  }
  Serial.println();

  // Check if a message is received on the topic "rpi/broadcast"
  if (String(topic) == "tranca") {
      if(messageTemp == "lamp"){
        Serial.println("comando para a lampada");
        digitalWrite(lampada,!digitalRead(lampada));
      }
      if(messageTemp == "porta"){
        Serial.println("comando para a porta");
        digitalWrite(porta, !digitalRead(porta));
      }
  }

  //Similarly add more if statements to check for other subscribed topics 
}
void setup() {
  // put your setup code here, to run once:
  pinMode(lampada, OUTPUT);
  pinMode(porta, OUTPUT);
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server,1883);//1883 is the default port for MQTT server
  client.setCallback(callback);

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  ArduinoOTA.handle();
  if (!client.connected()) {
    connect_mqttServer();
  }
  client.loop();
}
