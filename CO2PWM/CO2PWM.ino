

int sensorPin = 23;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
float onTime = 0;  
float offTime = 0; 
int switchTime = 0;
float ratioCo2 = 0;
float ratioGaz = 0;
bool state = 0;
bool state2 = 0;

void setup() {
  Serial.begin(250000);
}

void loop() {

    sensorValue = analogRead(sensorPin);
    Serial.println(sensorValue);
  if(sensorValue < 200){
    offTime += 1;
    state = true;
  } else if (state){
    state = false;
    switchTime +=1;
    state2 = true;
  }else if (sensorValue > 3500){
    onTime += 1;
  }

  if (switchTime == 1 && state2){

    ratioCo2 = (onTime*100/(offTime+onTime));

    if(ratioCo2 <= 50){
      Serial.print(" ISOBUTYLENE ");
      Serial.print((ratioCo2-5)*25 );
      Serial.println(" ppb");
      }

    else{
      Serial.print(" CO2 :");
      Serial.print(((ratioCo2)-55)*40 +400 );
      Serial.println(" ppm");
      }
      
    state2 = false;
    onTime = 0;
    offTime = 0;
    switchTime = 0;

    

     
  }  } 
  


    
