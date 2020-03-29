#define LPF_MACRO(output, input, accumulator, filterk)  { accumulator += input; output = (int32_t)(accumulator/filterk); accumulator -=output; }    // Macro for analogue measurement averaging

//static long accumulator = (analogRead(A0)*2.96)*256;     // delay needed to smooth initial motor current spike
static long accumulator = md.getM1CurrentMilliamps()*256;     // delay needed to smooth initial motor current spike


/************************ reads an analogue input **************************/
int getCurrentM1(){
#define FILTERK 256
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
