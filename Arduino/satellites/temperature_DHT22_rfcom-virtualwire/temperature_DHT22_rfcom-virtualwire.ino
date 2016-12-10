#include <DHT.h>

#include <rfcom.h>

#include <VirtualWire.h>
#include <VirtualWire_Config.h>

#define DHTPIN 8
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

rfcom rfcom_obj;
float h = NAN;
float t = NAN;
char t_buf[6];
char h_buf[6];

void setup() {
  //Serial.begin(9600);
  //Serial.println("start");
  rfcom_obj.init();
  dht.begin();
  //Serial.println("started");
  delay(500);
}

void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (!isnan(h) && !isnan(t)) {
    dtostrf(t, 0, 1, t_buf);
    dtostrf(h, 0, 1, h_buf);
    //Serial.print("[");
    //Serial.print(t_buf.'|'.h_buf);
    //Serial.println("]");
    rfcom_obj.send_message(String(t_buf)+"|"+String(h_buf));//.concat('|').concat(h,3));
  }
  delay(5000);
}
