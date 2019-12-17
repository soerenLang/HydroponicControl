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

    //sets up additional water sensor variables
    waterSensor.measureDelay = 60;
    waterSensor.measurementsAmount = 100;
    waterSensor.trigDelay=20;

    //sets the RTC time to time defined in dt( 1/1 2000, 00:00:00)
    rtc.adjust(dt);
    //tests all sensors
    sensorTest();
  
}
long lastMeasure = 23;
void loop()
{
  //rtc functions to avoid delays
    //measures amounts of seconds gone since beginning
    long timePassed = rtc.now().hour();
    if(timePassed == 0 && lastMeasure == 23)
    {
      lastMeasure = -1;
    }
    //Prints the current time
    Serial.print("Tid: ");
    Serial.print(rtc.now().hour());
    Serial.print(":");
    Serial.println(rtc.now().minute());
    Serial.print(":");
    Serial.println(rtc.now().second());

    //the times at which the light starts and stop
    int stopHour = 8;
    int startHour = 17;
    //if an hour has passed check up on: water level, light, pH
    if (timePassed!= lastMeasure)
    {
      //controls the waterlevel, sets it 65 mm
      controlWaterValve(65, 10000, 10000, pAir);
      //turns on the light at those two hours
      setLight(timePassed, stopHour, startHour);  
      //Controls flow from the pH solution ressoiur
      setPHPumpeState(evalpH(measPH()), 1230);    
      lastMeasure ++;
    }

  //controls the fan, when humidty is at a certain level
  fanControl(measHumid(), 5000, 60, 80, pFan);
}

