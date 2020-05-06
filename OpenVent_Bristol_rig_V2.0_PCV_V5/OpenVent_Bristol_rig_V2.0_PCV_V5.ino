/*
  OpenVent-Bristol version V2.0
  PCV (Pressure Control Ventilation)
  with aim of meeting design specification: https://www.gov.uk/government/publications/coronavirus-covid-19-ventilator-supply-specification/rapidly-manufactured-ventilator-system-specification

  Relevant requirements from MHRA UK spec document:
  - Default: "default settings of 90-100% oxygen, 400ml/s tidal volume and / or inspiratory plateau pressure 35 cmH2O, 15 cmH2O PEEP, rate 20 breaths min-1 ."
  - Display: "Must show the current settings of tidal volume, frequency, PEEP, FiO2, ventilation mode"
  - Display: "Must show the actual current airway pressure"
  Note: for user to work out FiO2 based on the exiting exygen flow rate gauge, the ventilator will need to display average flow rate
  - Alarm on:
            - Gas or electricity supply failure
            - Machine switched off while in mandatory ventilation mode
            - Inspiratory airway pressure exceeded
            - Inspiratory and PEEP pressure not achieved (equivalent to disconnection alarm)
            - Tidal volume not achieved or exceeded.

  Ristrictions of this test rig:
  1. Can't reliabily measure low flow rates less than 200ml/s. Which limits us to the following mim settings:
      a Min flow rate of 200ml/s
      b at 400ml tidal volume min 10 BPM with I:E ratio of 1:2
      c at 400ml tidal volume with I:E ratio of 1:1, minimum BPM is restricted to 15
*/

// stuff to do:
// add other alarm states?
// visual actual pressure display
// add press and hold left button motor start stop
// add battery back up
// stickers
// doesn't work at very low lung compliance at 45cmH2O

//#include <Wire.h>
//#include <sfm3000wedo.h>
#include <PID_v1.h>
#include "ArduinoMotorShieldR3.h"
ArduinoMotorShieldR3 md;

#include <LiquidCrystal.h>
// select the pins used on the Velleman LCD button Arduino shield
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define buttonsLCDpin A3        // Analogue input for buttons on LCD shield
#define btnRIGHT 0
#define btnUP 1
#define btnDOWN 2
#define btnLEFT 3
#define btnSELECT 4
#define btnNONE 5

// Status for UI
#define inhale 0
#define exhale 1
#define exhalePause 2
int breathStatus = exhalePause;

int exhalePeriod = 0;
int inhalePeriod = 0;

#define pressureSensePin A5
#define flowSensePin A4
#define buzzerPin 2

// for PID loop
#define PIN_INPUT 1
#define PIN_OUTPUT 3

//Define Variables for PID
double Setpoint, Input, Output;
//Specify the links and initial tuning parameters

double Kp = 500, Ki = 200, Kd = 14;   // 35cmH2O default
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);
unsigned long serialTime; //this will help us know when to talk with processing

// motor pins
int DIR_A = 12;
int PWM_A = 3;
int current = getCurrentM1();

unsigned long pauseTimer = millis();      // stop watch for pauses
unsigned long accelTime = millis();
int mSpeed = 0;

int revTimeSetting = 450;       // time it takes to reverse in ms
int revSpeed = 400;             // max speed of reverse stroke
int maxFwdSpeed = -400;         // max speed of motor/acceloration limit
//int currentLimit = 4000;      // current limit
int IEval = 2;                  // 1:IEval
int postInhaleDwell = 0;
//unsigned long postExhaleDwell = 0;
unsigned long breathTime = 0;
int target_BPM = 20;            // defult 20 BPM
int target_breathPeriod = (60 / target_BPM) * 1000;
int breathVolumemls = 0;                               // volume accumulator
int target_cmH2O = 35;         // defult 35cmH2O
int set_tidalMax = 800;
int set_tidalMin = 150;
#define minPressure 15                // set higher than the lowest PEEP for failsafe
#define maxPressure 45                // system can't deliver more than this

// alarm states
bool alarmMotorStop = 0;

