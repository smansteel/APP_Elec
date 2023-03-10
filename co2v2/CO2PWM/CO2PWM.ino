

int sensorPin = 23;  // select the input pin for the potentiometer
int levels;
  int sensorValue = 0; 
  bool state = 0;
  bool state2 = 0;
  int co2level;
  int isolevel;
  float onTime = 0;
  float offTime = 0;
  int switchTime = 0;
  int ratioCo2;


int readCO2()
{


  while (true)
  {
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

    if (switchTime == 1 && state2)
    {

      ratioCo2 = (onTime * 100 / (offTime + onTime));

      if (ratioCo2 >= 50)
      {
        // co2
        co2level = ((ratioCo2)-55) * 40 + 400;
        Serial.print(" CO2 ");
        Serial.println(co2level);
        return co2level;
      }

      state2 = false;
      onTime = 0;
      offTime = 0;
      switchTime = 0;
    }
  }
}

int readISO()
{
  sensorValue = analogRead(sensorPin);
  while (true)
  {
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

    if (switchTime == 1 && state2)
    {

      ratioCo2 = (onTime * 100 / (offTime + onTime));

      if (ratioCo2 <= 50)
      {
        // isobutylene
        isolevel = (ratioCo2 - 5) * 25;
        Serial.print(" ISOBUTYLENE ");
        Serial.println(isolevel);
        return isolevel;
      }

      state2 = false;
      onTime = 0;
      offTime = 0;
      switchTime = 0;
    }
  }
}

void setup()
{
  Serial.begin(250000);
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println("test");
}

void loop()
{
  levels = readCO2();
  levels = readCO2();
}
