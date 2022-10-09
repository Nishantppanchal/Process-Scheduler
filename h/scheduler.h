/* Import required header file */
#include "linked_list.h"

// Prevent multiple declarations when included in multiple times
#ifndef SCHEDULER_H_INCLUDED
#define SCHEDULER_H_INCLUDED

/* Scheduler structure */
struct Scheduler
{
    /* Data */
    void *process_storage;    // A pointer to the process storage data structure used by the scheduler
    int time_quantum;         // The time quantum
    int time_quantum_counter; // A counter to keep track of when a process needs to be preempted

    /* Functions */
    int (*schedule)(struct Scheduler *self, Linked_list *process_table, int current_time);                              // Function that return the process that should be run
    void (*update_add)(struct Scheduler *self, int *new_processes_array, Linked_list *process_table, int current_time); // Updates the process storage with any new processes that have entered the system
};
typedef struct Scheduler Scheduler; // Defines the Scheduler type as the Scheduler structure

/**
 * @brief Determine what process should be run
 * @attention The implementation changes based on the task
 * 
 * @param self The scheduler
 * @param process_table A pointer to the linked list that represents the process table
 * @param current_time The current time, which is the time the CPU has run for
 * @return int The index for the process that should be run in the process table or -1 if the CPU should be idle
 */
int schedule(Scheduler *self, Linked_list *process_table, int current_time);

/**
 * @brief Updates the process storage with any new processes that have entered the system 
 * @attention The implementation changes based on the task
 * 
 * @param self The scheduler
 * @param new_processes_array A integer array that consists of the indexes of the new processes in the process table 
 * @param process_table A pointer to the linked list that represents the process table
 * @param current_time The current time, which is the time the CPU has run for
 */
void update_add(Scheduler *self, int *new_processes_array, Linked_list *process_table, int current_time);

#endif