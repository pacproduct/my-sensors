#include <rfcom.h>

#include <VirtualWire.h>
#include <VirtualWire_Config.h>


rfcom rfcom_obj;
void setup() {
  Serial.begin(9600);
  rfcom_obj.init();
  rfcom_obj.start_listening();
}

void loop() {
  
  //byte message[RFCOM_MAX_MESSAGE_LENGTH];
  //byte taille_message = RFCOM_MAX_MESSAGE_LENGTH;
  //Serial.print("reading "); // Affiche le message
  //Serial.println('\n');
  String message = rfcom_obj.get_message(1000);
  if (message.length()>0) {
    Serial.print(message); // Affiche le message
    Serial.print('\n'); 
  }
  
}
