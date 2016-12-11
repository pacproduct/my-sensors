#include <PJON.h>
#include <PJONDefines.h>
#include <PJONMaster.h>
#include <PJONSlave.h>

void setup() {
  Serial.begin(115200);
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
