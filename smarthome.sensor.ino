#include <InfluxDb.h>

#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11 

#define INFLUXDB_HOST "192.168.0.200"
#define INFLUXDB_DATABASE "sensor"

const char* ssid     = "orokmozgo";
const char* password = "C1rm1c1ca88";

const String host = "192.168.0.200";
char* mac = "n/a";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);
Influxdb influx(INFLUXDB_HOST); // port defaults to 8086


void setup() {
  Serial.begin(115200);
  delay(2000);

  WiFi.disconnect(true);
  delay(1000);
  
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
  WiFi.macAddress().toCharArray(mac, 18); 
  Serial.println(mac);  

  influx.setDb(INFLUXDB_DATABASE);
}

void prepare_data(String valueType, float value)
{
  InfluxData measurement (valueType);
  measurement.addTag("device", mac);
  measurement.addTag("sensor", "dht11");
  measurement.addValue("value", value);
  
  influx.prepare(measurement);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  prepare_data("temperature", t);
  prepare_data("humidity", h);

  influx.write();
  
  delay(60000);
}

