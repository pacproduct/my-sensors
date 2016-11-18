/* rfcom library

*/

#include "rfcom.h"

rfcom::rfcom() {
}

void rfcom::init(void){ 
  vw_setup(RFCOM_SPEED);
  //Serial.println("vw_setup");
}
   
void rfcom::start_listening(void){
  vw_rx_start();
  //Serial.println("vw_rx_start"); 
}
void rfcom::stop_listening(void){
  vw_rx_stop();
}



int rfcom::send_message(String message){
  if (message.length() > RFCOM_MAX_MESSAGE_LENGTH) {
    return 0;
  }
  
  byte paquet[message.length()+1];
  message.getBytes(paquet, message.length()+1);
  //Serial.println((char*)paquet); 
  vw_send(paquet, message.length()); // On envoie le message
  vw_wait_tx(); // Attente de la fin de l'envoi
  return 1;
}

String rfcom::get_message(unsigned long milliseconds) {
  //Serial.println("get_message");
  if (milliseconds>0) {
    vw_wait_rx_max(milliseconds);
  }
  String message="";
  if (vw_have_message()) {
   //Serial.println("vw_have_message"); 
    byte message_buf[VW_MAX_MESSAGE_LEN+1];
    uint8_t taille_message = VW_MAX_MESSAGE_LEN;
    if (vw_get_message(message_buf, &taille_message)) {
      // On copie le message, qu'il soit corrompu ou non
      message_buf[taille_message] = '\0';
	  //Serial.println("vw_get_message"); 
      message = String((char*) message_buf); // Affiche le message
    }
  }
  return message;
}
