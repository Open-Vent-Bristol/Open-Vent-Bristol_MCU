#include "ArduinoMotorShieldR3.h"
ArduinoMotorShieldR3 md;

// motor pins
int DIR_A = 12;
int PWM_A = 3;
int current = 0;     // log filtered current reading

// UI inputs
int a1 = analogRead(A1);      // right POT variable
int maxPressurePOT = analogRead(A2);      // left POT variable
int maxPressurePOTconstrianed = 0;
int freqSettingCounter = 10;              // breaths per min
const int freqUpButtonPin = 2;
const int freqDownButtonPin = 6;
const int redLEDPin = A4;
const int blueLEDPin = A5;
unsigned long blueLEDonTime = 0;          // timer to turn on blue LED
unsigned long redLEDonTime = 0;          // timer to turn on blue LED
unsigned long POTblueLEDonTime = 0;       // timer for blue LED flashing with POT
int redLEDState = HIGH;         // the current state of the output pin
int blueLEDState = HIGH;         // the current state of the output pin
int buttonState2 = 0;             // the current reading from the input pin
int lastButtonState2 = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime2 = 0;  // the last time the output pin was toggled
int buttonState6 = 0;             // the current reading from the input pin
int lastButtonState6 = LOW;   // the previous reading from the input pin
long lastDebounceTime6 = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// LEDs
int redLED = A4;
int blueLED = A5;

unsigned long pauseTimer = millis();      // stop watch for pauses
unsigned long accelTime = millis();
int mSpeed = 0;

int revTimeSetting = 650;       // time it takes to reverse in ms
int revSpeed = 400;             // max speed of reverse stroke
int maxFwdSpeed = -400;         // max speed of motor/acceloration limit
int currentLimit = 1000;        // current limit
int postInhaleDwell = 0;
unsigned long postExhaleDwell = 0;
unsigned long lastBreathTime = 0;
unsigned long breathPeriod = 0;
/* with no reverse acceloration
    revSpeed:    Rev time:  currentLimit:   maxFwdSpeed:
   133           800 ms     1000 (min)      -400
   133           1100 ms    1400 (max)      -400
   200           500 ms     1000 (min)      -400
   200           700 ms     1400 (max)      -400
   400           ? ms     1000 (min)      -400
   400           ? ms     1400 (max)      -400

   with reverse acceloration every
    revSpeed:    Rev time:  currentLimit:   maxFwdSpeed:
   133           ? ms     1000 (min)      -400
   133           ? ms    1400 (max)      -400
   200           750 ms     1000 (min)      -400
   200           900 ms     1400 (max)      -400

   we don't change rev speed anymore
   400           650 ms     800 (min)       -400
   400           700 ms     1000            -400
   400           ? ms       1200            -400
   400           800 ms     1400 (max)      -400
*/

void setup()
{
  md.init();
  Serial.begin(115200);
  Serial.println("Arduino Motor Shield R3");
  //pinMode(9, OUTPUT);           // brake pin

  // set up motor pins
  pinMode(DIR_A, OUTPUT);
  pinMode(PWM_A, OUTPUT);

  // set up UI pins
  pinMode(freqUpButtonPin, INPUT);
  pinMode(freqDownButtonPin, INPUT);
  pinMode(redLEDPin, OUTPUT);
  pinMode(blueLEDPin, OUTPUT);
  digitalWrite(freqUpButtonPin, HIGH);        // needed for the buttons to work strangely enough
  digitalWrite(freqDownButtonPin, HIGH);

  for (int i = 0; i <= 4; i++)      // flash LEDs a
  {
    digitalWrite(redLEDPin, redLEDState);   // set initial LED state
    digitalWrite(blueLEDPin, blueLEDState);   // set initial LED state
    delay(200);
    redLEDState = !redLEDState;
    blueLEDState = !blueLEDState;
  }
  digitalWrite(redLEDPin, redLEDState);     // turn back off
  digitalWrite(blueLEDPin, blueLEDState);   // turn back off
}


void loop()
{
  checkUI();
  //digitalWrite(9, LOW);           // release brake DONT USE, IT DOESN'T UNBREAK

  //for (int i = 0; i <= 9; i++)      // repeat x number of times
  //{
  lastBreathTime = millis();
  inhale();
  postInhalePause();
  exhale();
  postExhalePause();
  //}
  //for (;;)    // for testing purposes
  //{
  //}
}



// *********** Inhale ***********
void inhale ()
{
  accelTime = millis();               // log time in ms
  mSpeed = 0;                     // clear motor speed variable
  setMotor1Speed(mSpeed);         // drive motor
  while (current < currentLimit)          // drive until current setting
  {
    if (mSpeed > maxFwdSpeed)            // acceloration
    {
      if (millis() - accelTime > 3)     // accelorate the motor by 1 every 3ms
      {
        mSpeed--;                 // - is forward direction
        accelTime = millis();
      }
    }
    setMotor1Speed(mSpeed);
    Serial.print("forwards , ");      // don't remove these or the current readings will update so quick
    Serial.println(current);          // thing to do: read current at regular time intervals
    checkUI();
  }
}


