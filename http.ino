void HTTP_init(void) {
  // Включаем работу с файловой системой
  FS_init();

  
  //Создание ответа
  HTTP.on("/ds", ds18b20); // обрашение к реле через web интерфейс
//  HTTP.on("/irc", irControlweb);   // обрашение к ИК через web интерфейс
//  HTTP.on("/Time", handle_Time); // обрашение к реле через web интерфейс
//  HTTP.on("/xml",handleXML); // формирование xml страницы для передачи данных в web интерфейс
  // Добавляем функцию Update для перезаписи прошивки
  update();
  // Запускаем HTTP сервер
  HTTP.begin();
}




