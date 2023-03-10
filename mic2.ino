

int sensorPin = 25;    // select the input pin for the potentiometer  
int sensorValue = 0; 
int mean = (1.62*4096)/3.3;
int thresh = (0.1*4096)/3.3;// variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  Serial1.begin(9600);
}

void loop() {

  sensorValue = analogRead(sensorPin);    
  
  if(sensorValue-mean>=thresh ||sensorValue-mean<= - thresh)
  {
    Serial1.println("#### Gros bruit #####");
  }
  else
  {
    Serial1.println("  ");
  }
   
}
