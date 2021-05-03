// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#ifndef TEST_MACROS_H
#define TEST_MACROS_H

#define TEST_ASSERT_VALUE_IN_ARRAY(expected, array) \
{ \
  bool found = false; \
  for (int i = 0u; i < (sizeof(array) / sizeof(array[0])); i++) \
  { \
    if (array[i] == expected) found = true; \
  } \
  TEST_ASSERT(found); \
}

#endif /* TEST_MACROS_H */
