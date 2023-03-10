/*
  Blink
  The basic Energia example.
  Turns on an LED on for one second, then off for one second, repeatedly.
  Change the LED define to blink other LEDs.
  
  Hardware Required:
  * LaunchPad with an LED
  
  This example code is in the public domain.
*/

// most launchpads have a red LED
#define V_LED 28
#define B_LED 27
#define R_LED 29

//see pins_energia.h for more LED definitions
//#define LED GREEN_LED
  
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(V_LED, OUTPUT);
  pinMode(R_LED, OUTPUT); 
  pinMode(B_LED, OUTPUT);    
}

// the loop routine runs over and over again forever:
void loop() {

  digitalWrite(B_LED, HIGH); 
  digitalWrite(V_LED, LOW);
  digitalWrite(R_LED, LOW);  
  delay(1000);  

    digitalWrite(B_LED, LOW); 
  digitalWrite(V_LED, LOW);
  digitalWrite(R_LED, HIGH);  
  delay(1000);  
  
    digitalWrite(B_LED, LOW); 
  digitalWrite(V_LED, HIGH);
  digitalWrite(R_LED, LOW);  
  delay(1000);  
  
  digitalWrite(B_LED, LOW); 
  digitalWrite(V_LED, HIGH);
  digitalWrite(R_LED, HIGH);  
  delay(1000);  
}
