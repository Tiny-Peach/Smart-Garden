#include <WiFi.h>
#include "PubSubClient.h"
#include <ESP32Servo.h>
#include <DHT.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqttServer = "broker.hivemq.com";
int port = 1883;
String stMac;
char mac[50];
char clientId[50];

WiFiClient espClient;
PubSubClient client(espClient);

// Pin definitions
#define DHTPIN 2
#define DHTTYPE DHT22
#define LDRPIN 34
#define SERVOPIN 14

DHT dht(DHTPIN, DHTTYPE);
Servo servo;

bool isManualMode = false;

void setup() {
 randomSeed(analogRead(0));

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  wifiConnect();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.macAddress());
  stMac = WiFi.macAddress();
  stMac.replace(":", "_");
  Serial.println(stMac);
  
  client.setServer(mqttServer, port);
  client.setCallback(callback);

  Serial.begin(115200);
  pinMode(DHTPIN, INPUT);
  pinMode(SERVOPIN, OUTPUT);
  pinMode(DHTTYPE , INPUT);
  pinMode(LDRPIN , INPUT);
  dht.begin();
  servo.attach(SERVOPIN);
}

void wifiConnect() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void mqttReconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    long r = random(1000);
    sprintf(clientId, "clientId-%ld", r);
    if (client.connect(clientId)) {
      Serial.print(clientId);
      Serial.println(" connected");
      client.subscribe("manual_control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String stMessage;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    stMessage += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "manual_control") {
    Serial.print("Changing output to ");
    if(stMessage == "on"){
      Serial.println("on");
      client.publish("manual_control", "watering is ON");
      servo.write(90);
      delay(10000);
      
    }
    else if(stMessage == "off"){
      Serial.println("off");
       client.publish("manual_control", "watering is OFF");
      servo.write(0);
      delay(10000);
     
    }
  }
}


void loop() {
  if (!client.connected()) {
   mqttReconnect();
  }
  client.loop();

  // Read sensor values
  float temperature = dht.readTemperature();
  Serial.print(F("Temperature: "));
  Serial.println(temperature);
  Serial.print(F("Humidity: "));
  float humidity = dht.readHumidity();
  Serial.println(humidity);
  int brightness = analogRead(LDRPIN);
  Serial.print(F("Brightness: "));
  Serial.println(brightness);
  
  // Publish sensor data
  client.publish("Temperature_G", String(temperature).c_str(), true); // Retain flag set to true for persistent message
  client.publish("Humidity_G", String(humidity).c_str(), true);
  client.publish("brightness_G", String(brightness).c_str(), true);

  // Automatic control logic
  if (!isManualMode) {
    if (temperature > 40 && humidity < 50 && brightness==1001) {
      servo.write(90); //  Turn on watering
    } else {
      servo.write(0); // Turn off watering
    }
  }

  delay(1000); 
}