// *********** post inhale pause ***********
void postInhalePause ()
{
  pauseTimer = millis();
  while (millis() - pauseTimer < postInhaleDwell)                     // dwell
  {
    setMotor1Speed(0);
    //digitalWrite(9, HIGH);          // apply brake DON'T USE
    checkUI();
    Serial.print("paused ");
    Serial.println(current);
  }
}


// *********** exhale ***********
void exhale ()
{
  unsigned long revTimer = millis();   // log time in ms
  //while (current > 1000 && millis() - revTime < revTimeSetting)     // reversing based on current was not repeatable

  accelTime = millis();               // log time in ms for acceloration
  int revAccelSpeed = 0;                     // clear motor speed variable
  while (millis() - revTimer < revTimeSetting)           // reverse back for a set time
  {
    if (revAccelSpeed < revSpeed)            // acceloration
    {
      if (millis() - accelTime > 1)     // accelorate the motor by 2 every 3ms
      {
        revAccelSpeed++;                 // - is forward direction
        accelTime = millis();
      }
    }
    setMotor1Speed(revAccelSpeed);
    checkUI();
    Serial.print("reverse , ");
    Serial.println(current);
  }
}


// *********** post exhale pause ***********
void postExhalePause ()
{
  while (millis() - lastBreathTime < breathPeriod)                     // dwell
  {
    setMotor1Speed(0);
    //digitalWrite(9, HIGH);          // apply brake DON'T USE
    checkUI();
    Serial.print("paused ");
    Serial.println(millis() - lastBreathTime);
  }
}



// ****** drive motors ******
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

// ***** check buttons & POTs and flash LEDs *****
void checkUI()          // check buttons and POTs
{
  current = getCurrentM1();
  a1 = analogRead(A1);
  if (analogRead(A2) > maxPressurePOT + 1 || analogRead(A2) < maxPressurePOT - 1) // if values are not equal within a tollerance
  {
    maxPressurePOT = analogRead(A2);                              // read POT
    if (maxPressurePOT > 950 || maxPressurePOT < 50) redLEDonTime = millis();
    else POTblueLEDonTime = millis();                                // flash LEDs for feedback
    maxPressurePOTconstrianed = constrain(maxPressurePOT, 50, 950);          // constrain POT values to account for tollerance
  }

  currentLimit = map(maxPressurePOTconstrianed, 50, 950, 800, 1200);       // set current limit
  currentLimit = constrain(currentLimit, 800, 1200);            // constrain current limits
  revTimeSetting = map(currentLimit, 800, 1200, 650, 850);      // set reversing time period
  revTimeSetting = constrain(revTimeSetting, 650, 850);         // constrain reverse time limits

  // frequency up button reading
  int reading2 = digitalRead(freqUpButtonPin);
  // debounce
  if (reading2 != lastButtonState2) lastDebounceTime2 = millis();    // reset the debouncing timer if state change
  if ((millis() - lastDebounceTime2) > debounceDelay) {              // if button pressed for longer than delay
    if (reading2 != buttonState2)
    {
      buttonState2 = reading2;
      if (buttonState2 == LOW)
      {
        freqSettingCounter++;         // add 1 to breath per min setting
        if (freqSettingCounter <= 30) blueLEDonTime = millis();
        else redLEDonTime = millis();
      }
    }
  }
  lastButtonState2 = reading2;

  // frequency down button reading
  int reading6 = digitalRead(freqDownButtonPin);
  // debounce
  if (reading6 != lastButtonState6) lastDebounceTime6 = millis();    // reset the debouncing timer if state change
  if ((millis() - lastDebounceTime6) > debounceDelay) {              // if button pressed for longer than delay
    if (reading6 != buttonState6)
    {
      buttonState6 = reading6;
      if (buttonState6 == LOW)
      {
        freqSettingCounter--;         // take away 1 from breath per min setting
        if (freqSettingCounter >= 10) blueLEDonTime = millis();
        else redLEDonTime = millis();
      }
    }
  }
  lastButtonState6 = reading6;

  freqSettingCounter = constrain(freqSettingCounter, 10, 30);       // constrain variable to 10-30 b/m
  breathPeriod = (60 / freqSettingCounter) * 1000;                  // calc time needed per breath in ms based on UI setting
  Serial.println(freqSettingCounter);

  if (millis() - blueLEDonTime < 200) digitalWrite(blueLEDPin, HIGH);   // flash blue LED for button press feedback
  else digitalWrite(blueLEDPin, LOW);
  if (millis() - redLEDonTime < 500) digitalWrite(redLEDPin, HIGH);     // flash red LED if b/m limit reached
  else digitalWrite(redLEDPin, LOW);
  if (millis() - POTblueLEDonTime < 10) digitalWrite(blueLEDPin, HIGH);   // flash blue LED for button press feedback
  else digitalWrite(blueLEDPin, LOW);
}

