#include <VirtualWire.h>

#include "rfcom.h"

rfcom rfcom_obj;
int i = 0;
void setup() {
  Serial.begin(9600);
  Serial.print("Go !"); 
  Serial.println('\n');
  rfcom_obj.init();
  rfcom_obj.start_listening();
}

void loop() {
  
  //byte message[RFCOM_MAX_MESSAGE_LENGTH];
  //byte taille_message = RFCOM_MAX_MESSAGE_LENGTH;
  Serial.print("reading "); // Affiche le message
  //Serial.println('\n');
  String message = rfcom_obj.get_message(1000);
  if (message.length()>0) {
    Serial.println(message); // Affiche le message
    Serial.println('\n'); 
  }
  
}
