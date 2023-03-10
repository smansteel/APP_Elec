

int sensorPin = 23;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
float onTime = 0;  
float offTime = 0; 
int switchTime = 0;
float ratioCo2 = 0;
float ratioGaz = 0;
bool state = 0;
bool state2 = 0;
int co2m = 0;


int readCO2(int co2m){


        co2m = 0;
    sensorValue = analogRead(sensorPin);
    while (!(co2m != 0)){
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

  if (switchTime == 1 && state2)
  {
    
    ratioCo2 = (onTime/(offTime+onTime));

      if(ratioCo2 <= 0.5){

        }

      else{
        co2m = ((ratioCo2)-55)*40 +400;

        }
   }
 }  state2 = false;
    onTime = 0;
    offTime = 0;
    switchTime = 0;
        return co2m;
}



void setup() {
  Serial.begin(250000);
}

void loop() {
  int test = readCO2;
    Serial.println(test);
  }  
  
 
    


  


    
