#include <Adafruit_Sensor.h>

// DHT lib

#include "DHT.h"
#include <DHT_U.h>

DHT dht(PE_2, DHT11);

// Screen lib by ISEP
#include "blank.h"
#include "Iseplogo128.h"

// Capteur cardiaque
int LEDPin = 12;
int sensorCardiacPin = 24;
int cardiacValue = 0;
float onTimeCardiac = 0;
float offTimeCardiac = 0;
int switchTimeCardiac = 0;
bool stateCardiac = 0;
bool state2Cardiac = 0;
float ratioCardiac = 0;
int counterCardiac = 0;
int histCardiac[100] = {};
int cardiacTime = 0;
int cardiacTemp = 0;
bool isLastCycleTrue = false;
int meanCardiac[10] = {};
int cardiacMeanCounter = 0;
float meanCardiacFl = 0.0;

// CO2
int sensorPin = 23;  // select the input pin for the potentiometer
int sensorValue = 0; // variable to store the value coming from the sensor
float onTime = 0;
float offTime = 0;
int switchTime = 0;
float ratioCo2 = 0;
float ratioGaz = 0;
bool state = 0;
bool state2 = 0;

// mic
int sensorPinMic = 25; // select the input pin for the potentiometer
int sensorValueMic = 0;
int mean = (1.62 * 4096) / 3.3;
int thresh = (0.1 * 4096) / 3.3; // variable to store the value coming from the sensor

// bruit pénible
float soundval = 0;
float soundArray[100] = {0};
double meanSoundArray[100];
double avgSound = 0;
int kk;
int K = 5;

// convertor
char buffer[12];
char buffer_out[12];

// Display char
char result[4];
// clear every 20 loops
int counter = 0;

// last vars
float CO2_last;
float ISO_last;
float mic_last;
float temp_last;
float humi_last;
float cardiac_last;

//SYNC

String TRA = "1";
String OBJ = "6969";
String REQ = "1";
String NUM = "01";
String TYP ;
String VAL ;
String CHK = "00" ;
String trame;


void setup()
{
  cardiacTime = millis();
  Serial1.begin(9600);
  // screen init
  InitI2C();
  InitScreen();

  Display(motif); // affichage de l'image décrite dans le tabelau de donnée motif.h

  delay(2000);

  Display(blank);
  DisplayString(35, 0, "AirQ Sensor");
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LOW);

  // DHT init

  dht.begin();

  Serial.begin(9600);

  // Base display

  DisplayString(0, 2, "Humidity  :");
  DisplayString(100, 2, " %");

  DisplayString(0, 3, "Temperatur:");
  DisplayString(100, 3, "°C");

  DisplayString(0, 4, "Isobutylen:");
  DisplayString(100, 4, " ppb");

  DisplayString(0, 5, "CarbonDiox:");
  DisplayString(100, 5, " ppm");

  DisplayString(0, 6, "Microphone:");
  DisplayString(100, 6, " mW");

  DisplayString(0, 7, "BPM du porteur:");
}

char *convertor(float val)
{
  itoa(int(val), buffer, 10);
  int dec = (val - (int)val) * 100;
  itoa(abs(val), buffer_out, 10);
  itoa(abs(dec), buffer, 10);
  // Serial.println(buffer);
  char point[2] = ".";
  strcat(buffer_out, point);
  strcat(buffer_out, buffer);
  return buffer_out;
}

char *convertor_int(float val)
{
  itoa(int(val), buffer, 10);
  itoa(abs(val), buffer_out, 10);
  return buffer_out;
}

void clean_and_display()
{
  float var_array[6] = {humi_last, temp_last, ISO_last, CO2_last, mic_last, cardiac_last};
  for (int i = 2; i <= 7; i++)
  {
    if (i == 7)
    {
      DisplayString(96, i, "      ");
    }
    else
    {
      DisplayString(68, i, "      ");
      if (i == 6)
      {
        DisplayString(68, i, "      ");
      }
    }
    if (i == 7)
    {
      DisplayString(96, i, convertor_int(var_array[i - 2]));
    }
    else
    {

      DisplayString(68, i, convertor(var_array[i - 2]));
    }
  }

    for (int i = 0; i <= 5; i++)
    {
      
      switch (i)
      {
      case 1:
        TYP = "3";
        sendTrame( (int) temp_last);
        break;
      case 2:
        TYP = "1";
        sendTrame( (int) ISO_last);
        break;
      case 3:
        TYP = "2";
        sendTrame( (int) CO2_last);
        break;
      case 4:
        TYP = "5";
        sendTrame( (int) mic_last*100);
      case 5:
        TYP = "6";
        sendTrame( (int) cardiac_last);
        break;
      case 0:
        TYP = "4";
       sendTrame( (int) humi_last);
       break;
      }

       
      
    }
    
}
void sendTrame(int value) {
  // CHK peut être calculé en sommant les valeurs ASCII de tous les caractères de la trame.



  int minutes = millis() / 60000 ;
  int secondes = millis() % 60000 / 1000;
  char hexValue[3]; // 2 characters for the hexadecimal representation and 1 for the null terminator
  char hexMinutes[3];
  char hexSecondes[3];
  sprintf(hexMinutes, "%02X", minutes);
  sprintf(hexSecondes, "%02X", secondes);
  if(value<0){
    value = 0;
  }
  sprintf(hexValue, "%02X", value);
  if(TYP == "2"){
    
  }
  // Construction de la trame
  trame = TRA + OBJ + REQ + TYP + NUM + hexValue + hexMinutes + hexSecondes;
    int chkSum = 0;
  for (int i = 0; i < trame.length(); i++) {
    chkSum += trame[i];
  }
    if (CHK.length() < 2) {
    CHK = "0" + CHK;  // Add a leading zero if needed
  }
  CHK = String(chkSum, HEX);  // convertit la somme en une chaîne hexadécimale
  trame += CHK;

  // Envoi de la trame et attente de la réponse de la passerelle
  Serial1.println(trame);  // envoie la trame
  Serial.println(trame);
}


