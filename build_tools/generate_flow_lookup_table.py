#! /usr/bin/python3
import sys
import re

# Constants calculated for use as definitions or to create table
# NOTE: The value after SHIFT changes the multiplier, e.g. (1 << 16), (1 << 22)
# "UNUSED" will prevent an integer version of the constant being emitted
constants = {
  "C1_UNUSED": 19.06998055,
  "E1_UNUSED": 0.517553019,
  "C2_SHIFT16": 0.00151537,
  "C3_SHIFT22": -4.22946e-6,
  "B1_SHIFT16": 0.96956652,
  "B2_SHIFT22": 0.999945636
}

def print_usage():
  print("\nUSAGE:")
  print("python3 generate_flow_lookup_table.py [ADC resolution] [ADC mVref] [mV per Pa] [mV at 0Pa]")
  print("\nTable will have (ADC resolution) rows and take up two bytes per row")
  exit()

def generate_flow_lookup_table(adc_resolution:int, adcref_mV:float, mV_perPa:float, mV_zeroPa:float):
  output_table = []
  index = 0
  while index < adc_resolution:
    mV = index * adcref_mV / adc_resolution
    pressure = (mV - mV_zeroPa) / mV_perPa
    output_flow = pressure ** 2
    output_flow = output_flow ** (constants["E1_UNUSED"] / 2)
    output_flow *= constants["C1_UNUSED"]
    output_flow = round(output_flow)

    if pressure < 0:
      output_flow = 0 - output_flow

    output_table.append(output_flow)
    index += 1

  return output_table

if __name__ == "__main__":
  if len(sys.argv) != 5:
    print_usage()

  adc_resolution = int(sys.argv[1])
  adcref_mV = int(sys.argv[2])
  mV_perPa = float(sys.argv[3])
  mV_zeroPa = int(sys.argv[4])

  # Print the definitions needed for final calculation
  constants_shifted = []
  constants_raw = []

  for key in constants:
    # Find the bit shift amount in the key name
    str_match = re.search(r"([A-Z][1-9])_?SHIFT([0-9]{1,2})", key)
    if str_match:
      label_raw = str(str_match.group(1))
      bit_shift_amount = int(str(str_match.group(2)))
    else:
      label_raw = key.replace("_UNUSED", "")
      bit_shift_amount = 0

    value = constants[key]
    value *= (1 << bit_shift_amount)
    value = round(value)

    if value > 65535:
      type = "ul"
    else:
      type = "u"

    if "UNUSED" not in key:
      constants_shifted.append(("#define {0} ({1}{2})").format(key, value, type))
    constants_raw.append(("#define {0} ({1})").format(label_raw, constants[key]))

  print("\n// Shifted integer constants for runtime calculation")
  for define in constants_shifted:
    print(define)

  print(("#define ADC_RESOLUTION ({})").format(adc_resolution))
  print(("#define ADC_REF_MV ({})").format(adcref_mV))
  print(("#define MV_ZERO_PA_SHIFT10 ({})").format(mV_zeroPa << 10))
  print(("#define PA_PER_MV_SHIFT10 ({})").format(round((1 << 10) / mV_perPa)))

  print("\n// Raw constants for unit tests")
  for define in constants_raw:
    print(define)

  print(("#define MV_PER_PA ({})").format(mV_perPa))
  print(("#define MV_AT_ZERO_PA ({})").format(mV_zeroPa))

  flow_table = generate_flow_lookup_table(adc_resolution, adcref_mV, mV_perPa, mV_zeroPa)

  # Print the table (as an array)
  print("\nstatic const PROGMEM int16_t flow_lookup_table[] =\n{")
  for value in flow_table:
    print(("  {},").format(value))

  # Last line with no comma
  print(("  {}").format(flow_table[adc_resolution - 1]))
  print("};")
