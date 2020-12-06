#ifndef UTILITY_H
#define UTILITY_H

/**
 * Convert text into a string
 */
#define STR(s) #s

/**
 * Expand a symbol's value into a string - useful for debugging preprocessor using pragma message
 */
#define XSTR(s) STR(s)

/**
 * Macro to concatenate two strings
 */
#define CONCAT(s1, s2) STR(s1)##STR(s2)

#endif /* UTILITY_H */