byte up[8] = {              // create arrows
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b00100
};
byte down[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
byte fullBar[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};
byte oneBar[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00000,
  0b10000,
  0b10000,
  0b10000,
  0b10000
};
byte twoBar[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00000,
  0b11000,
  0b11000,
  0b11000,
  0b11000
};
byte threeBar[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00000,
  0b11100,
  0b11100,
  0b11100,
  0b11100
};
byte fourBar[8] = {
  0b00100,
  0b00100,
  0b00100,
  0b00000,
  0b11110,
  0b11110,
  0b11110,
  0b11110
};

void setup()
{
  md.init();
  //Wire.begin();
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);       // setup buzzer pin as output
  delay(500); // let serial console settle
  // initialize the SFM3300
  //measflow.init();
  //Serial.println("Sensor initialized!");

  // set up motor pins
  pinMode(DIR_A, OUTPUT);
  pinMode(PWM_A, OUTPUT);

  //initialize the variables PID is linked to
  Input = getFlowRate();
  Setpoint = target_cmH2O;       // flowrate target

  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 255);
  myPID.SetSampleTime(5);
  getFlowRate();        //

  //    while (getCurrent() < 600)
  //    {
  //      setMotor1Speed(150);             // reset motor position
  //      Serial.println(getCurrent());
  //    }
  //  setMotor1Speed(0);

  // LCD display setup
  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("OpenVent-Bristol");    // headings to LCD
  lcd.setCursor(0, 1);
  lcd.print("Alert system");
  digitalWrite(buzzerPin, HIGH);    // test buzzer
  delay(50);
  digitalWrite(buzzerPin, LOW);
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Scroll using    ");    // headings to LCD
  lcd.setCursor(0, 1);
  lcd.print("lft & rgt keys  ");
  delay(500);

  lcd.createChar(0, down);      // create new characters
  lcd.createChar(1, up);
  lcd.createChar(2, fullBar);
  lcd.createChar(3, oneBar);
  lcd.createChar(4, twoBar);
  lcd.createChar(5, threeBar);
  lcd.createChar(6, fourBar);

  offState();
  setDisplayHeadings();
  //for (;;)
  //{
  setMotor1Speed(0);
  //}
}


int offState ()
{
  // start up screen and off state screen
  while (read_LCD_buttons() != btnRIGHT)          // run starting screen
  {
    lcd.setCursor(0, 0);                         // display messages
    lcd.print("Press->  ");
    lcd.write(byte(1));
    lcd.print("M up  ");
    lcd.setCursor(0, 1);
    lcd.print("to start ");
    lcd.write(byte(0));
    lcd.print("M down");
    if (read_LCD_buttons() == btnUP) setMotor1Speed(80);    // move motor controls
    else if (read_LCD_buttons() == btnDOWN) setMotor1Speed(-80);
    else setMotor1Speed(0);
  }
}



// function senses if machine off button has been pressed
int offButtonSense(int leftButtonState)     
{
  static unsigned long leftTimer;       // counter for recording how long the left button has been pressed
  static int warningBuzz = 0;           // counter for sounding a warning buzzer
  static const int offButtonDebounceTime = 3000;

  if (leftButtonState == 0)
  {
    warningBuzz = 0;
    leftTimer = millis();
    return 150;       // set debounce time
  }
  // switch off ventilator if left button has been held down for longer than x seconds
  else if (leftButtonState == 1 && millis() - leftTimer > offButtonDebounceTime && breathStatus == exhalePause)
  { // only do once motor is up/in exhalePause
    if (warningBuzz < 4)
    {
      if (millis() - leftTimer > offButtonDebounceTime + 500)
      {
        soundBuzzer(1);           // soundBuzzer() holds buzzer on for 100ms
        leftTimer = millis() - offButtonDebounceTime;
        warningBuzz++;
      }
      else soundBuzzer(0);
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print(" Ventilation off              ");          // headings to LCD
      lcd.setCursor(0, 1);
      lcd.print("                              ");      // clear number display
      delay(1000);
      offState();       // turn ventilator off if button has been held long enough and buzzer has "blipped" a few times
      return 1500;      // new buttton debounce time
    }
  }
}



void loop()
{
  static unsigned long inhaleCounter = millis();
  static unsigned long exhaleCounter = millis();
  target_breathPeriod = (60 / target_BPM) * 1000;               // set time for inhale + exhale
  int target_inhalePeriod = target_breathPeriod / (IEval + 1);  // set inhale period
  Setpoint = target_cmH2O;                                      // pressure target
  //for (int i = 0; i < 5 ; i++)     // debug only
  //{
  unsigned long motorSetTimer = millis();
  while (getFilteredPressure(getPressure()) == getPressure())  // waits for motor arm to begin pushing BVM
  {
    setMotor1Speed(-400);
  }
  int motorSetTime = millis() - motorSetTimer;
  breathTime = millis();                           // used to start and end inhale and exhale
  exhalePeriod = millis() - exhaleCounter;         // store exhale time duration
  inhaleCounter = millis();                        // start inhale time counter
  // ensures inhale time period is met even if motor doesn't start in contact with BVM
  while (millis() - inhaleCounter < target_inhalePeriod)
  {
    doInhale();   // pass inhale set time to function
  }
  inhalePeriod = millis() - inhaleCounter;         // store inhale time duration
  exhaleCounter = millis();                        // start exhale time counter
  doExhale();                                      // blocking function
  while (millis() - (breathTime - motorSetTime) < target_breathPeriod)  // for remainder of exhale period
  { // subtract motorSetTime to take into account the time taken for motor to contact with the bag
    postExhalePause();                             // not blocking function
  }
  //}
  //for (;;)    // for debug purposes
  //{
  //setMotor1Speed(0);
  //}
}


