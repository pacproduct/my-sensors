#include <rfcom.h>

#include <VirtualWire.h>
#include <VirtualWire_Config.h>


rfcom rfcom_obj;
char vcc_voltage_buf[6];
long vcc_voltage;//readvcc



// number of analog samples to take per reading
#define NUM_SAMPLES 10

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage
char voltage_buf[6];

int battery_pin = A2;

void setup() {
  //Serial.begin(9600);
  //Serial.println("start");
  rfcom_obj.init();
  //Serial.println("started");
  delay(500);
}

void loop() {
  //vcc_voltage = readVcc();
  //dtostrf(vcc_voltage, 0, 1, vcc_voltage_buf);
  //rfcom_obj.send_message(String(vcc_voltage_buf));

  
  readVoltagePin();
  dtostrf(voltage, 0, 2, voltage_buf);
  rfcom_obj.send_message(String(voltage_buf));
  
  delay(5000);
}

void readVoltagePin() {
  // take a number of analog samples and add them up
    while (sample_count < NUM_SAMPLES) {
        sum += analogRead(battery_pin);
        sample_count++;
        delay(10);
    }
    // calculate the voltage
    // use 5.0 for a 5.0V ADC reference voltage
    // 5.015V is the calibrated reference voltage
    voltage = ((float)sum / (float)NUM_SAMPLES * 5.09) / 1024.0;

    sample_count = 0;
    sum = 0;
}
long readVcc() {
  // Read 1.1V reference against AVcc
  // set the reference to Vcc and the measurement to the internal 1.1V reference
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif  

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH  
  uint8_t high = ADCH; // unlocks both

  long result = (high<<8) | low;

  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}