void loop()
{

  // DHT READ by DHT lib
  //  put your main code here, to run repeatedly:
  humi_last = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temp_last = dht.readTemperature();

  // CO2 ISO PWN CODE
  sensorValue = analogRead(sensorPin);

  if (sensorValue < 200)
  {
    offTime += 1;
    state = true;
  }
  else if (state)
  {
    state = false;
    switchTime += 1;
    state2 = true;
  }
  else if (sensorValue > 3500)
  {
    onTime += 1;
  }

  // float val =   sensorValue = analogRead(sensorPin);
  // DisplayString(68,4, convertor(val));

  if (switchTime == 1 && state2)
  {

    ratioCo2 = (onTime * 100 / (offTime + onTime));

    if (ratioCo2 <= 50)
    {
      // ISO
      ISO_last = (ratioCo2 - 5) * 25;
    }

    else
    {
      // CO2
      CO2_last = ((ratioCo2)-55) * 40 + 400;
    }

    state2 = false;
    onTime = 0;
    offTime = 0;
    switchTime = 0;
  }

  // Mic
  // checkout https://electronics.stackexchange.com/questions/211426/calculating-spl-from-voltage-output-of-a-microphone-with-max4466-amplifier
  // for detailed conversion to db
  sensorValueMic = analogRead(sensorPinMic);

  if (counter % 50 == 0)
  {
    soundArray[counter / 50] = (sensorValueMic - 2048.0) / 4096;
  }

  // Serial.println( (sensorValueMic-2048.0)/4096);

  // mic_last = ((sensorValueMic*3.3/4096)-1.62)*1000;

  // Cardiac Sensor

  if (counter % 100 == 0)
  {
    cardiacValue = analogRead(sensorCardiacPin);
    // Serial.print("cardiacValue :");
    // Serial.print(cardiacValue);
    // Serial.print(",");
    // Serial.print("mrancardiac :");
    // Serial.print(meanCardiac[0]);
    // Serial.print(",");
    // Serial.print("cardiac_last :");
    // Serial.println(cardiac_last);

    histCardiac[counter % 100] = cardiacValue;

    if ((histCardiac[counter % 100] - histCardiac[(counter % 100) + 1] > 1000) && (!isLastCycleTrue))
    {

      meanCardiac[cardiacMeanCounter] = millis() - cardiacTemp;

      cardiacTemp = millis();
      isLastCycleTrue = true;

      meanCardiacFl = 0.0;
      for (int i = 0; i < 10; i++)
      {
        meanCardiacFl += meanCardiac[i];
      }

      meanCardiacFl = meanCardiacFl / 10000;
      cardiac_last = 60 / meanCardiacFl;

      cardiacMeanCounter += 1;
      if (cardiacMeanCounter >= 10)
      {
        cardiacMeanCounter = 0;
      }
    }

    else if ((histCardiac[counter % 100] - histCardiac[(counter % 100) + 1] < 1000))
    {
      isLastCycleTrue = false;
    }
  }

  // Serial.println(ratioCardiac);

  // mean calculation

  if (counter >= 5000)
  {

    for (int i = K; i < counter / 50 - K; i++)
    {
      double x[2 * K + 1];
      for (int j = 0; j <= 2 * K; j++)
      {
        x[j] = soundArray[i - K + j];
      }
      double sum = 0.0;
      for (int j = 0; j <= 2 * K; j++)
      {
        // Serial.println(x[j]);
        // Serial.println(x[j]*x[j]);
        sum += x[j] * x[j];
      }

      meanSoundArray[i] = sum / (2 * K + 1);
      // Serial.println(sum);
      // Serial.println((2*K+1));
    }
    avgSound = 0;

    for (int i = 0; i < counter / 50; i++)
    {
      avgSound += meanSoundArray[i] / (counter / 50);
    }

    mic_last = avgSound * 1000;

    clean_and_display();
    counter = 0;
  }
  else
  {
    counter++;
  }
}
