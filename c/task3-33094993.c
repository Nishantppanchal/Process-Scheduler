/**
 * @file task1-33094993.c
 * @author Nishant Panchal
 * @brief Run all the processes from the file passed or "processes.txt" using a custom deadline based scheduling algorithm.
 * @date Start date: 2022-10-06
 * @date Last modified date: 2022-09-26
 */

/* Import required header file */
#include "../h/task3-33094993.h"
#include "../h/cpu_controller.h"
#include "../h/scheduler.h"
#include "../h/queue.h"
#include "../h/int_functions.h"
/* Imports required libraries */
#include <stdlib.h>
#include <stdio.h>

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
    Scheduler *scheduler = DB();
    // Initialise the CPU controller
    CPU_controller *cpu_controller = initialise_CPU_controller(filename, scheduler);
    // Run the CPU
    run_CPU(cpu_controller);
    // Shutdown the CPU
    shutdown_CPU(cpu_controller, '3');

    /* Free memory */
    // Process storage structure
    Process_storage_structure *pss = (Process_storage_structure *)scheduler->process_storage;
    // Priority queue
    Priority_queue *pq = (Priority_queue *)pss->deadline_pq;
    // Free priority queue's linked list
    free(pq->linked_list);
    // Free priority queue itself
    free(pq);
    // Queue
    Queue *q = (Queue *)pss->end_q;
    // Free queue's linked list
    free(q->linked_list);
    // Free queue itself
    free(q);
    // Free process storage structure
    free(pss);
    // Scheduler
    free(scheduler);

    /* Return 0 */
    return 0;
}

Scheduler *DB()
{
    /* Allocate memory for the scheduler */
    Scheduler *scheduler = (Scheduler *)malloc(sizeof(Scheduler));

    /* Set process storage */
    Process_storage_structure *process_storage = (Process_storage_structure *)malloc(sizeof(Process_storage_structure)); // Allocate memory for the process storage structure
    // Creates a priority queue used to scheduling process in order to return meet their deadline
    Priority_queue *deadline_pq = create_priority_queue();
    // Set deadline_pq in the process storage structure to the priority pointer from above
    process_storage->deadline_pq = deadline_pq;
    // Creates a queue used to store processes whose deadline cannot be met
    Queue *end_q = create_queue();
    // Set end_q in the process storage structure to the queue pointer from above
    process_storage->end_q = end_q;
    // Set the process storage to the process storage structure created
    scheduler->process_storage = (void *)process_storage;

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
    /* Get the process storage structure */
    Process_storage_structure *process_storage = (Process_storage_structure *)self->process_storage;

    /* Get the PID of the previously run process */
    int *prev_running_process = (int *)malloc(sizeof(int));
    *prev_running_process = find_running_process(process_table);

    /* Check if there are no process left to schedule */
    /* If there are no processes left in the both the deadline_pq priority queue or the end_q queue and no process was previously run or it was
     * terminated, return -1
     */
    if (!p_len(process_storage->deadline_pq) && !q_len(process_storage->end_q) && *prev_running_process == -1)
    {
        // Free the memory used to the store the PID of the previously run process
        free(prev_running_process);
        // Return -1
        return -1;
    }

    /* Check if there are processes in the deadline_pq priority queue */
    if (p_len(process_storage->deadline_pq))
    {
        /* Case 1: No process previous running or it was terminated */
        // If no process was previously run or it was terminated
        if (*prev_running_process == -1)
        {
            // Get the next process to run by serving a pointer to the next PID from the deadline_pq priority queue
            int *return_val_pointer = (int *)p_serve(process_storage->deadline_pq);
            // Gets the PID of the process from the pointer
            int return_val = *return_val_pointer;
            // Free the memory used to store the PID for the process priority queue
            free(return_val_pointer);
            // Free the memory used to the store the PID of the previously run process
            free(prev_running_process);
            // Return the PID of the next process to run
            return return_val;
        }

        /* Case 2: A process was previously running */

        // Get the previously run process from the process table
        pcb_t *process = (pcb_t *)find_by_PID(process_table, *prev_running_process);

        // Update costs of processes in the deadline_pq priority queue
        update_costs(process_storage->deadline_pq, process_table, current_time);

        // Calculates the new cost of the previously run process
        int cost = cost_function(process->remainingTime, process->deadline, process->entryTime, current_time);

        /* Appends the process to the deadline_pq priority queue
         * Cost is negative to make the smallest cost the highest priority
         */
        p_append(process_storage->deadline_pq, prev_running_process, -cost);

        // Move all processes in the queue whose deadline cannot be met to the end_q queue
        DB_move_priority_to_queue(process_storage->deadline_pq, process_storage->end_q, process_table, current_time);

        // If there are no longer any processes in the deadline_pa priority queue
        if (!p_len(process_storage->deadline_pq))
        {
            // Get the next process to run by serving a pointer to the next PID from the process queue
            int *return_val_pointer = (int *)q_serve(process_storage->end_q);
            // Gets the PID of the process from the pointer
            int return_val = *return_val_pointer;
            // Free the memory used to store the PID for the process priority queue
            free(return_val_pointer);
            // Return the PID of the next process to run
            return return_val;
        }

        // Get the next process to run by serving a pointer to the next PID from the process priority queue
        int *return_val_pointer = (int *)p_serve(process_storage->deadline_pq);
        // Gets the PID of the process from the pointer
        int return_val = *return_val_pointer;

        /* Reset state */
        // If the new process to run is not the same as the previously run process
        if (return_val != *prev_running_process)
        {
            // Set the state of the process to ready
            process->state = READY;
        }
        // Free the memory used to store the PID for the process priority queue
        free(return_val_pointer);

        /* Return the PID of the process to run */
        return return_val;
    }
    /* If there are no processes in the deadline_pq priority queue */
    else
    {
        // If no process was previously run or it was terminated
        if (*prev_running_process == -1)
        {
            // Get the next process to run by serving a pointer to the next PID from the end_q queue
            int *return_val_pointer = (int *)q_serve(process_storage->end_q);
            // Gets the PID of the process from the pointer
            int return_val = *return_val_pointer;
            // Free the memory used to store the PID for the process priority queue
            free(return_val_pointer);
            // Free the memory used to store the PID of the previously run process
            free(prev_running_process);
            // Return the PID of the next process to run
            return return_val;
        }
        // Otherwise, return the PID of the previously run process
        int return_val = *prev_running_process; // Place previously running process' PID in return_val
        free(prev_running_process);             // Free memory
        return return_val;
    }
}

