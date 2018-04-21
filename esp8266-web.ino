#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <FS.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

#include <PubSubClient.h>

#define DHT_PIN 13
#define DS18B20_PIN 12


ADC_MODE(ADC_VCC);

//String _ssid; // Для хранения SSID
//String _password; // Для хранения пароля сети
//String _ssidAP = "WiFi";   // SSID AP точки доступа
//String _passwordAP = ""; // пароль точки доступа
String Hostname;
bool DS_EN;
bool DHT_EN;
bool BMP_EN;
bool NM_EN;
int NM_INTERVAL;
int Timezone;               // часовой пояс GTM

String M_Server;
int M_Port;
String M_User;
String M_Password;

unsigned long LastSendTimer_0 = 0;

OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);
DHT dht(DHT_PIN, DHT11);
//Adafruit_BMP085 bmp;

Adafruit_BME280 bme;

ESP8266HTTPUpdateServer httpUpdater;
// Web интерфейс для устройства
ESP8266WebServer HTTP(80);
// Для файловой системы
File fsUploadFile;

WiFiClient wclient;
PubSubClient client(wclient);

void mqttConnect(){
   if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {

      if (client.connect(MQTT::Connect(WiFi.macAddress()).set_auth(M_User, M_Password))) {
        Serial.println("Connected to MQTT server ");
         client.subscribe("Debug");
      } else {
        Serial.println("Could not connect to MQTT server");
      }
    }
  }

  String Sys = WiFi.macAddress();
  Sys += " -- ";
  Sys += WiFi.localIP().toString();
  Sys += " -- (";
  Sys += WiFi.RSSI();
  Sys += ")";
  client.publish("Info", Sys);
  
}

void callback(const MQTT::Publish& pub)
{
  String payload = pub.payload_string();
  String topic = pub.topic();

  Serial.print(pub.topic()); // выводим в сериал порт название топика
  Serial.print(" => ");
  Serial.println(payload); // выводим в сериал порт значение полученных данных
 
}

unsigned long timer = 0;
void refreshData() {
  if (millis() - timer > 30000) {
  DeviceAddress tempDeviceAddress;
  
  int deviceCount = sensors.getDeviceCount();  // узнаем количество подключенных градусников
  sensors.requestTemperatures();
  if (DHT_EN) {
     client.publish("Data/DHT1/t", String(dht.readTemperature()));
     client.publish("Data/DHT1/h", String(dht.readHumidity()));
  }
  if (BMP_EN) {
     client.publish("Data/BMP/t", String(bme.readTemperature()));
     client.publish("Data/BMP/p", String(bme.readPressure() / 133.3));
      client.publish("Data/BMP/h", String(bme.readHumidity()));
  }
  if (DS_EN) {
      String buf;
    for (int i = 0; i < deviceCount; i++)  { //перечисляем датчики и их показания
      sensors.getAddress(tempDeviceAddress, i);
    
      buf="";
      for (uint8_t i = 0; i < 8; i++) {
        if (tempDeviceAddress[i] < 16) buf += "0";  // адрес датчика
        buf += String(tempDeviceAddress[i], HEX);
      }
      client.publish("Data/DS/"+buf, String(sensors.getTempCByIndex(i)));
    }
  }
  timer=millis();
  }
}

void setup() {
  // Настраиваем вывод отладки
  Serial.begin(115200);


  //настраиваем HTTP интерфейс


  FS_init();
  loadConfig();
  client.set_server(M_Server, M_Port);
  client.set_callback(callback);
  
  //WiFi.hostname(Hostname);

  //Включаем WiFiManager
  WiFiManager wifiManager;

  //Если не удалось подключиться клиентом запускаем режим AP
  // доступ к настройкам по адресу http://192.168.4.1
  wifiManager.autoConnect("AutoConnectAP");

  //если подключение к точке доступа произошло сообщаем
  Serial.println("connected...yeey :)");
  HTTP_init();
  Time_init();
  sensors.begin();
  sensors.setResolution(12);
  dht.begin();



  if (!bme.begin()) {
    Serial.println("Could not find a valid BMP180 sensor, check wiring!");
  }

}




unsigned long intervalsend = 0;

void snd() {
  if (millis() - intervalsend > 20000) {
    HTTPClient httpClient;


    int deviceCount = sensors.getDeviceCount();  // узнаем количество подключенных градусников
    sensors.requestTemperatures();

    String req = "cid=" + String(ESP.getChipId());

    req += "&humidity_"+String(ESP.getChipId())+"=";
    req += bme.readHumidity();
    req += "&pressure_"+String(ESP.getChipId())+"=";
    req += bme.readPressure() / 133.3;
    for (int i = 0; i <= deviceCount - 1; i++) {
      DeviceAddress Address18b20;
      sensors.getAddress(Address18b20, i);
      String tmp = "";
      String t = "";
      for (int i = 0; i < 8; i++) {
        t = String(Address18b20[i], HEX);
        while ( t.length() < 2)  t = "0" +  t;
        tmp += t;
      }
      req += "&" +tmp+"=";
      req += sensors.getTempC(Address18b20);
    }
    httpClient.begin("http://192.168.1.141/update.php?" + req);
    httpClient.GET();
    httpClient.end();
    intervalsend = millis();
  }

}

void loop() {
snd();
  HTTP.handleClient();
  delay(1);
if (client.connected()) {
    client.loop();
    refreshData();
  } else {
    mqttConnect();
  }
  
  if (millis() - LastSendTimer_0 > NM_INTERVAL && NM_EN) {
    if (WiFi.status() == WL_CONNECTED) {
      if (Narodmon()) {
        LastSendTimer_0 = millis();
      } else {
        LastSendTimer_0 = millis() - NM_INTERVAL + 15000;
      }
    } else {
      LastSendTimer_0 = millis() - NM_INTERVAL + 15000;
      Serial.println("Not connected to WiFi");
    }
  }
}
