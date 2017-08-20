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
#define postingInterval  300000

String _ssid; // Для хранения SSID
String _password; // Для хранения пароля сети
String _ssidAP = "WiFi";   // SSID AP точки доступа
String _passwordAP = ""; // пароль точки доступа
String Hostname;
bool DS_EN;
bool DHT_EN;
bool BMP_EN;
bool NM_EN;
int NM_INTERVAL;
//String hostname;
int timezone = 3;               // часовой пояс GTM

String jsonConfig = "{}";
unsigned long lastConnectionTime = 0;

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
  sensors.setResolution(12);
  dht.begin();



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
  
  if (millis() - lastConnectionTime > NM_INTERVAL && NM_EN) { // ждем 5 минут и отправляем
      if (WiFi.status() == WL_CONNECTED) { // ну конечно если подключены
      if (Narodmon()) {
      lastConnectionTime = millis();
      }else{  lastConnectionTime = millis() - postingInterval + 15000; }//следующая попытка через 15 сек    
      }else{  lastConnectionTime = millis() - postingInterval + 15000; Serial.println("Not connected to WiFi");}//следующая попытка через 15 сек
    }
}
