
#ifndef LocalizeAGV_h
#define LocalizeAGV_h

#if defined (ARDUINO) && ARDUINO >= 100
   #include <Arduino.h>
#else
   #include <WProgram.h>
   #include <pins_arduino.h>
#endif

#if defined (__AVR__)
   #include <avr/io.h>
#endif


#define colorIndex   0
#define frontIndex   2
#define rightIndex   4
#define backIndex    6
#define leftIndex    8


class LocalizeAGV
{
public:
   LocalizeAGV();          //Constructor
   void setOrientation(char facing);                  //To set Orientation of ROBOT
   void pleaseLocalize(char inputString[31]);         //To Localize ROBOT wrt Sonar and Color Sensor Reading
   unsigned int getX();          //To get the X coordinate of localized ROBOT
   unsigned int getY();          //To get the Y coordinate of localized ROBOT

private:
   char _facing;                 //To hold orientation of ROBOT
   char _colorArray[11][5];      //To hold color Sensor Reading
   char _frontArray[11][5];      //To hold front sonar Sensor Reading
   char _rightArray[11][5];      //To hold right sonar Sensor Reading
   char _backArray[11][5];       //To hold back sonar Sensor Reading
   char _leftArray[11][5];       //To hold left sonar Sensor Reading

   char* _inputString;           //To hold input String from Sonar Board

   uint8_t _row;
   uint8_t _col;
   uint8_t _rowLocalize;
   uint8_t _colLocalize;
};


#endif