// ********** Resets the display *******
int setDisplayHeadings ()
{
  // Breath frequency (FR), peak pressure (PP), tidal volume (VOL), inhale:exhale ratio (I:E), ventilation mode (Mode)
  //lcd.noCursor();
  lcd.setCursor(0, 0);
  lcd.print("FR AP VOL SP Vmax Vmin I:E  ");          // headings to LCD
  lcd.setCursor(0, 1);
  lcd.print("                              ");      // clear number display
  lcd.setCursor(23, 1);
  lcd.print("1:");                                // print the static part of the I:E ratio
}



// used for periodically sending values to Arduino serial plotter
int serialPlot (int val1, int val2, int val3, int val4, int val5)
{
  static unsigned long plotTimer = millis();
  const int plotInterval = 10;
  if (millis() - plotTimer > plotInterval)
  {
    Serial.print(val1);
    Serial.print("  ");
    Serial.print(val2);
    Serial.print("  ");
    Serial.print(val3);
    Serial.print("  ");
    Serial.print(val4);
    Serial.print("  ");
    Serial.print(val5);
    Serial.print("  ");
    Serial.println("uT");

    plotTimer = millis();
  }
}



// *********** Inhale ***********
int doInhale ()
{
  breathStatus = inhale;
  //int newFlowVal = getFlowRate();
  float newPressureVal = getPressure();
  static float storedPressureSetting = target_cmH2O;
  static int speedLimit = -400;
  static int Kval = 500;
  static int Dval = 16;

  if (target_cmH2O != storedPressureSetting)
  {
    //double Kp = 60, Ki = 200, Kd = 20;    // 15 cmH2O with speed limiting -225
    //double Kp = 200, Ki = 200, Kd = 16;   // 30cmH2O
    //double Kp = 500, Ki = 200, Kd = 14;   // 35cmH2O
    //double Kp = 500, Ki = 200, Kd = 10;   // 45cmH2O
    Kval = map(target_cmH2O, minPressure, maxPressure, 60, 750);
    Dval = map(target_cmH2O, minPressure, maxPressure, 20, 10);
    speedLimit = map(target_cmH2O, minPressure, maxPressure, -225, -400);
    storedPressureSetting = target_cmH2O;
  }

  Input = newPressureVal;
  myPID.Compute();

  float outputPID = round(Output);                       // round to as map() only takes intiger
  int maxFwdSpeed = map(outputPID, 0, 255, 0, speedLimit); // set motor speed proportionally to pressure difference
  maxFwdSpeed = constrain(maxFwdSpeed, speedLimit, 0);
  setMotor1Speed(maxFwdSpeed);

  //Serial.println(getCurrentM1());
  //Serial.println(md.getM1CurrentMilliamps());
  updateLCD();

  serialPlot(target_cmH2O, newPressureVal, getFilteredPressure(getPressure()), maxFwdSpeed / 10, target_cmH2O - (2 + (target_cmH2O * 0.04)));

  //    Serial.print("inhale");
  //    Serial.println(millis() - doInhaleTimer);
}




// *********** exhale ***********
int doExhale ()
{
  breathStatus = exhale;
  unsigned long revTimer = millis();     // log time in ms
  revTimeSetting = map(getVolumePerBreath(), 200, 800, 216, 315);

  accelTime = millis();               // log time in ms for acceloration
  int revAccelSpeed = 0;                     // clear motor speed variable
  //while (millis() - revTimer < revTimeSetting && getCurrent() < 600)  // reverse back until end stop sensed or for a set time
  while (millis() - revTimer < revTimeSetting)  // reverse back for a set time
  {
    if (revAccelSpeed < revSpeed)            // acceloration
    {
      if (millis() - accelTime > 20)     // accelorate the motor by 2 every 3ms
      {
        revAccelSpeed = revAccelSpeed + 40;                 // - is forward direction
        accelTime = millis();
      }
    }
    //revAccelSpeed = 400;
    setMotor1Speed(revAccelSpeed);
    updateLCD();

    serialPlot(target_cmH2O, getPressure(), getFilteredPressure(getPressure()), revAccelSpeed / 10, target_cmH2O - (2 + (target_cmH2O * 0.04)));

    //Serial.println(getCurrentM1());
    //Serial.print("exhale");
    //Serial.println(millis() - breathTime);
  }
}



// *********** post exhale pause ***********
int postExhalePause ()
{
  breathStatus = exhalePause;
  setMotor1Speed(0);

  updateLCD();

  serialPlot(target_cmH2O, getPressure(), getFilteredPressure(getPressure()), 0, target_cmH2O - (2 + (target_cmH2O * 0.04)));

  //  Serial.print("paused ");
  //  Serial.println(millis() - breathTime);
}



