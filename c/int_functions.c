/* Import required header file */
#include "../h/int_functions.h"
/* Imports required libraries */
#include <stdio.h>

/* int_len function */
int int_len(int x)
{
    /* Case 1: x is 0 */
    // If x is 0, return 1
    if (!x)
    {
        return 1;
    }

    /* Case 2: x is not 0 */
    // Initialise the length to 0
    int len = 0;
    // While x is not 0, increment the length and divide x by 10
    for (len = 0; x != 0; len++)
    {
        x /= 10;
    }
    // Return the length
    return len;
}

/* power function */
int power(int a, int b)
{
    /* Case 1: b is 0 */
    // If b is 0, return 1
    if (!b)
    {
        return 1;
    }

    /* Case 2: b is not 0 */
    // Initialise the result to a
    int result = a;
    /* Multiply the result by a b - 1 times
     * The reason we multiple b - 1 times is because result is already set to a when initialised
     */
    for (int i = 1; i < b; i++)
    {
        result *= a;
    }
    // Return the result
    return result;
}

/* int_to_str function */
void int_to_str(char *buff, int val, int len)
{
    // If val is 0, set the first character of buff to 0
    if (!val)
    {
        buff[0] = '0';
    }
    else
    {
        // Convert val to a string and store it in buff
        sprintf(buff, "%d", val);
    }

    // Add the null terminator to buff
    buff[len - 1] = '\0';
}