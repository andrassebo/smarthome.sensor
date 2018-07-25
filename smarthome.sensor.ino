#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11 

const char* ssid     = "";
const char* password = "";

const String host = "192.168.0.200";
String mac = "n/a";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(2000);

  // We start by connecting to a WiFi network
  WiFi.disconnect(true);
  delay(1000);
  WiFi.onEvent(WiFiEvent);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("MAC: ");
  mac = WiFi.macAddress();
  Serial.println(mac);  
}

void send_value(String type, float value) {
   String content = type + ",host=" + mac + " value=" + String(value);
   
   client.print("POST /write?db=sensor HTTP/1.1\r\n");
   client.print("User-Agent: esp8266/0.1\r\n");
   client.print("Host: " + host + ":8086\r\n");
   client.print("Accept: */*\r\n");
   client.print("Content-Length: " + String(content.length()) + "\r\n");
   client.print("Content-Type: application/x-www-form-urlencoded\r\n");
   client.print("\r\n");
   client.print(content + "\r\n");

   Serial.println(content);

   client.flush();
}

void WiFiEvent(WiFiEvent_t event) {
    Serial.printf("[WiFi-event] event: %d\n", event);

    switch(event) {
        case WIFI_EVENT_STAMODE_GOT_IP:
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            break;
        case WIFI_EVENT_STAMODE_DISCONNECTED:
            Serial.println("WiFi lost connection");
            break;
    }
}

void loop() {
  // Wait one minute between measurements.
  delay(60000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  send_value("temperature", t);
  send_value("humidity", h);
  
  Serial.print("T: ");
  Serial.print(t);
  Serial.print(" H:");
  Serial.println(h);
}