/* update_add function
 * Implements update_add from scheduler.h
 */
void update_add(Scheduler *self, int *new_processes_array, Linked_list *process_table, int current_time)
{
    /* Get process storage */
    Process_storage_structure *process_storage = (Process_storage_structure *)self->process_storage;

    /* Add new processes */
    // For all processes in new_processes_array
    for (int i = 0; new_processes_array[i] != -1; i++)
    {
        /* Get new process' PID */
        // Allocate memory for the new process' PID
        int *new_process_PID = (int *)malloc(sizeof(int));
        // Get the new PID from the new_processes_array
        *new_process_PID = new_processes_array[i];

        // Get a pointer to the process pcb using its PID
        pcb_t *process = (pcb_t *)find_by_PID(process_table, *new_process_PID);

        // Calculates the cost of the process
        int cost = cost_function(process->remainingTime, process->deadline, process->entryTime, current_time);

        // Append the new process to the deadline_pq priority queue with the priority was -cost
        p_append(process_storage->deadline_pq, new_process_PID, -cost);
    }
}

/* cost_function function */
int cost_function(int remaining_time, int deadline, int entry_time, int current_time)
{
    // Calculates the cost of the process
    int cost = power((deadline + entry_time) - current_time, remaining_time);
    // Returns the cost of the process
    return cost;
}

/* DB_move_priority_to_queue function */
void DB_move_priority_to_queue(Priority_queue *pq, Queue *q, Linked_list *process_table, int current_time)
{
    /* Declare variables */
    int max_cost;      // Stores the maximum cost process can have before it's deadline is missed
    int deadline_time; // Stores the time at which the process must be completed by to meet its deadline

    /* Move all processes in the priority queue whose deadline can no longer be met to the queue */
    // For each process PID in the priority queue
    for (int i = 0; i < p_len(pq); i++)
    {
        /* Get the ith process PID and priority */
        // Get the element at the ith index of the linked list that is used to implement the priority queue
        Element *element = (Element *)get_at_index(pq->linked_list, i);
        // Get the priority of the element
        int element_priority = element->priority;
        // Get the process PID stored in the element
        int PID = *((int *)element->data);

        /* Get the process */
        // Get the process with the PID of the value in the PID variable
        pcb_t *process = (pcb_t *)find_by_PID(process_table, PID);

        /* Calculate the maximum value after which the deadline can't be met
         * The maximum value would be when the remaining time > deadline time - current time.
         * Hence we can substitute (deadline time - current time) with remaining time into the cost function and multiple
         * it by -1 to get the maximum value since the priorities are -cost.
         * Look at the cost function above for more details about cost function.
         */
        max_cost = -power(process->remainingTime, process->remainingTime);
        deadline_time = process->deadline + process->entryTime;

        /* If the priority of the element is greater than the maximum value or the deadline time has passed */
        if (element_priority > max_cost || deadline_time < current_time)
        {
            // Remove the element from the priority queue
            remove_at_index(pq->linked_list, i);
            // Add the process PID to the queue
            q_append(q, element->data);

            /* Free memory */
            free(element);
        }
    }
}

/* update_costs function */
void update_costs(Priority_queue *pq, Linked_list *process_table, int current_time)
{
    for (int i = 0; i < p_len(pq); i++)
    {
        /* Get the ith process PID and priority */
        // Get the element at the ith index of the linked list that is used to implement the priority queue
        int *PID = (int *)p_serve(pq);

        /* Get the process */
        // Get the process with the PID of the value in the PID variable
        pcb_t *process = (pcb_t *)find_by_PID(process_table, *PID);

        /* Calculate the new cost */
        int cost = cost_function(process->remainingTime, process->deadline, process->entryTime, current_time);

        /* Add the process back to the priority queue */
        p_append(pq, PID, -cost);
    }
}