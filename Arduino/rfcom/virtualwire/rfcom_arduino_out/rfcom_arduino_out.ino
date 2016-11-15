
#include "rfcom.h"
#include <VirtualWire.h>

rfcom rfcom_obj;
int i = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("Go !");
  rfcom_obj.init();
}

void loop() {
  i++;
  Serial.print("sending "); // Affiche le message
  Serial.println(i); // Affiche le message
  rfcom_obj.send_message(String(i));
  delay(500);
}