// *********** read current at regular intervals ***********
int getCurrent ()
{
  static unsigned long currentTimer = millis(); // log time in ms for adding to current accumulator
  if (millis() - currentTimer > 20)     // add to current accumulator regularly
  {
    current = getCurrentM1();
    currentTimer = millis();
  }
  return current;
}



// ****** drive motors ******
int setMotor1Speed(int M1speed)
{
  static const int currentLimit = 3000;
  if (M1speed < 0) {
    M1speed = -M1speed;  // Make speed a positive quantity
    digitalWrite(DIR_A, LOW);
  }
  else {
    digitalWrite(DIR_A, HIGH);
  }
  if (M1speed > 400) M1speed = 400;  // Max PWM dutycycle
  //M1speed = 0;        // debug only stops motor

  if (getCurrentM1() > currentLimit)        // sense if motor hits end stop
  {
    M1speed = 0;
    lcd.setCursor(0, 0);         // format (cell, line)
    lcd.print("    Over current      ");
    soundBuzzer(1);
  }
  else
  {
    soundBuzzer(0);
  }

  if (alarmMotorStop == 1 && breathStatus == inhale) M1speed = 0;      // sense alert from other fucntion

  analogWrite(PWM_A, M1speed * 51 / 80); // default to using analogWrite, mapping 400 to 255
}



int updateLCD ()
{
  const int BPM_cell = 0;             // LCD cell positions
  const int cmH2O_cell = 3;
  const int vol_cell = 6;
  const int setPressure_cell = 10;
  const int Vmax_cell = 13;
  const int Vmin_cell = 18;
  const int IE_cell = 25;

  static int cursorPos;               // position variable of select cursor
  static unsigned long buttonTimer;   // lof time for how long the button has been pressed for for debounce
  static int buttonState;
  static int lastButtonState;
  static int debounceTime = 150;       // time period for debouncing the buttons
  static int screenLatPos;
  static int screenLatPosTarget;

  int cmH2Oreading = getMaxPressure();      // actual value variables
  int volumeReading = getVolumePerBreath();
  int BPMreading = getBPM();
  float IEratioReading = getIEratio();

  // target settings variables
  //static int target_BPM = 20;           // defult 20 BPM now global
  //  static int target_cmH2O = 20;         // defult 35cmH2O
  //  static int set_tidalMax = 400;
  //  static int set_tidalMin = 150;
  static int target_IE = 2;             // defult I:E 1:2

  static bool leftButtonPressed = 0;    // variable to sense left button pressed for START/STOP of ventilator

  // read the buttons
  int lcd_key = read_LCD_buttons();
  if (lastButtonState != lcd_key) buttonTimer = millis();    // reset the debouncing timer if state change
  if (millis() - buttonTimer > debounceTime)                 // if button pressed for longer than delay
  {
    if (lcd_key != buttonState) buttonState = lcd_key;       // if there is a new button reading
    leftButtonPressed = 0;
    switch (buttonState)                                     //
    {
      case btnRIGHT:        // change cursor position
        {
          if (cursorPos == BPM_cell) cursorPos = setPressure_cell;
          else if (cursorPos == setPressure_cell) cursorPos = Vmax_cell;
          else if (cursorPos == Vmax_cell) cursorPos = Vmin_cell;
          else if (cursorPos == Vmin_cell) cursorPos = IE_cell;
          cursorPos = constrain(cursorPos, 0, IE_cell);
          break;
        }
      case btnLEFT:        // change cursor position
        {
          if (cursorPos == IE_cell) cursorPos = Vmin_cell;
          else if (cursorPos == Vmin_cell) cursorPos = Vmax_cell;
          else if (cursorPos == Vmax_cell) cursorPos = setPressure_cell;
          else if (cursorPos == setPressure_cell) cursorPos = BPM_cell;
          cursorPos = constrain(cursorPos, 0, IE_cell);
          leftButtonPressed = 1;
          break;
        }
      case btnUP:         // change target value
        {
          if (cursorPos == BPM_cell) target_BPM = target_BPM + 2;         // NHS spec asked for increments of 2
          else if (cursorPos == setPressure_cell) target_cmH2O++;         // 'SP' on LCD for set pressure
          else if (cursorPos == Vmax_cell) set_tidalMax = set_tidalMax + 10;
          else if (cursorPos == Vmin_cell) set_tidalMin = set_tidalMin + 10;
          // I:E optionally needed in future
          break;
        }
      case btnDOWN:        // change target value
        {
          if (cursorPos == BPM_cell) target_BPM = target_BPM - 2;     // NHS spec asked for increments of 2
          else if (cursorPos == setPressure_cell) target_cmH2O--;         // 'SP' on LCD for set pressure
          else if (cursorPos == Vmax_cell) set_tidalMax = set_tidalMax - 10;
          else if (cursorPos == Vmin_cell) set_tidalMin = set_tidalMin - 10;
          // I:E optionally needed in future
          break;
        }
    }
    buttonTimer = millis();       // to make sure it doesn't register multiple presses from one finger press

    // constrain targets
    target_BPM = constrain(target_BPM, 10, 30);     // as of requirements
    target_cmH2O = constrain(target_cmH2O, minPressure, maxPressure);
    set_tidalMax = constrain(set_tidalMax, 300, 800);
    set_tidalMin = constrain(set_tidalMin, 50, 300);
    target_IE = constrain(target_IE, 1, 3);         // as of requirements
  }
  lastButtonState = lcd_key;

  screenLatPosTarget = cursorPos / 2;
  if (screenLatPos != screenLatPosTarget)     // shift characters along if needed
  {
    screenLatPosTarget = constrain(screenLatPosTarget, 0, 12);
    if (screenLatPosTarget > screenLatPos)
    {
      lcd.scrollDisplayLeft();
      screenLatPos++;
    }
    if (screenLatPosTarget < screenLatPos)
    {
      lcd.scrollDisplayRight();
      screenLatPos--;
    }
  }

  debounceTime = offButtonSense(leftButtonPressed);      // function senses if machine off button has been pressed

  // display outputs
  static bool firstInhale = 1;                // identify the 1st run of the inhale
  static int LCDupdateTimer = millis();
  if (millis() - LCDupdateTimer > 100)         // update only every 50ms to speed up code and
  {
    if (breathStatus == inhale)
    {
      if (firstInhale == 1)                     // if 1st inhale run
      {
        setDisplayHeadings();
        firstInhale = 0;                        // reset identifier
      }
    }
    else
    {
      firstInhale = 1;                          // reset identifier
    }
    static unsigned long blinkTimer = millis();
    static const int blinkPeriod = 1000;
    // "FR PP VOL SP Vmax Vmin I:E  "
    lcd.setCursor(BPM_cell, 1);                 // format (cell, line)
    lcd.print(target_BPM);
    lcd.setCursor(cmH2O_cell, 1);         
    lcd.print(cmH2Oreading);
    lcd.setCursor(vol_cell, 1);      
    lcd.print(volumeReading);
    lcd.setCursor(setPressure_cell, 1);    
    lcd.print(target_cmH2O);
    lcd.setCursor(Vmax_cell, 1);  
    lcd.print(set_tidalMax);
    lcd.setCursor(Vmin_cell, 1);
    lcd.print(set_tidalMin);
    lcd.setCursor(IE_cell, 1);    
    lcd.print(IEratioReading);

    if (millis() - blinkTimer < 300)            // flash the number to change
    {
      lcd.setCursor(cursorPos, 1);
      lcd.print("   ");
    }
    else if (millis() - blinkTimer > 1000) blinkTimer = millis();

    LCDupdateTimer = millis();
  }
}



