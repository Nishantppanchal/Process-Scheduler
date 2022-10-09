/* Import required header file */
#include "../h/str_functions.h"
#include "../h/int_functions.h"
/* Imports required libraries */
#include <stdlib.h>

/* copy_str function */
void copy_str(char *str, int *i, char delimiter, char *dest)
{
    /* Declare and initialise variables */
    int j = 0; // Used to iterate through dest

    /* Copy the string */
    // Do, while the current character is not the delimiter or the end of the string
    do
    {
        // Copy the current character to dest
        dest[j] = str[*i];
        // Increment i and j
        ++(*i), j++;
    } while (str[*i] != delimiter && str[*i] != '\0');

    /* Add null terminator to dest */
    dest[j] = '\0';

    /* Increment i */
    ++(*i);
}

/* str_to_int function */
int str_to_int(char *str, int *i, char delimiter)
{
    /* Declare variables */
    char current_char; // Used to store the current character
    int digit;         // Used to store the current digit
    /* Declare and initialise variables */
    // j is used to iterate through the string, num is used to store the resulting integer
    int j = 0, num = 0;

    // Do, while the current character is not the delimiter or the end of the string
    do
    {
        /* Add the digit to the integer */
        // Get the current character
        current_char = str[*i];
        // Gets the digit from the current character
        digit = atoi(&current_char);
        // Add the digit to the number
        num = num * 10 + digit;

        /* Increment i and j */
        ++(*i), j++;
    } while (str[*i] != delimiter && str[*i] != '\0');

    /* Increment i */
    ++(*i);

    /* Return the number */
    return num;
}

/* string_len function */
int string_len(char *str)
{
    /* Declare variables */
    int length = 0; // Used to iterate through the string

    /* Return the length of the string */
    // Increment i until the end of the string
    while (*str != '\0')
    {
        // Increment to next character in the string
        str++;
        // Increment length
        length++;
    }
    // Return the length
    return length;
}

/* concat function */
void concat(char *ouput, char **strings, int num_strings, char seperator)
{
    /* Declare and initialise variables */
    int output_index = 0; // Used to iterate through the output string

    /* For each string in the strings array */
    for (int i = 0; i < num_strings; i++)
    {
        /* Add seperator if not the first string */
        // If not the first string
        if (i > 0)
        {
            // Add the seperator to the output
            ouput[output_index] = seperator;
            // Increment the output index
            output_index++;
        }

        /* Add the string to the output */
        // For each character in the string
        for (int j = 0; strings[i][j] != '\000'; j++)
        {
            // Add the character to the output
            ouput[output_index] = strings[i][j];
            // Increment the output index
            output_index++;
        }
    }
}

/* append_to_str function */
int append_to_str(char **str1, int str1_len, char *str2, int str2_len)
{
    /* Declare and initialise variables */
    int i = 0;                              // Used to track position in strings when appending
    int new_str1_len = str1_len + str2_len; // The new length of append string, with -1 to account for termination character in str2

    /* Increase the size of str1 */
    *str1 = (char *)realloc(*str1, new_str1_len + 1); // Reallocates str1 memory to the new length and a termination character

    /* Append str2 to str1 */
    // For each character in str2
    for (i = 0; i < str2_len; i++)
    {
        // Add the character to str1
        (*str1)[str1_len + i] = str2[i];
    }

    /* Add termination character */
    (*str1)[str1_len + i] = '\0';

    /* Return the new length of str1 */
    return new_str1_len;
}