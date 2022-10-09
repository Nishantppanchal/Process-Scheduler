// Prevent multiple declarations when included in multiple times
#ifndef STR_FUNCTIONS_H_INCLUDED
#define STR_FUNCTIONS_H_INCLUDED

/**
 * @brief Copy a string from a specifed index, until the specified delimiter is reached
 *
 * @param data The string we want to copy from
 * @param i The index we want to start copying from
 * @param delimiter The character we want stop copying at
 * @param dest The the destination to which we want to copy the string to
 */
void copy_str(char *str, int *i, char delimiter, char *dest);

/**
 * @brief Converts a string from a index to the delimiter specified to a integer
 *
 * @param str The string that you want get the integer
 * @param i The index from which the integer
 * @param delimiter The delimiter to stop reading the integer at
 * @return int The resulting integer
 */
int str_to_int(char *str, int *i, char delimiter);

/**
 * @brief Returns the length for a string from a char pointer
 * 
 * @param str A char pointer to the string
 * @return int The length of the string
 */
int string_len(char *str);

/**
 * @brief Concatenates strings together
 * 
 * @param ouput The string to output to
 * @param strings An array of pointers to the strings
 * @param num_strings The number of strings in the strings array
 * @param seperator The seperator between the string when concatenated
 */
void concat(char *ouput, char **strings, int num_strings, char seperator);

/**
 * @brief Appends str2 to the end of str1
 * 
 * @param str1 A pointer to the string that will have str2 appended to its end
 * @param str1_len The length of str1
 * @param str2 The string that will be appended to the end of str1
 * @param str2_len The length of str2
 * @return int The new length of str1
 */
int append_to_str(char **str1, int str1_len, char *str2, int str2_len);

#endif