int soundBuzzer(bool buzzerSetting)          // sounds the buzzer during an alarm state
{
  static unsigned long volAlarmTimer;        // count time

  if (buzzerSetting == 1)
  {
    digitalWrite(buzzerPin, HIGH);           // sonund buzzer
    volAlarmTimer = millis();
  }
  else if (millis() - volAlarmTimer > 100)   // hold buzzer on for 100ms
  {
    digitalWrite(buzzerPin, LOW);
  }
}



int getVolumePerBreath ()
{
  int newFlowRate = getFilteredFlowRate();                            // in ml/s
  static byte integratePeriod = 10;                           // in miliseconds
  static unsigned long integratePeriodCounter = millis();     // to keep track of the current integration time
  static unsigned long breathVolumeOutput = 0;                         // final volume to 'return' from function
  static unsigned long breathVolume = 0;                               // volume accumulator

  // add to integrated flow rate accumulator every Xms
  if (breathStatus == inhale)        // if during inhale
  {
    // add to integrated flow rate accumulator with volume recorded over the integratePeriod
    if (millis() - integratePeriodCounter >= integratePeriod)
    {
      breathVolume = breathVolume + (newFlowRate * (millis() - integratePeriodCounter)); // convert to volume in ml/ms over integratePeriod
      //Serial.println(millis() - integratePeriodCounter);      // code run time
      breathVolumemls = breathVolume / 1000;                    // store volume rise (useful in VCV mode)
      integratePeriodCounter = millis();
    }
  }
  else                                    // if exhale
  {
    breathVolumemls = 0;                  // reset volume rise variable
    if (breathVolume > 0)                 // if volume hasn't been recorded already for last breath
    {
      breathVolumeOutput = round(breathVolume * 0.001);   // set output volume
      breathVolume = 0;                   // reset accumulator
    }
    integratePeriodCounter = millis();    // reset
  }

  if (breathVolumemls > set_tidalMax)
  {
    if (breathVolumemls > 800) alarmMotorStop = 1;
    lcd.setCursor(0, 0);         // format (cell, line)
    lcd.print("     Over volume      ");
    soundBuzzer(1);
  }
  else if (breathVolumeOutput < set_tidalMin)
  {
    lcd.setCursor(0, 0);         // format (cell, line)
    lcd.print("     Under volume      ");
    soundBuzzer(1);
  }
  else
  {
    alarmMotorStop = 0;
    soundBuzzer(0);
  }

  return breathVolumeOutput;
}



