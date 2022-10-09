/**
 * @file task1-33094993.c
 * @author Nishant Panchal
 * @brief Run all the processes from the file passed or "processes.txt" using a round robin scheduling algorithm.
 * @date Start date: 2022-10-06
 * @date Last modified date: 2022-09-26
 */

/* Import required header file */
#include "../h/task2-33094993.h"
#include "../h/cpu_controller.h"
#include "../h/scheduler.h"
#include "../h/priority_queue.h"
/* Imports required libraries */
#include <stdlib.h>
#include <stdio.h>

/* Define time quantum of scheduling algorithm */
#define TIME_QUANTUM 2;

/* Main function */
int main(int argc, char *argv[])
{
    /* Check number of arguments */
    // If there are more than 2 arguments
    if (argc > 2)
    {
        // Print error message
        perror("Too many arguments passed");
        // Exit with status code 1
        exit(1);
    }

    /* Get filename */
    char *filename = argc == 1 ? "processes.txt" : argv[1]; // If no filename is given, use the default filename processes.txt

    /*Run the CPU */
    // Initialise the scheduler
    Scheduler *scheduler = RR();
    // Initialise the CPU controller
    CPU_controller *cpu_controller = initialise_CPU_controller(filename, scheduler);
    // Run the CPU
    run_CPU(cpu_controller);
    // Shutdown the CPU
    shutdown_CPU(cpu_controller, '2');

    /* Free memory */
    // Process priority queue
    Priority_queue *process_q = (Priority_queue *)scheduler->process_storage;
    // Free the queue's linked list
    free(process_q->linked_list);
    // Free the queue itself
    free(process_q);
    // Scheduler
    free(scheduler);

    /* Return 0 */
    return 0;
}

/* RR function */
Scheduler *RR()
{
    /* Allocate memory for the scheduler */
    Scheduler *scheduler = (Scheduler *)malloc(sizeof(Scheduler));

    /* Set process storage */
    Priority_queue *process_q = create_priority_queue(); // Create a priority queue for the process storage
    scheduler->process_storage = (void *)process_q;      // Set the process storage to the process priority queue

    /* Set time quantum */
    scheduler->time_quantum = TIME_QUANTUM; // Set the time quantum of the scheduler
    scheduler->time_quantum_counter = 0;    // Set the time quantum counter to 0, which is the inital value

    /* Set the scheduler function */
    scheduler->schedule = schedule;     // Set the schedule function to the schedule function
    scheduler->update_add = update_add; // Set the update_add function to the update_add function

    /* Return the scheduler pointer */
    return scheduler;
}

/* schedule function
 * Implements schedule from scheduler.h
 */
int schedule(Scheduler *self, Linked_list *process_table, int current_time)
{
    /* Get the process priority queue */
    Priority_queue *process_q = (Priority_queue *)self->process_storage;

    /* Get the PID of the previously run process */
    int *prev_running_process = (int *)malloc(sizeof(int));
    *prev_running_process = find_running_process(process_table);

    /* Check if there are no process left to schedule */
    // If there are no processes left in the process priority queue and no process was previously run or it was terminated, return -1
    if (!p_len(process_q) && *prev_running_process == -1)
    {
        // Free prev_running_process
        free(prev_running_process);
        // Return -1
        return -1;
    }

    /* Runs there are processes left to schedule */
    // If the process need to be preempted or no process was previously run/it was terminated
    if (self->time_quantum_counter == self->time_quantum || *prev_running_process == -1)
    {
        // Reset the time quantum counter to 1, since we have to add 1 from 0 to it anyway for the new process
        self->time_quantum_counter = 1;

        /* Case 1: No process previous running or it was terminated */
        // If no process was previously run or it was terminated
        if (*prev_running_process == -1)
        {
            /* Serve the next process in the queue */
            // Get the next process to run by serving a pointer to the next PID from the process priority queue
            int *return_val_pointer = (int *)p_serve(process_q);
            // Gets the PID of the process from the pointer
            int return_val = *return_val_pointer;
            // Free the memory used to store the PID for the process priority queue
            free(return_val_pointer);
            // Fre the memory used to store the PID of the previously run process
            free(prev_running_process);
            // Return the PID of the next process to return
            return return_val;
        }

        /* Case 2: Process was previous and has not finished yet */
        // Append the process back into the queue
        p_append(process_q, prev_running_process, 1);
        // Get the next process to run by serving a pointer to the next PID from the process priority queue
        int *return_val_pointer = (int *)p_serve(process_q);
        // Gets the PID of the process from the pointer
        int return_val = *return_val_pointer;

        /* Set previous running process into ready state */
        // Get pointer to the pcb of the previously running process using it's PID
        pcb_t *process = (pcb_t *)find_by_PID(process_table, *prev_running_process);
        // If the new process to run is not the same as the previously run process
        if (return_val != *prev_running_process)
        {
            // Set the state of the process to ready
            process->state = READY;
        }
        // Free the memory used to store the PID for the process priority queue
        free(return_val_pointer);

        /* Reset priorities */
        edit_priorities(process_q, 2, 1); // Set the priorities of all processes with priority of 2 to 1 since they are no longer newly arrived processes

        // Returns the PID of the process to run next
        return return_val;
    }
    // Otherwise, increment the time quantum counter and return the previous run process' PID to let it run again
    self->time_quantum_counter++;
    int return_val = *prev_running_process; // Place previously running process' PID in return_val
    free(prev_running_process);             // Free memory
    return return_val;
}

/* update_add function
 * Implements update_add from scheduler.h
 */
void update_add(Scheduler *self, int *new_processes_array, Linked_list *process_table, int current_time)
{
    /* Get the process priority queue */
    Priority_queue *process_q = (Priority_queue *)self->process_storage;

    /* Add the new processes to the process priority queue */
    // Loop through the new processes array
    for (int i = 0; new_processes_array[i] != -1; i++)
    {
        /*Add the process to the process priority queue */
        // Allocate memory for the new_process_PID
        int *new_process_PID = (int *)malloc(sizeof(int));
        // Get the ith process PID from new_processes_array and set it to new_process_PID
        *new_process_PID = new_processes_array[i];
        // Append the new process PID to the process priority queue
        p_append(process_q, new_process_PID, 2);
    }
}