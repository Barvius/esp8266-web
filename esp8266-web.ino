#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <ESP8266SSDP.h>
#include <FS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>

#define ONE_WIRE_BUS 13
#define TEMPERATURE_PRECISION 12
#define DHTTYPE DHT11
DHT dht(DHTPin, DHTTYPE);
int deviceCount = 0;

// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
// Для файловой системы
File fsUploadFile;

OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

void loop() {
  // put your main code here, to run repeatedly:
  HTTP.handleClient();
  delay(1);
}





