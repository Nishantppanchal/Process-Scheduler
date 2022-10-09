/**
 * @file task1-33094993.c
 * @author Nishant Panchal
 * @brief Run all the processes from the file passed or "processes.txt" using a first-come-first-serve scheduling algorithm.
 * @date Start date: 2022-10-06
 * @date Last modified date: 2022-09-26
 */

/* Import required header file */
#include "../h/task1-33094993.h"
#include "../h/cpu_controller.h"
#include "../h/scheduler.h"
#include "../h/queue.h"
/* Imports required libraries */
#include <stdlib.h>
#include <stdio.h>

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
    Scheduler *scheduler = FCFS();
    // Initialise the CPU controller
    CPU_controller *cpu_controller = initialise_CPU_controller(filename, scheduler);
    // Run the CPU
    run_CPU(cpu_controller);
    // Shutdown the CPU
    shutdown_CPU(cpu_controller, '1');

    /* Free memory */
    // Process queue
    // Free each element in the queue
    Queue *process_q = (Queue *)scheduler->process_storage;
    for (int i = 0; i < q_len(process_q); i++)
    {
        pcb_t *process = remove_at_index(process_q->linked_list, 0);
        free(process);
    }
    // Free the queue linked list
    free(process_q->linked_list);
    // Free the queue itself
    free(process_q);
    // Scheduler
    free(scheduler);

    /* Return 0 */
    return 0;
}

/* Constructor for the scheduler */
Scheduler *FCFS()
{
    /* Allocate memory for the scheduler */
    Scheduler *scheduler = (Scheduler *)malloc(sizeof(Scheduler));

    /* Set the process storage */
    Queue *process_q = create_queue();              // Allocate memory for the process queue
    scheduler->process_storage = (void *)process_q; // Set the process storage to the process queue

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
    /* Get the process queue */
    Queue *process_q = (Queue *)self->process_storage;

    /* Get the PID of the previously run process */
    int prev_running_process = find_running_process(process_table);

    /* Check if there are no process left to schedule */
    // If there are no processes left in the process queue and no process was previously run or it was terminated, return -1
    if (!q_len(process_q) && prev_running_process == -1)
    {
        return -1;
    }

    /* Runs there are processes left to schedule */
    // If no process was previously run or it was terminated
    if (prev_running_process == -1)
    {
        // Get the next process to run by serving a pointer to the next PID from the process priority queue
        int *return_val_pointer = (int *)q_serve(process_q);
        // Gets the PID of the process from the pointer
        int return_val = *return_val_pointer;
        // Free the memory used to store the PID for the process priority queue
        free(return_val_pointer);
        // Return the PID of the process to run
        return return_val;
    }
    // Otherwise, continue running the previous process
    return prev_running_process;
}

/* update_add function
 * Implements update_add from scheduler.h
 */
void update_add(Scheduler *self, int *new_processes_array, Linked_list *process_table, int current_time)
{
    /* Get the process queue */
    Queue *process_q = (Queue *)self->process_storage;

    /* Add the new processes to the process queue */
    // Loop through the new processes array
    for (int i = 0; new_processes_array[i] != -1; i++)
    {
        /*Add the process to the process queue */
        // Allocate memory for the new_process_PID
        int *new_process_PID = (int *)malloc(sizeof(int));
        // Get the ith process PID from new_processes_array and set it to new_process_PID
        *new_process_PID = new_processes_array[i];
        // Append the new process PID to the process queue
        q_append(process_q, new_process_PID);
    }
}