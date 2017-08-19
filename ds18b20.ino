

void ds18b20() {
  sensors.requestTemperatures();
  HTTP.sendHeader("Connection", "close");
  HTTP.sendHeader("Access-Control-Allow-Origin", "*");
String msg = dht.readHumidity();;
msg = "count: ";
msg += deviceCount;
msg += "\n";
for (int i = 0; i <= deviceCount - 1; i++){ 
      DeviceAddress Address18b20;
      sensors.getAddress(Address18b20, i);
      float temp = sensors.getTempC(Address18b20);
      msg += temp;

      msg += " | ";
      for(int i = 0; i < 8; i++) {
        msg += Address18b20[i];
      }
      msg += "\n";
   }  
  
  
  HTTP.send(200, "text/plain", msg);
}

