/* Import required header file */
#include "cpu.h"
#include "linked_list.h"
#include "handle_processes.h"
#include "scheduler.h"

// Prevent multiple declarations when included in multiple times
#ifndef CPU_CONTROLLER_H_INCLUDED
#define CPU_CONTROLLER_H_INCLUDED

/* CPU controller structure */
typedef struct
{
    /* The information for the final file ouput */
    char *result_str; // The string to write the the output file
    int results_len;  // The length of the string above

    CPU *cpu;                           // The CPU to be controlled
    Scheduler *scheduler;               // The scheduler algorithm
    Linked_list *process_table;         // The process table
    Process_iterator *process_iterator; // The process iterator
} CPU_controller;

/**
 * @brief Initialise CPU controller
 *
 * @param filename The file that contents process data
 * @param scheduler The scheduler to use
 * @return CPU_controller* A CPU_controller
 */
CPU_controller *initialise_CPU_controller(char *filename, Scheduler *scheduler);

/**
 * @brief Runs the CPU
 *
 * @param self CPU_controller for which we want to run the CPU
 */
void run_CPU(CPU_controller *self);

/**
 * @brief Shutdown the CPU
 *
 * @param self CPU_controller for which we want to shutdown the CPU
 * @param task_num The task number character, which will be use to make the results file
 */
void shutdown_CPU(CPU_controller *self, char task_num);

#endif