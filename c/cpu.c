/* Import required header file */
#include "../h/cpu.h"
/* Imports required libraries */
#include <stdlib.h>

/* create_CPU function */
CPU *create_CPU()
{
    /* Create CPU */
    // Allocate memory for the CPU
    CPU *cpu = (CPU *)malloc(sizeof(CPU));
    // Set the current time to 0
    cpu->current_time = 0;
    // Set the PID of the process running to -1, meaning the CPU is idle
    cpu->process_running = -1;

    /* Return CPU */
    return cpu;
}