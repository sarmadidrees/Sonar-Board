/************ CODE for SONAR CONTROL with NANO Board ************/

/* NOTE: Digital pins 10,11,12,13 are extra pins which are not used */
/****************************************************************/

#include <NewPing.h>
#include "LocalizeAGV.h"

/**** For 4 UltraSonic Sensors ****/
#define SONAR_NUM       4
#define MAX_DISTANCE    160
#define PING_INTERVAL   33

unsigned long   pingTimer[SONAR_NUM];     // Holds the times when the next ping should happen for each sensor.
unsigned int    cm[SONAR_NUM];            // Where the ping distances are stored.
uint8_t         currentSensor = 0;        // Keeps track of which sensor is active.

NewPing sonar[SONAR_NUM] = {      // Each sensor's trigger pin, echo pin, and max distance to ping.
  NewPing(2, 3, MAX_DISTANCE),    // Refers to Front Sonar
  NewPing(4, 5, MAX_DISTANCE),    // Refers to Right Sonar
  NewPing(6, 7, MAX_DISTANCE),    // Refers to Back Sonar
  NewPing(8, 9, MAX_DISTANCE)     // Refers to Left Sonar
  };

  
/**** For 1 Color Sensor ****/
/* NOTE: 1st Connector has S0, S1 and LED
         2nd Connecetor has S2,S3 and OUT
  Color Sensor is attached to Analog Pins as follows */
  
#define S0      A0 
#define S1      A1  
#define S2      A3
#define S3      A4
#define LED     A2
#define OUT     A5

/* To hold the RGB values returned from Color Sensor */
int red     = 0;
int green   = 0;
int blue    = 0;

bool colorReadDone = false;
bool sonarReadDone = false;

/********** for DEBUGGING **********/ 
#define DEBUG         true
//#define PRINT_SONAR   true

String data     = "";               // To hold the sending data
char dataBuffer[20] = "";
#define SHORT_DISTANCE        8     // in cm
#define ONE_BOX_DISTANCE      55    // in cm
#define TWO_BOX_DISTANCE      95    // in cm
#define LONG_DISTANCE         150   // in cm
#define ERROR_DISTANCE        15    // in cm

String inputString = "";            // To hold incomming string through serial
boolean stringComplete = false;

LocalizeAGV robot = LocalizeAGV();  // For localization of robot
String localizeData = "SONAR,";

void setup() {
  
  #if DEBUG 
  Serial.begin(9600);
  #endif
 
  initColorSonar();

  inputString.reserve(200);       //Reserve 200 bytes for inputString from Serial
  data.reserve(20);
  localizeData.reserve(10);
}

void loop() {
/*
  if (!colorReadDone){
  readColorSensor();                    //This loop reads light intensity 
  char currentColor = findColor();      //To find current color  
  data += currentColor;
  data += ","; 
  }
  
  readAllSonar();
 
  if (sonarReadDone){
    if (stringComplete) {
        if (inputString[0] == 'S'){
          Serial.print(data);
        }
      inputString = "";
      stringComplete = false;
    }
    sonarReadDone = false;
    colorReadDone = false;
    data = "";
  }
  */

  if (stringComplete) {
      if (inputString.startsWith("L")){
        robot.setOrientation(inputString[2]);      //setting robot orientation
        
        if (!colorReadDone){
          readColorSensor();                    //This loop reads light intensity 
          char currentColor = findColor();      //To find current color  
          data += currentColor;
          data += ","; 
        }
  
        readAllSonar();

         if (sonarReadDone){
          data.toCharArray(dataBuffer,15);
          robot.pleaseLocalize(dataBuffer);
          localizeData = "SONAR,";
          localizeData += "XY";
          localizeData += ",";
          localizeData += (char)robot.getX();
          localizeData += ",";
          localizeData += (char)robot.getY();
          localizeData += "\n";
          
          Serial.print(localizeData);

          localizeData = "";
          inputString = "";
          stringComplete = false;
     
          sonarReadDone = false;
          colorReadDone = false;
          data = "";
        }
    }
    else if (inputString.startsWith("D")){
      if (!colorReadDone){
          readColorSensor();                    //This loop reads light intensity 
          char currentColor = findColor();      //To find current color  

          data = inputString[2];        //add orientation
          data += ",";
          data += currentColor;
          data += ","; 
        }

        readAllSonar();

        if (sonarReadDone){
          localizeData = "SONAR,DATA,";
          localizeData += data;
          
          Serial.print(localizeData);

          localizeData = "";
          inputString = "";
          stringComplete = false;
     
          sonarReadDone = false;
          colorReadDone = false;
          data = "";
        }
        
    }
  }
    
  
  #if PRINT_SONAR
  printSonar();
  #endif
}

/********* User defined Functions ************/

