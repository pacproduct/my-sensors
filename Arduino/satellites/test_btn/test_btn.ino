#include <EEPROM.h>

int led = 13; // choose the pin for the LED
int btn = 7;   // choose the input pin (for a pushbutton)

int btn_state = 0;     // variable for reading the pin status

int addr = 0;
/*void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  
  Serial.println("test eprom");
  
  Serial.println(EEPROM.length());
  Serial.println(EEPROM.read(addr));
  
  pinMode(led, OUTPUT);  // declare LED as output
  pinMode(btn, INPUT);    // declare pushbutton as input
  digitalWrite(btn, HIGH); // connect internal pull-up
}*/

void loop() {
  // put your main code here, to run repeatedly:

/*
  val = digitalRead(inPin);  // read input value
  if (val == HIGH) {         // check if the input is HIGH (button released)
    digitalWrite(ledPin, LOW);  // turn LED OFF
  } else {
    digitalWrite(ledPin, HIGH);  // turn LED ON
  }*/
  
  checkButton();

}


/* Click and Press+Hold Test Sketch
By Jeff Saltzman
To keep input interfaces simple, I want to use a single button to:
1) click (fast press and release) for regular button use, and
2) press and hold to enter a configuration mode.
*/

#define buttonPin 7 // analog input pin to use as a digital input
#define ledPin 13 // digital output pin for LED indicator

#define debounce 20 // ms debounce period to prevent flickering when pressing or releasing the button
#define holdTime 2000 // ms hold period: how long to wait for press+hold event

// Button variables
int buttonVal = 0; // value read from button
int buttonLast = 0; // buffered value of the button's previous state
long btnDnTime; // time the button was pressed down
long btnUpTime; // time the button was released
boolean ignoreUp = false; // whether to ignore the button release because the click+hold was triggered

// LED variables
boolean ledVal = false; // state of LED


//=================================================


void setup() 
{

// Set button input pin
pinMode(buttonPin, INPUT);
digitalWrite(buttonPin, HIGH );

// Set LED output pins
pinMode(ledPin, OUTPUT);
digitalWrite(ledPin, ledVal);

  Serial.begin(115200);
  
  Serial.println("check btn");
  
  //Serial.println(EEPROM.length());
  //Serial.println(EEPROM.read(addr));
  
}


//=================================================


void checkButton() {

  // Read the state of the button
  buttonVal = digitalRead(buttonPin);
  
  // Test for button pressed and store the down time
  if (buttonVal == LOW && buttonLast == HIGH && (millis() - btnUpTime) > long(debounce)) {
    btnDnTime = millis();
  }
  
  // Test for button release and store the up time
  if (buttonVal == HIGH && buttonLast == LOW && (millis() - btnDnTime) > long(debounce)) {
    if (ignoreUp == false) {
      //event1();
      Serial.println("short press");
    } else {
      ignoreUp = false;
    }
    btnUpTime = millis();
  }
  
  // Test for button held down for longer than the hold time
  if (buttonVal == LOW && (millis() - btnDnTime) > long(holdTime)) {
      Serial.println("long press");
    configure();
    ignoreUp = true;
    btnDnTime = millis();
  }
  
  buttonLast = buttonVal;
}


//=================================================
// Events to trigger by click and press+hold
/*
void event1()
{
ledVal1 = !ledVal1;
digitalWrite(ledPin1, ledVal1);
}*/

void configure() {
  ledVal = !ledVal;
  digitalWrite(ledPin, ledVal);
}
