#! /usr/bin/python3
import sys
import re

# Constants used to calculate table
C1 = 19.06998055
E1 = 0.517553019

# Constants calculated for use as definitions
# NOTE: The value after SHIFT changes the multiplier, e.g. (1 << 16), (1 << 22)
constants = {
  "C2_SHIFT16": 0.00151537,
  "C3_SHIFT22": -4.22946e-6,
  "B1_SHIFT16": 0.96956652,
  "B2_SHIFT22": 0.999945636,
}

def print_usage():
  print("\nUSAGE: python3 generate_flow_lookup_table.py [pressure limit]")
  print("\nTable will have (pressure limit + 1) rows and take up two bytes per row")
  exit()

def generate_flow_lookup_table(pressure_limit:int):
  output_table = []
  pressure = 0
  while pressure <= pressure_limit:
    output_flow = pressure ** 2
    output_flow = output_flow ** (E1 / 2)
    output_flow *= C1
    output_flow = round(output_flow)

    output_table.append(output_flow)
    pressure += 1

  return output_table

if __name__ == "__main__":
  if len(sys.argv) < 2:
    print_usage()

  # Print the definitions needed for final calculation
  for key in constants:
    # Find the bit shift amount in the key name
    bit_shift_amount = re.search(r"SHIFT([0-9]{1,2})", key)
    if bit_shift_amount:
      bit_shift_amount = int(str(bit_shift_amount.group(1)))
    else:
      bit_shift_amount = 0

    value = constants[key]
    value *= (1 << bit_shift_amount)
    value = round(abs(value))

    if value > 65535:
      type = "ul"
    else:
      type = "u"

    print(("#define {0} ({1}{2})").format(key, value, type))

  pressure_limit = int(sys.argv[1])
  print(("#define PRESSURE_LIMIT ({})").format(pressure_limit))
  table = generate_flow_lookup_table(pressure_limit)

  # Print the table (array)
  print("\nstatic const PROGMEM uint16_t flow_lookup_table[] =\n{")
  for pressure in range(0, pressure_limit):
    print(("  {}u,").format(table[pressure]))

  # Last line with no comma
  print(("  {}u").format(table[pressure_limit]))
  print("};")
