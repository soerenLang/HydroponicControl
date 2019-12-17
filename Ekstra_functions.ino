

//measures the humidity and returns the humidity
float measHumid()
{
  //initializing
  delay(2000);
  //reads the humidity to be returned
  float h = dht.readHumidity();
  //prints the humidity
  Serial.print("humidity: ");
  Serial.println(h);
  
  return h;
}


//controls the fan to adjust the humidity of the system
void fanControl(float humid, int fanTime, int minHum, int maxHum, int pinFan)
{
  //activates the fan for a full system-cycle if the relative humidity is above the maximum value
  if (humid >= maxHum)
  {
    Serial.println("Humidity larger then maximum. Fanmode: max speed");
    digitalWrite(pinFan, HIGH);
  }

  //turns off the fan if the relative humidity is below the minimum value
  else if (humid <= minHum)
  {
    Serial.println("Humidity is lower then minimum. Fanmode: off");
    digitalWrite(pinFan, LOW);
  }
  //if the relative humidity is between minimum and maximum, run the fan for the set amount of time
  else
  {
    Serial.println("Humidity is normal. Fanmode: regulating");
    digitalWrite(pinFan, HIGH);
    delay(fanTime);
    digitalWrite(pinFan, LOW);
  }
}


//measures the pH-value of the water, prints the value to the console and returns the pH-value
float measPH()
{
  float pHData = analogRead(pHSensor);
  float volt = (pHData / 1023) * 5;
  float pH = (-5.8816 * volt) + 21.802;
  return pH;
}

//measures the pH-value, and returns this value
float evalpH(float pHpre)
{
  float toReturn;
  //if the pH-value measured to begin with is equal to 7 or above, pH is measured 10 times and averages the measurements to obtain a more accurate result.
  if (pHpre >= 7.0)
  {
    float sum = 0;
    //sums up ten measurements
    for (int i = 0; i < 10; i++)
    {
      float pH = measPH();
      sum += pH;
    }
    //averages the sum 
    float averagepH = sum / 10;
    Serial.println(averagepH);
    //returns the sum
    toReturn = averagepH;
  }

  //if the pH-value is not above or equal to 7, skip all of the above and return 0
  else
  {
    toReturn = 0;
  }
  return toReturn;
}

//adds acid for a certain amount of time if the pH-value is above 7
void setPHPumpeState(float averagepH, int pumpTime)
{
  if (averagepH >= 7.0)
  {
    digitalWrite(pPh, HIGH);
    delay(pumpTime);
    digitalWrite(pPh, LOW);
  }
  else Serial.println("pH acceptable");
}

//turns on light in the interval between start time and stop time
void setLight(long timeRTC, long startTime, long stopTime)
{ 
  //if start lies earlier in the day than stop
  if(startTime<stopTime)
  {
    if(timeRTC > startTime && timeRTC<stopTime)
    {
      digitalWrite(pLED, HIGH);
    }
    else digitalWrite(pLED, LOW);
  }
  //if start lies later in the day than stop
  else if (startTime>stopTime)
  {
    if(startTime< timeRTC && stopTime < timeRTC)
    {
      digitalWrite(pLED, HIGH);
    }
    else if (startTime>timeRTC && stopTime>timeRTC)
    {
      digitalWrite(pLED, HIGH);
    }
    else digitalWrite(pLED, LOW);
  }
  else digitalWrite(pLED, LOW);
}



// control the water valve, and raises the water level if the water level is too low
void controlWaterValve(int waterLevel, int valveOpenTime, int waterSettelingTime, int pinAir)
{
  Serial.println("sampling of waterheight");
  //measures the water height
  float toMeasure = waterSensor.waterHeightMeasurement();
  //prints the water height
  Serial.print("højde ");
  Serial.println(toMeasure);

  //turns off the airpump to prewent waves in the water
  if(toMeasure < waterLevel)
  {
    digitalWrite(pinAir, LOW);
    delay(120000);
  }
  //raises the water level while the water level is too low
  while (toMeasure < waterLevel) {
    Serial.print("rasing waterlevel ");
    Serial.println(toMeasure);
    digitalWrite(pWater, HIGH);
    delay(valveOpenTime);
    digitalWrite(pWater, LOW);
    Serial.println("stopped raising the water");
    delay(waterSettelingTime);
    toMeasure = waterSensor.waterHeightMeasurement();
  }
  digitalWrite(pinAir, HIGH);
}

// makes a short test wheater the sensors are connected correctly
void sensorTest()
{
  // test DHT11
  Serial.println("Testing DHT11 temp");
  float test = dht.readTemperature();
  if (isnan(test))
  {
    Serial.println("condition: not connected");
  }
  else
  {
    Serial.println("condition: connected");
  }

  Serial.println("Testing DHT11 humidity");
  test = dht.readHumidity();
  if (isnan(test))
  {
    Serial.println("Condition: not connected");
  }
  else
  {
    Serial.println("condition: connected");
  }
 
  Serial.println("Testing waterhight sensor");
  test = waterSensor.waterHeightMeasurement();

  if (test >= 100)
  {
    Serial.println("Condition: not connected");
  }
  else
  {
    Serial.println("condition: connected");
  }
}
