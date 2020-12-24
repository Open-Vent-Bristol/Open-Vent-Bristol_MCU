#define LPF_MACRO(output, input, accumulator, filterk)  { accumulator += input; output = (int32_t)(accumulator/filterk); accumulator -=output; }    // Macro for analogue measurement averaging
#define LPF_MACRO2(output2, input2, accumulator2, filterk2)  { accumulator2 += input2; output2 = (int32_t)(accumulator2/filterk2); accumulator2 -=output2; }    // Macro for analogue measurement averaging
#define LPF_MACRO3(output3, input3, accumulator3, filterk3)  { accumulator3 += input3; output3 = (int32_t)(accumulator3/filterk3); accumulator3 -=output3; }    // Macro for analogue measurement averaging


//static long accumulator = (analogRead(A0)*2.96)*256;     // delay needed to smooth initial motor current spike
long accumulator = md.getM1CurrentMilliamps() * 64;    // delay needed to smooth initial motor current spike
long accumulator2 = 0;   // flow sensor
long accumulator3 = 0;   // pressure sensor

// With RS motor VCV Filterk at 64, pressure 38, volume 400ml. Current max values were:
// Inhale: 1100mA (ave)    Exhale: ? mA
// With RS motor VCV Filterk at 64, pressure 57, volume 800ml. Current max values were:
// Inhale: 2000mA (ave)    Exhale: ? mA
int getCurrentM1() {
#define FILTERK 64
  long filtered = 0;
  int input = 0;

  // 5 V / 1024 ADC counts = 4.88 mV per count
  // 3.3 V = 2.0 A; 3.3 V / 4.88 mv per count = 676 counts
  // 2.0 A / 676 counts = 2.96 mA per count
  //input = analogRead(A0)*2.96;
  input = md.getM1CurrentMilliamps();

  LPF_MACRO(filtered, input, accumulator, FILTERK);

  return (filtered);
}

// orifice flow sensor reading
int getFilteredFlow(int input2) {
#define FILTERK2 4
  long filtered2 = 0;

  LPF_MACRO2(filtered2, input2, accumulator2, FILTERK2);

  return (filtered2);
}

// pressure reading. Not yet used to control motor
int getFilteredPressure(int input3) {
#define FILTERK3 4
  long filtered3 = 0;

  LPF_MACRO2(filtered3, input3, accumulator3, FILTERK3);

  return (filtered3);
}
