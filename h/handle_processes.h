/* Import required header file */
#include "linked_list.h"
#include "pcb.h"

// Prevent multiple declarations when included in multiple times
#ifndef HANDLE_PROCESSES_H_INCLUDED
#define HANDLE_PROCESSES_H_INCLUDED

/* Process iterator structure */
struct Process_iterator
{
    /* Data */
    int yield_pos; // Keep track of number of previous iterations
    /* Data - Yield state */
    char *data;                 // The data read from the file
    int read_all_processes;     // 1 if all process have been entered in the the system, otherwise 0
    int length;                 // The length of the data
    Linked_list *process_table; // The process_table
    int *current_time;          // A pointer to the current time since the CPU was initialise

    /* Functions */
    int *(*next_processes)(struct Process_iterator *self); // A pointer to the function that read the next set of processes
};
typedef struct Process_iterator Process_iterator;

typedef struct
{
    char *str;
    int len;
} Process_str;

/**
 * @brief Reads process data from a file
 *
 * @param filename Filename of file that the process data is read from
 * @return char* A string contenting the contents of the file
 */
char *read_process_data(char *filename);

/**
 * @brief Create a process table
 *
 * @return Linked_list* Returns the process table, which is a linked list
 */
Linked_list *create_process_table();

/**
 * @brief Removes the terminated process from the process table
 *
 * @param process_table The linked list representing the process table
 * @return int* A array will the PIDs of all the terminated processes with -1 marking the end of the array
 */
void remove_terminated_processes(Linked_list *process_table);

/**
 * @brief Gets the index of process that is running
 *
 * @param process_table The process table of the all the PCBs
 */
int find_running_process(Linked_list *process_table);

/**
 * @brief Find a process in the process table using the PID
 *
 * @param process_table A linked list that is used to represent the process table
 * @param PID The PID we are looking for
 * @return pcb_t* A pointer to the PCB of the process with PID passed in or NULL if the process is not found
 */
pcb_t *find_by_PID(Linked_list *process_table, int PID);

/**
 * @brief Create a process iterator
 *
 * @param data The process data as a string from a file
 * @param length The length of the process data
 * @param process_table A linked list that represents the process table
 * @param current_time A pointer to the current time, which is the time the CPU has run for
 * @return Process_iterator* The iterator struct
 */
Process_iterator *create_process_iterator(char *data, int length, Linked_list *process_table, int *current_time);

/**
 * @brief Processes any new processes that would have arrived
 *
 * @param self The process iterator
 * @return int* A array with the indexes in the process table of all the newly arrived processes with -1 marking the end of the array
 */
int *next_processes(Process_iterator *self);

/**
 * @brief Generates the process output string
 *
 * @param self The process iterator
 * @param process The process for which you want to generate the output string
 * @return Process_str A structure where the process string is stored in str and the length of the string in len
 */
Process_str generate_process_str(Process_iterator *self, pcb_t *process);

#endif