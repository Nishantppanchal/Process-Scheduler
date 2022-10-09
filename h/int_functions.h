// Prevent multiple declarations when included in multiple times
#ifndef INT_FUNCTIONS_H_INCLUDED
#define INT_FUNCTIONS_H_INCLUDED

/**
 * @brief Returns the number of digits in a integer
 * 
 * @param x The integer
 * @return int The number of digits in the integer
 */
int int_len(int x);

/**
 * @brief Returns a^b
 *
 * @param a The base
 * @param b The power
 * @return int The value of a^b
 */
int power(int a, int b);

/**
 * @brief Converts integer to string
 * 
 * @param buff The buffer where the integer will be placed
 * @param val The integer value that is to be converted to string
 * @param len The length of the integer
 */
void int_to_str(char *buff, int val, int len);

#endif