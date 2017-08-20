void HTTP_init(void) {
  // Включаем работу с файловой системой



  //Создание ответа
  HTTP.on("/raw", raw); // обрашение к реле через web интерфейс
  HTTP.on("/restart", restart); 
  HTTP.on("/sys", sys); 
//  HTTP.on("/dht", handle_Set_Dht_Pin);
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
void sys(){
  FSInfo fs_info;
  SPIFFS.info(fs_info);
  
  HTTP.sendHeader("Connection", "close");
  HTTP.sendHeader("Access-Control-Allow-Origin", "*");
  String json = "{";

 json += "\"fm\":";

 json += ESP.getFreeHeap();
 json += ",\"m\":";

 json += ESP.getFlashChipSize();
 json += ",\"f\":";

 json +=  ESP.getSketchSize();

 json += ",\"used\":";
 json +=  fs_info.usedBytes;
 json += ",\"all\":";
 json +=  fs_info.totalBytes;
  json += ",\"physical\":\"";
 json += WiFi.macAddress();

 json += "\"}";

 HTTP.send(200, "text/json", json);
}

//void handle_Set_Dht_Pin() {
//  DHT_PIN = HTTP.arg("pin").toInt(); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
//  saveConfig();                 // Функция сохранения данных во Flash пока пустая
//  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
//}
//void handle_Set_DS18b20_Pin() {
//  DS18B20_PIN = HTTP.arg("pin").toInt(); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
//  saveConfig();                 // Функция сохранения данных во Flash пока пустая
//  HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
//}