float getIEratio () // to calculate the 'E' of the Inhale : Exhale ratio
{
  static float IEratio = 0;
  if (inhalePeriod > 0 && exhalePeriod > 0) IEratio = (float)exhalePeriod / (float)inhalePeriod;       // DO THIS: display to 1 decimal place & remove the rounding
  static float returnIEratio = 0;
  if (IEratio > (float)0.5 && IEratio < 4) returnIEratio = IEratio;                 // remove odd readings
  returnIEratio = round(returnIEratio * 10);
  returnIEratio = returnIEratio / 10;                   // round to 1 decimel place
  return returnIEratio;
}



int getBPM ()     // breaths per minute DO THIS: it seems to display a 3 digit number, fix it
{
  //float BPM = round(60 / (float)((inhalePeriod + exhalePeriod) * 0.001));   // calc number for breath per minute
  float BPM = round(60 / (float)((inhalePeriod + exhalePeriod) * 0.001));   // calc number for breath per minute
  static float returnBPM = 0;
  if (BPM > 1 && BPM < 100) returnBPM = BPM;
  return returnBPM;
}



// ***** read MPX5010GP pressure sensor *****
int getPressure ()
{
  const float ADC_mV = 4.8828125;       // convesion multiplier from Arduino ADC value to voltage in mV
  const float SensorOffsetGP = 40;      // in mV taken from datasheet
  const float sensitivity = 4.413;      // in mV/mmH2O taken from datasheet
  const float mmh2O_cmH2O = 10;         // divide by this figure to convert mmH2O to cmH2O
  const float mmh2O_kpa = 0.00981;      // convesion multiplier from mmH2O to kPa
  const float mmh2O_pa = 9.80665;       // convesion multiplier from mmH2O to Pa

  float rawValueMP = analogRead(pressureSensePin);          // max pressure sensor reading
  // ((sensorValue in mV -sensorOffset)/sensitivity in mV/mmH2O)*kPa conversion constant
  float sensorValueMP = (((rawValueMP - SensorOffsetGP) * ADC_mV) / sensitivity / mmh2O_cmH2O);     // result in cmH2O
  return sensorValueMP;
}



// ********* used for UI display only ***********
int getMaxPressure ()
{ // round the values
  int newP = getPressure();           // read current pressure
  static int maxP = 0;                    // variable for max pressure
  static unsigned long maxPressureHoldTimer = millis();   // for timing duration that maxPressure is shown on display

  if (breathStatus == inhale)
  {
    if (newP > maxP) maxP = newP;     // record max pressure during inhale
    maxPressureHoldTimer = millis();
  }
  else
  {
    if (millis() - breathTime > target_breathPeriod - 800) maxP = 0;  // displays max reading for time period
  }
  return maxP;
}



