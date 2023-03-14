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

//mic
  int sensorPinMic = 25;    // select the input pin for the potentiometer  
  int sensorValueMic = 0; 
  int mean = (1.62*4096)/3.3;
  int thresh = (0.1*4096)/3.3;// variable to store the value coming from the sensor

//convertor
  char char_output[10];

//Display char
        char result[4]; 



void setup() {
  Serial.begin(250000);


//screen init
  InitI2C();
  InitScreen();

  Display(motif);                           // affichage de l'image décrite dans le tabelau de donnée motif.h

  delay(00);

  Display(blank);  
  DisplayString(35,0,"AirQ Sensor");   

  //DHT init

    dht.begin();
}

char* convertor(float val){
  Serial.println(val);
   sprintf(char_output, "%.4f", val);
   Serial.println(char_output);
  return char_output;
  }

void loop() {


  //Base display 


      DisplayString(0,2,"Humidity  :");   
      DisplayString(100,2, " %"); 
      
      DisplayString(0,3,"Temperatur:");   
      DisplayString(100,3, "°C"); 

      DisplayString(0,4,"Isobutylen:");   
      DisplayString(100,4, " ppb"); 

      DisplayString(0,5,"CarbonDiox:");   
      DisplayString(100,5, " ppm"); 

      DisplayString(0, 6, "Microphone:");
      DisplayString(100,6, " vdif"); 
  

  //DHT READ by DHT lib
  // put your main code here, to run repeatedly: 
    float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  //Display result of DHT Read

      
      float potate = 266.7;
      DisplayString(70, 2, convertor(potate));
      //DisplayString(56,2, convertor(h) ); 
      //DisplayString(0,7, convertor(3));
      //DisplayString(56,3, result ); 

  
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


      //DisplayString(56,4, convertor((ratioCo2-5)*25)); 


        
  
      }

    else{

      dtostrf(((ratioCo2)-55)*40 +400, 3, 1, result);


      //DisplayString(56,5, convertor(((ratioCo2)-55)*40 +400) ); 



      }
      
    state2 = false;
    onTime = 0;
    offTime = 0;
    switchTime = 0;     
  } 
  //Mic
  
  sensorValue = analogRead(sensorPin);    
  if(sensorValue-mean>=thresh ||sensorValue-mean<= - thresh)
  {
    Serial1.println("#### Gros bruit #####");
    //DisplayString()
  }
  else
  {
    Serial1.println("  ");
  }

  
}
