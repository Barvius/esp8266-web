

void raw() {
   digitalWrite(12,LOW);
   //sensors.begin();
  //deviceCount = sensors.getDeviceCount();  // узнаем количество подключенных градусников
  //sensors.requestTemperatures();
  HTTP.sendHeader("Connection", "close");
  HTTP.sendHeader("Access-Control-Allow-Origin", "*");
  String json = "{";

 json += "\"dht\":{";
 json += "\"t\":";
 json += dht.readTemperature();
 json += ",\"h\":";
 json += dht.readHumidity();
 json += "},\"bmp\":{";
 json += "\"t\":";
 json += bmp.readTemperature();
 json += ",\"p\":";
 json += bmp.readPressure() / 133.3;
 json += "}}";

 
 HTTP.send(200, "text/json", json);
// 
//String msg;
//msg = "count: ";
//msg += deviceCount;
//msg += "\n";
//msg += dht.readHumidity();
//msg += "aaa\n";
//for (int i = 0; i <= deviceCount - 1; i++){ 
//      DeviceAddress Address18b20;
//      sensors.getAddress(Address18b20, i);
//      float temp = sensors.getTempC(Address18b20);
//      msg += temp;
//
//      msg += " | ";
//      for(int i = 0; i < 8; i++) {
//        msg += Address18b20[i];
//      }
//      msg += "\n";
//   }  
//  
//  
//  HTTP.send(200, "text/plain", msg);
}

