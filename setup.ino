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

  sensors.begin();
  deviceCount = sensors.getDeviceCount();  // узнаем количество подключенных градусников
  //  sensors.getAddress(Address18b20, 0);
  sensors.setResolution(12); 
dht.begin();
}
