


/*  Hydroponi system v.6

   Made by
 *    * Simon Jepsen
 *    * Søren Lang
 *    * Mikkel Stenkjær Hansen
 *    * Kasper Bruhn
 *    * Rasmus Johansen
 *    * Kiet Nguyen
 *    * Sylvester Rathke Udengaard

*/
// included libraries

#include <waterHeight.h>
#include <RTClib.h>
#include <DHT.h>
#include <DHT_U.h>
// Defined pins and types
  //Pins
  #define pLED         2
  #define pWater       3
  #define pPh          4
  #define pFan         5
  #define pAir         6
  #define DHTPIN       7  
  #define distanceTrig 8
  #define distanceEcho 9
  #define pHSensor     A0

  //Type
  #define DHTTYPE DHT11

  //intialise sensor class/struct
  DHT dht(DHTPIN, DHTTYPE);         // Initialise sensor "dht" with pin and type
  RTC_DS1307 rtc;                   // Initialise real time clock chip DS1307
  waterHeight waterSensor(distanceEcho, distanceTrig, 172); // initializes the HC-SR04 sensor to prepare for measuring the water height
  DateTime dt(2000, 1, 1, 0, 0, 0); // Struct to hold the time recived form the DS1307
  // takes a starting time in the form of (yyyy-mm-dd-hh-mm-ss)




void setup()
{
  //initializes pins and objects
    // serial communication
    Serial.begin(9600);
    dht.begin();
    rtc.begin();

    // initialise pins
    pinMode(pLED, OUTPUT);
    pinMode(pFan, OUTPUT);
    pinMode(pAir, OUTPUT);
    pinMode(pPh, OUTPUT);
    pinMode(pWater, OUTPUT);
    pinMode(distanceTrig, OUTPUT);
    pinMode(distanceEcho, INPUT);
    digitalWrite(pAir, HIGH);

    waterSensor.measureDelay = 60;
    waterSensor.measurementsAmount = 100;
    waterSensor.trigDelay=20;

    //sets the RTC time to time defined in dt
    rtc.adjust(dt);
    //tests all sensors
    //sensorTest();
  
}
long lastMeasure;
void loop()
{
  //rtc functions to avoid delays
    //measures amounts of seconds gone since beginning
    long timePassed = rtc.now().minute();
    if(timePassed == 0)
    {
      lastMeasure = -1;
    }
    //Resets number of seconds passed to zero when a new day begins
    //resetRTC(timePassed);
    //Prints the current time
    Serial.print("Tid i minutter: ");
    Serial.println(rtc.now().minute());
    Serial.print("Totaltid i sekunder: ");
    Serial.println(timePassed);
    Serial.print("Tid i sekunder: ");
    Serial.println(rtc.now().second());
    
    //if 10 minutes have passed check the water level and adjust appropiately
      //for testing purposes set to 10 seconds
    int waterLightMeasureTime = 10;
    // control the water from the water ressouir
    //the times at which the light starts and stop
    int stopHour = 1;
    int startHour = 3;
    if (timePassed!= lastMeasure)
    {
      controlWaterValve(10, 10000, 10000, pAir);
      setLight(timePassed, stopHour, startHour);  
      //Controls flow from the pH solution ressoiur
      setPHPumpeState(evalpH(measPH()), 1230);    
      lastMeasure ++;
    }

  

  //controls the fan, when humidty is at a certain level
  fanControl(measHumid(), 5000, 60, 80, pFan);
}


// 1. Retrieve sensor values from sensors

//  float Time       = measTime();
//  float WaterLevel = measWaterLevel();
//  float Temp       = measTemp();
//  float Humid      = measHumid();
//  float PH         = measPH();
//  bool pumpState   = 1;

// 2. Determine relevant action

// sætter lys ud fra evaulered tid som er målt
//  setLight(evalTime(Time));

//  setFan(evalHumid(Humid));
//  setPumpe(evalPH(PH));
//  setLuftPumpe(pumpState);
//setSolenoid(evalWaterLevel(WaterLevel));

// 3. delay repitition


// Global variable declaration

// Funktion declaration

/* /*  Description: Measures and calculates the distance given from HR-SR04
     Parameters :
     Returns    : the distance to reflective surface, given in cm
   float measWaterLevel();

  /*  Description: Measures the ambient temperature
     Parameters :
     Returns    : The temperature given in the celsius with 1 decimal
   float measTemp();

  /*  Description: Measures relative humidity
     Parameters :
     Returns    : The relative humidity in [%] with 2 decimals precisions
   float measHumid();

  /*  Description: Measures the PH-value in the nutrient solution
     Parameters :
     Returns    : The PH-value
   float measPH();

  /*  Description: Measures the given time at one point
     Parameters :
     Returns    : The time measured in a 24- hour format
   float measTime();

  /* Description: Evaluates if the temperature is withing tolerance
    Parameters : temp = temperature in celcius
    Returns    : 0 if within tolerances, 1 if not
   bool evalTemp(float temp);

  /* Description: Evaluates if the humidity is withing tolerance
    Parameters : Humid = float between 00,00 and 90,00
    Returns    : 0 if within tolerances, 1 if not
   bool evalHumid(float humid);

  /* Description: Evaluates if the ph is withing tolerance
    Parameters :
    Returns    : 0 if within tolerances, 1 if not
   bool evalPH(float PH);

  /* Description: Evaluates what the time is, and if action is deemed necessary
    Parameters :
    Returns    : 0 if no action needed, else 1
  bool evalTime(float Time);

  /* Description: Evaluates if the water level is withing tolerance
    Parameters : wLevel given in cm
    Returns    : 0 if within tolerances, 1 if not
   bool evalWaterLevel(float WaterLevel);

  /* Description : Evaluates if the water level is withing tolerance
     Parameters :
     Returns    :
    void setLight(bool state);

  /* Description : Evaluates if the water level is withing tolerance
     Parameters :
     Returns    :
    void setFan(bool state);

  /* Description : Evaluates if the water level is withing tolerance
     Parameters :
     Returns    : 0 if within tolerances, 1 if not
    void setPumpe(bool state);

  /* Description :
     Parameters :
     Returns    :
   void setLuftPumpe(bool state);

  /* Description :
     Parameters :
     Returns    :
    void setSolenoid(bool state);*/
