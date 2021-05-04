// Copyright (c) 2021 <OpenVent-Bristol, Donald Robson>

#pragma once

#ifndef UTEST_MODE
#define TESTABLE static
#else
#define TESTABLE
#endif

/**
 * Convert text into a string
 */
#define STR(s) #s

/**
 * Expand a symbol's value into a string - useful for debugging preprocessor using pragma message
 */
#define XSTR(s) STR(s)

#define ARRAY_LENGTH(a) (sizeof(a) / sizeof(a[0]))
