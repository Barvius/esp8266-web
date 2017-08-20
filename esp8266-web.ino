#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <FS.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>

#define DHT_PIN 13
#define DS18B20_PIN 14

String _ssid; // Для хранения SSID
String _password; // Для хранения пароля сети
String _ssidAP = "WiFi";   // SSID AP точки доступа
String _passwordAP = ""; // пароль точки доступа
String hostname;
int timezone = 3;               // часовой пояс GTM

String jsonConfig = "{}";


int deviceCount;
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);
DHT dht(DHT_PIN, DHT11);
Adafruit_BMP085 bmp;
// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
// Для файловой системы
File fsUploadFile;

void setup() {
  // Настраиваем вывод отладки
  Serial.begin(115200);


  //Включаем WiFiManager
  WiFiManager wifiManager;

  //Если не удалось подключиться клиентом запускаем режим AP
  // доступ к настройкам по адресу http://192.168.4.1
  wifiManager.autoConnect("AutoConnectAP");

  //если подключение к точке доступа произошло сообщаем
  Serial.println("connected...yeey :)");
  //настраиваем HTTP интерфейс

  HTTP_init();
  FS_init();
  loadConfig();

  sensors.begin();
  dht.begin();
  deviceCount = sensors.getDeviceCount();  // узнаем количество подключенных градусников


  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");

  }



  //  sensors.getAddress(Address18b20, 0);
  //sensors.setResolution(12);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);


}





void loop() {
  // put your main code here, to run repeatedly:
  HTTP.handleClient();
  delay(1);
  digitalWrite(12, HIGH);
}
