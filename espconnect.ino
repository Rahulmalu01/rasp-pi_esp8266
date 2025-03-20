#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "SSID";   
const char* password = "PASSWORD";
const char* mqttServer = "192.168.1.100";  // Replace with Raspberry Pi IP
const int mqttPort = 1883;
const char* clientID = "NodeMCU_1"; // Unique identifier

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("[NodeMCU_1] Message received on topic: ");
  Serial.println(topic);
  
  Serial.print("[NodeMCU_1] Data: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("[NodeMCU_1] Connecting to WiFi...");
  }
  Serial.println("[NodeMCU_1] Connected to WiFi");

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("[NodeMCU_1] Connecting to MQTT...");
    if (client.connect(clientID)) {
      Serial.println("[NodeMCU_1] Connected to MQTT");
      client.subscribe("sensor/control");
    } else {
      Serial.print("[NodeMCU_1] Failed, state: ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    while (!client.connect(clientID)) {
      Serial.println("[NodeMCU_1] Reconnecting to MQTT...");
      delay(2000);
    }
  }

  int temperature = random(20, 30);
  int humidity = random(40, 60);

  // Label data for differentiation
  String data = "[NodeMCU_1] Temp: " + String(temperature) + "°C, Humidity: " + String(humidity) + "%";
  
  Serial.println(data); // Print labeled data
  client.publish("sensor/data", data.c_str());

  client.loop();
  delay(5000);
}
