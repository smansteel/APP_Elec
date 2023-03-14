//DHT lib
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN PE_2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//Screen lib by ISEP
#include "blank.h"
#include "Iseplogo128.h"

//CO2
int sensorPin = 23;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
float onTime = 0;  
float offTime = 0; 
int switchTime = 0;
float ratioCo2 = 0;
float ratioGaz = 0;
bool state = 0;
bool state2 = 0;



//Display char
        char result[4]; 



void setup() {
  Serial.begin(250000);


//screen init
  InitI2C();
  InitScreen();

  Display(motif);                           // affichage de l'image décrite dans le tabelau de donnée motif.h

  delay(1000);

  Display(blank);  
  DisplayString(35,0,"AirQ Sensor");   

  //DHT init

    dht.begin();
}

char * convertor(float val){
  char sz[4] = {' '} ;

  int val_int = (int) val;   // compute the integer part of the float


  sprintf (sz, "%d", val_int); 
  return sz;
  
  
  
  
  }

void loop() {

  //DHT READ by DHT lib
  // put your main code here, to run repeatedly: 
    float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  //Display result of DHT Read

      dtostrf(h, 3, 1, result);

      DisplayString(0,2,"Humidity : ");   
      DisplayString(56,2, convertor(h) ); 
      DisplayString(80,2, " %"); 

      

      dtostrf(t, 3, 1, result);

      DisplayString(0,3,"Temp : ");   
      DisplayString(56,3, result ); 
      DisplayString(80,3, " °C"); 

  
//CO2 ISO PWN CODE
  sensorValue = analogRead(sensorPin);

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

      dtostrf((ratioCo2-5)*25, 3, 1, result);

      DisplayString(0,4,"ISO : ");   
      DisplayString(56,4, String((ratioCo2-5)*25).c_str ); 
      DisplayString(80,4, " ppb"); 

        
  
      }

    else{

      dtostrf(((ratioCo2)-55)*40 +400, 3, 1, result);

      DisplayString(0,5,"CO2 : ");   
      DisplayString(56,5, String(((ratioCo2)-55)*40 +400).c_str ); 
      DisplayString(80,5, " ppm"); 


      }
      
    state2 = false;
    onTime = 0;
    offTime = 0;
    switchTime = 0;

    

     
  } 

  
}
