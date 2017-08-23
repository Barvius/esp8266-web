void HTTP_init(void) {
  // Включаем работу с файловой системой



  //Создание ответа
  HTTP.on("/raw", raw); // обрашение к реле через web интерфейс
  HTTP.on("/restart", restart);
  HTTP.on("/sys", sys);
  HTTP.on("/ds", ds_config);
  HTTP.on("/dht", dht_config);
  HTTP.on("/bmp", bmp_config);
  HTTP.on("/nm", nm_config);
  HTTP.on("/hostname", hostname_config);

  //  HTTP.on("/ds18b20", handle_Set_DS18b20_Pin);
  //  HTTP.on("/irc", irControlweb);   // обрашение к ИК через web интерфейс
  //  HTTP.on("/Time", handle_Time); // обрашение к реле через web интерфейс
  //  HTTP.on("/xml",handleXML); // формирование xml страницы для передачи данных в web интерфейс
  // Добавляем функцию Update для перезаписи прошивки
  update();
  // Запускаем HTTP сервер
  HTTP.begin();
}


void restart() {
  HTTP.send(200, "text / plain", "Reset OK");
  ESP.restart();
}
void sys() {
  FSInfo fs_info;
  SPIFFS.info(fs_info);

  HTTP.sendHeader("Connection", "close");
  HTTP.sendHeader("Access-Control-Allow-Origin", "*");
  String json = "{";
  json += "\"mac\":\"";
  json += WiFi.macAddress();
  json += "\",\"ip\":\"";
  json +=  WiFi.localIP().toString();
  json += "\",\"ssid\":\"";
  json += WiFi.SSID();
  json += "\",\"rssi\":";
  json += WiFi.RSSI();
  json += ",\"uptime\":";
  json += millis();
  json += ",\"vcc\":";
  json +=  ESP.getVcc();
  json += "}";

  HTTP.send(200, "text/json", json);
}

void hostname_config() {
  if (HTTP.argName(0) == "name") {
    Hostname = HTTP.arg("name");
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
    ESP.restart();
  }
}
void ds_config() {
  if (HTTP.argName(0) == "en") {
    DS_EN = HTTP.arg("en").toInt();
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
}
void dht_config() {
  if (HTTP.argName(0) == "en") {
    DHT_EN = HTTP.arg("en").toInt();
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
}
void bmp_config() {
  if (HTTP.argName(0) == "en") {
    BMP_EN = HTTP.arg("en").toInt();
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
}

void nm_config() {
  if (HTTP.argName(0) == "en") {
    NM_EN = HTTP.arg("en").toInt();
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
  if (HTTP.argName(0) == "interval") {
    NM_INTERVAL = HTTP.arg("interval").toInt();
    saveConfig();
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
  }
}
//void handle_Set_DS18b20_Pin() {
//  DS18B20_PIN = HTTP.arg("pin").toInt(); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
//  saveConfig();                 // Функция сохранения данных во Flash пока пустая
//  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
//}