void initColorSonar(){

  //Initialize Color Sensor
  pinMode(S0, OUTPUT);  
  pinMode(S1, OUTPUT);  
  pinMode(S2, OUTPUT);  
  pinMode(S3, OUTPUT);  
  pinMode(OUT, INPUT);  
  //pinMode(LED, OUTPUT);

  /*** Output Frequency of Color Sensor can be adjust as follows
    **********************************************************
        S0      |     S1      |    Output Frequency Scaling
       LOW      |    LOW      |          Power down
       LOW      |    HIGH     |              2%
       HIGH     |    LOW      |              20%
       HIGH     |    HIGH     |              100%
    **********************************************************/
    
  digitalWrite(S0, HIGH);         // Set output frequency as 100% 
  digitalWrite(S1, HIGH); 

  // initialize UltraSonic Sensors
  pingTimer[0] = millis() + 100;                       // First ping starts at 100ms, gives time for the Arduino to chill before starting.
  for (uint8_t i = 1; i < SONAR_NUM; i++)             // Set the starting time for each sensor.
  pingTimer[i] = pingTimer[i - 1] + PING_INTERVAL;
  
}

void readColorSensor()
{   
  /* Selecting Photodiode to get the respective RGB value 
    **********************************************************
        S2      |     S3      |     Photodiode Type
       LOW      |    LOW      |          RED
       LOW      |    HIGH     |          BLUE
       HIGH     |    LOW      |     Clear (no filter)
       HIGH     |    HIGH     |          Green
    **********************************************************/
    
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  
  red = pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH);
  digitalWrite(S3,HIGH);
  
  blue = pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH);
  digitalWrite(S2, HIGH);

  green = pulseIn(OUT, digitalRead(OUT) == HIGH ? LOW : HIGH);
 
}


char findColor(){

  char color = ' ';

  /*
  Serial.print("Red Intensity:");         // Every color's intensity (individually) 
  Serial.print(red, DEC);  
  Serial.print(" Green Intensity: ");  
  Serial.print(green, DEC);  
  Serial.print(" Blue Intensity : ");  
  Serial.print(blue, DEC);  
  */
   if (green > 12 && green < 15)
    {//Serial.print("GREEN");             //Writes actual color at which robot is placed
     //Serial.print('\t');
     color = 'G';
    }
    
  else if (blue <= 7)
    {//Serial.print("WHITE");
     // Serial.print('\t');
     color = 'W';
    }
    
  else if (red < green && red < blue && green < blue)
    {//Serial.print("YELLOW");
     // Serial.print('\t');
     color = 'Y';
    }
    
  else if ( blue < 12 && blue >9)
    {//Serial.print("BLUE");
     // Serial.print('\t');
     color = 'B';
    }
    
  else if (red < green && red < blue && green > blue)
    {//Serial.print("RED");
     // Serial.print('\t');
     color = 'R';
    }
   else color = 'U'; 

  colorReadDone = true;
  // delay(500);            //Add this delay if needed (if sonar has no problem with this)
  return color;
  }


/********** Functions for ULTRA-SONIC SENSOR *************/
void readAllSonar(){
    
  for (uint8_t i = 0; i < SONAR_NUM; i++) {                              // Loop through all the sensors.
    if (millis() >= pingTimer[i]) {                                      // Is it this sensor's time to ping?
      pingTimer[i] += PING_INTERVAL * SONAR_NUM;                         // Set next time this sensor will be pinged.
      if (i == 0 && currentSensor == SONAR_NUM - 1) oneSensorCycle();    // Sensor ping cycle complete, do something with the results.
      sonar[currentSensor].timer_stop();                                 // Make sure previous timer is canceled before starting a new ping (insurance).
      currentSensor = i;                                                 // Sensor being accessed.
      cm[currentSensor] = 200;                                           // Make distance zero in case there's no ping echo for this sensor.
      sonar[currentSensor].ping_timer(echoCheck);                        // Do the ping (processing continues, interrupt will call echoCheck to look for echo).
    }
  }
}

// If ping received, set the sensor distance to array.
void echoCheck() {                                                        
  if (sonar[currentSensor].check_timer())
    cm[currentSensor] = sonar[currentSensor].ping_result / US_ROUNDTRIP_CM;
}

// Sensor ping cycle complete, add all sensor values to the sending data.
void oneSensorCycle() {                                                   

    for (uint8_t i = 0; i < SONAR_NUM; i++) {
      if ( (cm[i] > (SHORT_DISTANCE - 5)) && (cm[i] < (SHORT_DISTANCE + ERROR_DISTANCE)) )
         data += "A,";
      else if ( (cm[i] > (ONE_BOX_DISTANCE - ERROR_DISTANCE)) && (cm[i] < (ONE_BOX_DISTANCE + ERROR_DISTANCE)) )
         data += "B,";
      else if ( (cm[i] > (TWO_BOX_DISTANCE - ERROR_DISTANCE)) && (cm[i] < (TWO_BOX_DISTANCE + ERROR_DISTANCE)) )
         data += "C,";
      else if ( cm[i] > LONG_DISTANCE )
         data += "D,";
      else data += "P,";
  }
   data += "X\n";       //Just to show that all sonar values are stored in data(string)
   sonarReadDone = true;
}

// Just for DEBUGGING
void printSonar(){
  
  for (uint8_t i = 0; i < SONAR_NUM; i++) {
    Serial.print(i);
    Serial.print("=");
    Serial.print(cm[i]);
    Serial.print("cm ");
  }
  Serial.println();
  
}

// Serial Event to check serial data without intruption 
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

