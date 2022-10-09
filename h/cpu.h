// Prevent multiple declarations when included in multiple times
#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

/* CPU structure */
typedef struct
{
    /* Data */
    int process_running; // Stores the PID of process currently running in the process table or -1 if CPU is idle
    int current_time;    // Keeps track the time since CPU initialised
} CPU;

/**
 * @brief Creates a single core CPU
 *
 * @return CPU* A pointer to the CPU struct
 */
CPU *create_CPU();

#endif