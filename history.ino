float History[10][6];

void push_history(int row, float val) {
  for (int i = 0; i < 6; i++) {
    History[row][i] = History[row][i + 1];
  }
  History[row][5] = val;
}

void history() { // Собственно формирование пакета и отправка.
  if (DHT_EN) {
    push_history(0, (float)dht.readTemperature());
    push_history(1, (float)dht.readHumidity());
  }
  if (BMP_EN) {
    push_history(2, bmp.readTemperature());
    push_history(3, bmp.readPressure() / 133.3);
  }
  if (DS_EN) {
    int deviceCount = sensors.getDeviceCount();  // узнаем количество подключенных градусников
    sensors.requestTemperatures();
    DeviceAddress tempDeviceAddress;
    for (int i = 0; i < deviceCount; i++)  {
      sensors.getAddress(tempDeviceAddress, i);
      push_history(4 + i, sensors.getTempCByIndex(i));
    }
  }

}



