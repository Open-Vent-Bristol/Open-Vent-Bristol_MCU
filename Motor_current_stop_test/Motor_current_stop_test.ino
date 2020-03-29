#include "ArduinoMotorShieldR3.h"

ArduinoMotorShieldR3 md;

// motor pins
int DIR_A = 12;
int PWM_A = 3;

unsigned long time2 = millis();
int mSpeed = 0;

int revTimeSetting = 700;
/* Speed:     Rev time:
 * 100        
 * 200        700 ms
 * 300
 */

void setup()
{
  Serial.begin(115200);
  Serial.println("Arduino Motor Shield R3");
  md.init();
  //pinMode(9, OUTPUT);           // brake pin
  pinMode(DIR_A, OUTPUT);
  pinMode(PWM_A, OUTPUT);
}

void loop()
{
  int current = getCurrentM1();     // log filtered current reading

  //digitalWrite(9, LOW);           // release brake DONT USE, IT DOESN'T UNBREAK

  for (int i = 0; i <= 9; i++)      // repeat x number of times
  {
    time2 = millis();               // log time in ms
    mSpeed = 0;                     // clear motor speed variable
    setMotor1Speed(mSpeed);         // drive motor
    while (current < 1400)          // drive until current setting
    {
      if (mSpeed > -400)            // acceloration
      {
        if (millis() - time2 > 3)     // accelorate the motor by 1 every 3ms
        {
          mSpeed--;                 // - is forward direction
          time2 = millis();
          Serial.print("forwards , ");
        }
      }
      setMotor1Speed(mSpeed);
      Serial.print(mSpeed);
      Serial.print(" , ");
      Serial.println(current);      // print time? or speed?
      current = getCurrentM1();
    }

    // reverse
    unsigned long revTime = millis();
    //while (current > 1000 && millis() - revTime < revTimeSetting)     // reversing based on current was not repeatable
    while (millis() - revTime < revTimeSetting)           // reverse back for a set time
    {
      setMotor1Speed(200);
      current = getCurrentM1();
      Serial.print("reverse , ");
      Serial.println(current);
    }
    // stopped
    unsigned long stopTime = millis();
    while (millis() - stopTime < 200)                     // dwell
    {
      setMotor1Speed(0);
      //digitalWrite(9, HIGH);          // apply brake DON'T USE
      current = getCurrentM1();
      Serial.print("stopped ");
      Serial.println(current);
      //delay(1000);
    }
  }
  for (;;)    // for testing purposes
  {
  }
}

void setMotor1Speed(int M1speed)
{
  if (M1speed < 0) {
    M1speed = -M1speed;  // Make speed a positive quantity
    digitalWrite(DIR_A, LOW);
  }
  else {
    digitalWrite(DIR_A, HIGH);
  }
  if (M1speed > 400) M1speed = 400;  // Max PWM dutycycle
  analogWrite(PWM_A, M1speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
}