// ***** check home made orifice flow sensor *****
int getFlowRate ()
{
  // ADC count, flow (ml/s) lookup table
  // NOTE: This table is not suitable for tidal volumes greater than 400ml at high BPMs or IE ratios
  static int flowArray[415] = {       // full array 645
    0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 0
    , 97    // new values
    , 141   //
    , 174   //
    , 189
    , 200
    , 211
    , 221
    , 231
    , 240
    , 249
    , 257
    , 266
    , 274
    , 282
    , 289
    , 297
    , 304
    , 311
    , 317
    , 324
    , 331
    , 337
    , 343
    , 349
    , 355
    , 361
    , 367
    , 373
    , 379
    , 384
    , 389
    , 395
    , 400
    , 405
    , 410
    , 416
    , 420
    , 425
    , 430
    , 435
    , 440
    , 444
    , 449
    , 453
    , 458
    , 463
    , 467
    , 472
    , 476
    , 480
    , 484
    , 489
    , 493
    , 497
    , 501
    , 505
    , 509
    , 513
    , 517
    , 521
    , 525
    , 529
    , 533
    , 536
    , 540
    , 544
    , 548
    , 552
    , 555
    , 559
    , 562
    , 566
    , 570
    , 573
    , 577
    , 580
    , 584
    , 587
    , 591
    , 594
    , 597
    , 601
    , 604
    , 607
    , 611
    , 614
    , 617
    , 621
    , 624
    , 627
    , 630
    , 633
    , 637
    , 640
    , 643
    , 646
    , 649
    , 652
    , 655
    , 658
    , 661
    , 664
    , 667
    , 670
    , 673
    , 676
    , 679
    , 682
    , 685
    , 688
    , 691
    , 694
    , 697
    , 699
    , 702
    , 705
    , 708
    , 711
    , 713
    , 716
    , 719
    , 722
    , 725
    , 727
    , 730
    , 733
    , 735
    , 738
    , 741
    , 744
    , 746
    , 749
    , 751
    , 754
    , 757
    , 759
    , 762
    , 764
    , 767
    , 769
    , 772
    , 775
    , 777
    , 780
    , 782
    , 785
    , 787
    , 790
    , 792
    , 795
    , 797
    , 799
    , 802
    , 804
    , 807
    , 809
    , 812
    , 814
    , 816
    , 819
    , 821
    , 824
    , 826
    , 828
    , 831
    , 833
    , 835
    , 838
    , 840
    , 842
    , 845
    , 847
    , 849
    , 852
    , 854
    , 856
    , 858
    , 861
    , 863
    , 865
    , 867
    , 870
    , 872
    , 874
    , 876
    , 878
    , 881
    , 883
    , 885
    , 887
    , 889
    , 891
    , 894
    , 896
    , 898
    , 900
    , 902
    , 904
    , 906
    , 909
    , 911
    , 913
    , 915
    , 917
    , 919
    , 921
    , 923
    , 925
    , 927
    , 930
    , 931
    , 934
    , 936
    , 938
    , 940
    , 942
    , 944
    , 946
    , 948
    , 950
    , 952
    , 954
    , 956
    , 958
    , 960
    , 962
    , 964
    , 966
    , 968
    , 970
    , 972
    , 974
    , 976
    , 978
    , 980
    , 981
    , 983
    , 985
    , 987
    , 989
    , 991
    , 993
    , 995
    , 997
    , 999
    , 1001
    , 1002
    , 1004
    , 1006
    , 1008
    , 1010
    , 1012
    , 1014
    , 1016
    , 1017
    , 1019
    , 1021
    , 1023
    , 1025
    , 1027
    , 1028
    , 1030
    , 1032
    , 1034
    , 1036
    , 1038
    , 1039
    , 1041
    , 1043
    , 1045
    , 1046
    , 1048
    , 1050
    , 1052
    , 1054
    , 1056
    , 1057
    , 1059
    , 1061
    , 1063
    , 1064
    , 1066
    , 1068
    , 1070
    , 1071
    , 1073
    , 1075
    , 1077
    , 1078
    , 1080
    , 1082
    , 1083
    , 1085
    , 1087
    , 1089
    , 1090
    , 1092
    , 1094
    , 1095
    , 1097
    , 1099
    , 1100
    , 1102
    , 1104
    , 1105
    , 1107
    , 1109
    , 1111
    , 1112
    , 1114
    , 1115
    , 1117
    , 1119
    , 1120
    , 1122
    , 1124
    , 1125
    , 1127
    , 1129
    , 1130
    , 1132
    , 1134
    , 1135
    , 1137
    , 1138
    , 1140
    , 1142
    , 1143
    , 1145
    , 1147
    , 1148
    , 1150
    , 1151
    , 1153
    , 1155
    , 1156
    , 1158
    , 1159
    , 1161
    , 1162
    , 1164
    , 1166
    , 1167
    , 1169
    , 1170
    , 1172
    , 1173
    , 1175
    , 1177
    , 1178
    , 1180
    , 1181
    , 1183
    , 1184
    , 1186
    , 1187
    , 1189
    , 1190
    , 1192
    , 1193
    , 1195
    , 1197
    , 1198
    , 1200
    , 1201
    , 1203
    , 1204
    , 1206
    , 1207
    , 1209
    , 1210
    , 1212
    , 1213
    , 1215
    , 1216
    , 1218
    , 1219
    , 1220
    //    , 1222
    //    , 1223
    //    , 1225
    //    , 1226
    //    , 1228
    //    , 1229
    //    , 1231
    //    , 1232
    //    , 1234
    //    , 1235
    //    , 1237
    //    , 1238
    //    , 1240
    //    , 1241
    //    , 1242
    //    , 1244
    //    , 1245
    //    , 1247
    //    , 1248
    //    , 1250
    //    , 1251
    //    , 1253
    //    , 1254
    //    , 1256
    //    , 1257
    //    , 1258
    //    , 1260
    //    , 1261
    //    , 1263
    //    , 1264
    //    , 1265
    //    , 1267
    //    , 1268
    //    , 1270
    //    , 1271
    //    , 1272
    //    , 1274
    //    , 1275
    //    , 1277
    //    , 1278
    //    , 1279
    //    , 1281
    //    , 1282
    //    , 1284
    //    , 1285
    //    , 1287
    //    , 1288
    //    , 1289
    //    , 1291
    //    , 1292
    //    , 1293
    //    , 1295
    //    , 1296
    //    , 1297
    //    , 1299
    //    , 1300
    //    , 1302
    //    , 1303
    //    , 1304
    //    , 1306
    //    , 1307
    //    , 1308
    //    , 1310
    //    , 1311
    //    , 1313
    //    , 1314
    //    , 1315
    //    , 1316
    //    , 1318
    //    , 1319
    //    , 1321
    //    , 1322
    //    , 1323
    //    , 1325
    //    , 1326
    //    , 1327
    //    , 1329
    //    , 1330
    //    , 1331
    //    , 1333
    //    , 1334
    //    , 1335
    //    , 1336
    //    , 1338
    //    , 1339
    //    , 1340
    //    , 1342
    //    , 1343
    //    , 1344
    //    , 1346
    //    , 1347
    //    , 1348
    //    , 1350
    //    , 1351
    //    , 1352
    //    , 1353
    //    , 1355
    //    , 1356
    //    , 1357
    //    , 1359
    //    , 1360
    //    , 1361
    //    , 1363
    //    , 1364
    //    , 1365
    //    , 1366
    //    , 1368
    //    , 1369
    //    , 1370
    //    , 1371
    //    , 1373
    //    , 1374
    //    , 1375
    //    , 1377
    //    , 1378
    //    , 1379
    //    , 1380
    //    , 1382
    //    , 1383
    //    , 1384
    //    , 1385
    //    , 1387
    //    , 1388
    //    , 1389
    //    , 1390
    //    , 1392
    //    , 1393
    //    , 1394
    //    , 1395
    //    , 1397
    //    , 1398
    //    , 1399
    //    , 1400
    //    , 1402
    //    , 1403
    //    , 1404
    //    , 1405
    //    , 1407
    //    , 1408
    //    , 1409
    //    , 1410
    //    , 1411
    //    , 1413
    //    , 1414
    //    , 1415
    //    , 1416
    //    , 1418
    //    , 1419
    //    , 1420
    //    , 1421
    //    , 1422
    //    , 1424
    //    , 1425
    //    , 1426
    //    , 1427
    //    , 1428
    //    , 1430
    //    , 1431
    //    , 1432
    //    , 1433
    //    , 1434
    //    , 1436
    //    , 1437
    //    , 1438
    //    , 1439
    //    , 1441
    //    , 1442
    //    , 1443
    //    , 1444
    //    , 1445
    //    , 1446
    //    , 1448
    //    , 1449
    //    , 1450
    //    , 1451
    //    , 1452
    //    , 1454
    //    , 1455
    //    , 1456
    //    , 1457
    //    , 1458
    //    , 1459
    //    , 1461
    //    , 1462
    //    , 1463
    //    , 1464
    //    , 1465
    //    , 1466
    //    , 1468
    //    , 1469
    //    , 1470
    //    , 1471
    //    , 1472
    //    , 1473
    //    , 1474
    //    , 1476
    //    , 1477
    //    , 1478
    //    , 1479
    //    , 1480
    //    , 1481
    //    , 1483
    //    , 1484
    //    , 1485
    //    , 1486
    //    , 1487
    //    , 1488
    //    , 1489
    //    , 1491
    //    , 1492
    //    , 1493
    //    , 1494
    //    , 1495
    //    , 1496
    //    , 1497
    //    , 1498
    //    , 1500
    //    , 1501
    //    , 1502
    //    , 1503
    //    , 1504
    //    , 1505
    //    , 1506
    //    , 1507
  };
  // read flow rate from lookup table based on calculated values in excel table
  int flowRate = flowArray[analogRead(flowSensePin)];       // ml/s

  return flowRate;    // return ml/s
}



// ********* LPF on flow to remove freak readings ********
int getFilteredFlowRate ()
{
  int returnFlow = getFilteredFlow(getFlowRate());       // low pass filter on flow readings. Not used
  if (returnFlow < 80) returnFlow = 0;
  return returnFlow;    // return ml/s
}



// read the buttons
int read_LCD_buttons()      // debounce these
{
  int adc_key_in = analogRead(buttonsLCDpin);             // read the value from the sensor
  // buttons are centered at these values: 0, 102, 258, 412, 641 (example code vals: 0, 144, 329, 504, 741)
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be  the most likely result
  // For V1.1 us this threshold
  if (adc_key_in < 50) return btnRIGHT;
  if (adc_key_in < 175) return btnUP;
  if (adc_key_in < 350) return btnDOWN;
  if (adc_key_in < 525) return btnLEFT;
  if (adc_key_in < 900) return btnSELECT;
  return btnNONE; // when all others fail, return this

  // IMPORTANT: Calibration routine needed to account for resistors out of tollerance
}
