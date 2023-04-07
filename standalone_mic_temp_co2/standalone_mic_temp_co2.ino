//DHT lib
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 28     // Digital pin connected to the DHT sensor
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

  //bruit pénible
  float soundval =0;
  float soundArray[100] = {0};
  double meanSoundArray[100];
  double avgSound=0;
  int kk;
  int K = 5;

//convertor
  char buffer[12];
  char buffer_out[12];

//Display char
        char result[4]; 
//clear every 20 loops
int counter = 0;

//last vars 
float CO2_last;
float ISO_last;
float mic_last;
float temp_last;
float humi_last;



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
      DisplayString(100,6, "   W"); 
}

char* convertor(float val){
  itoa(int(val), buffer, 10);
  int dec = (val - (int)val) * 100;
  itoa(abs(val), buffer_out, 10);
  itoa(abs(dec), buffer, 10);
  //Serial.println(buffer);
  char point[2] = ".";
  strcat(buffer_out, point);
  strcat(buffer_out, buffer);
  return buffer_out;
  }



void clean_and_display(){
  float var_array[5]={humi_last, temp_last, ISO_last, CO2_last, mic_last};
  for(int i =2; i<=6; i++){
    DisplayString(68,i, "      "); 
    if(i == 6)
  {
    DisplayString(68,i, "        "); 
  }
    DisplayString(68,i, convertor(var_array[i-2])); 
  }

  
}

void loop() {



  

  //DHT READ by DHT lib
  // put your main code here, to run repeatedly: 
    humi_last = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temp_last = dht.readTemperature();



  
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

  //float val =   sensorValue = analogRead(sensorPin);
  //DisplayString(68,4, convertor(val)); 

  if (switchTime == 1 && state2){

    ratioCo2 = (onTime*100/(offTime+onTime));

    if(ratioCo2 <= 50){
        //ISO
         ISO_last = (ratioCo2-5)*25;
 
      }

    else{
      //CO2
         CO2_last = ((ratioCo2)-55)*40 +400;
      }
      
      
      
    state2 = false;
    onTime = 0;
    offTime = 0;
    switchTime = 0;     
  } 

  
  //Mic
  //checkout https://electronics.stackexchange.com/questions/211426/calculating-spl-from-voltage-output-of-a-microphone-with-max4466-amplifier
  //for detailed conversion to db
  sensorValueMic = analogRead(sensorPinMic);    
  if(counter%50 == 0){
      soundArray[counter/50] = (sensorValueMic-2048.0)/4096;
  }

  //Serial.println( (sensorValueMic-2048.0)/4096);
    
  //mic_last = ((sensorValueMic*3.3/4096)-1.62)*1000;

  
  //mean calculation
  

    
  if(counter>= 5000){
  
  for (int i = K; i < counter/50-K; i++) {
      double x[2*K+1];
      for (int j = 0; j <= 2*K; j++) {
        x[j] = soundArray[i-K+j];

      }
      double sum = 0.0;
      for (int j = 0; j <= 2*K; j++) {
        //Serial.println(x[j]);
        //Serial.println(x[j]*x[j]);
        sum += x[j]*x[j];


      }
  
  meanSoundArray[i] = sum / (2*K+1);
  //Serial.println(sum);
  //Serial.println((2*K+1));
  }
avgSound=0;

  for(int i = 0; i<counter/50; i++){
    avgSound += meanSoundArray[i]/(counter/50);

  }

  mic_last = avgSound*5000;
    
    clean_and_display();
  counter=0;
  }else{counter++;}
  
  
}
