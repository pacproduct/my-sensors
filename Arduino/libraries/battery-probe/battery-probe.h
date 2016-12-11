
#ifndef BATTERY_NUM_SAMPLES
	// number of analog samples to take per reading
	// do not exceed 255 without changing "sample_count" type
	// "sum" may need to be changed to long type if number of samples exceeds roughly 60
	// (sum might be 1023* BATTERY_NUM_SAMPLES)
	// /!\ must be > 0
	#define BATTERY_NUM_SAMPLES 10
#endif

#ifndef BATTERY_TEST_PIN
	// battery pin
	#define BATTERY_TEST_PIN A2
#endif

#ifndef BATTERY_CALIBRATED_VCC
	// calibrated vcc
	// /!\ must be > 0
	#define BATTERY_CALIBRATED_VCC 5.0
#endif

#ifndef BATTERY_SAMPLES_DELAY
	// delay between two samples
	#define BATTERY_SAMPLES_DELAY 10
#endif

// reads BATTERY_NUM_SAMPLES samples on pin BATTERY_TEST_PIN separated
// by BATTERY_SAMPLES_DELAY ms and computes the mean voltage as a float and returns it.
float readBatteryVoltage() {
	unsigned int total = 0; // total of samples taken
	// take a number of analog samples and add them up
    for (unsigned char i = 0; i < BATTERY_NUM_SAMPLES; i++) {
        total += analogRead(BATTERY_TEST_PIN);
        delay(BATTERY_SAMPLES_DELAY);
    }
    // calculate the voltage
    // use 5.0 for a 5.0V ADC reference voltage
    // 5.015V is the calibrated reference voltage
    float voltage = ((float)total / (float)BATTERY_NUM_SAMPLES * BATTERY_CALIBRATED_VCC) / 1024.0;
}