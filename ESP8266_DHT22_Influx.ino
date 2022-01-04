/**
 * Example for reading temperature and humidity
 * using the DHT22 and ESP8266
 *
 * Copyright (c) 2016 Losant IoT. All rights reserved.
 * https://www.losant.com
 */

#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <InfluxDb.h>

#define DHTPIN 4     // what digital pin the DHT22 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
#define INFLUXDB_HOST "192.168.1.225"   //Enter IP of device running Influx Database
#define WIFI_SSID "Homeland Security"              //Enter SSID of your WIFI Access Point
#define WIFI_PASS "jade103andruby"          //Enter Password of your WIFI Access Point

ESP8266WiFiMulti WiFiMulti;
Influxdb influx(INFLUXDB_HOST);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);
  // Start the DS18B20 sensor
  //sensors.begin();
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WIFI");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
    }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  influx.setDb("backrm");

  // Wait for serial to initialize.
  while(!Serial) { }
  
  dht.begin();

  Serial.println("Device Started");
  Serial.println("-------------------------------------");
  Serial.println("Running DHT!");
  Serial.println("-------------------------------------");

}

int timeSinceLastRead = 0;
void loop() {

  // Report every 60 seconds.
  if(timeSinceLastRead > 60000) 
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float humid = dht.readHumidity(true);
    // Read temperature as Celsius (the default)
    float F = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float tempf = dht.readTemperature(true);

     // sensors.requestTemperatures(); 
  //float temperatureC = sensors.getTempCByIndex(0);
  //float temperatureF = sensors.getTempFByIndex(0);
  //Serial.print(temperatureC);
  //Serial.println("ºC");
  Serial.print(tempf);
  Serial.println("ºF");

  
  InfluxData row("data");
  row.addTag("Device", "ESP8266");
  row.addTag("Sensor", "Temp");
  row.addTag("Unit", "F");
  row.addTag("Sensor2", "Hum");
  row.addTag("Unit", "H");
  //You can change the data being sent to influx either ºC or ºF here
  row.addField("Temp", tempf);
  row.addField("Hum", humid);
  
  influx.write(row);
  delay(60000);
 
}
