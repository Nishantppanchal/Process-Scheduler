/* Import required header file */
#include "../h/cpu_controller.h"
#include "../h/cpu.h"
#include "../h/handle_processes.h"
#include "../h/scheduler.h"
#include "../h/str_functions.h"
/* Imports required libraries */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

/* initialise_CPU_controller function */
CPU_controller *initialise_CPU_controller(char *filename, Scheduler *scheduler)
{
    // Reads and returns all process data in file with the passed filename
    char *file_data = read_process_data(filename);

    // Allocate memory for the CPU controller and store a pointer to the memory
    CPU_controller *cpu_controller = (CPU_controller *)malloc(sizeof(CPU_controller));

    /* Output data */
    cpu_controller->result_str = (char *)malloc(sizeof(char)); // Allocates memory for a string with just the termination character
    cpu_controller->result_str[0] = '\0';                      // Add termination character to result_str
    cpu_controller->results_len = 0;                           // Sets the initial length of the result_str to 0

    /* Components that have to be managed */
    cpu_controller->cpu = create_CPU();                     // Creates the CPU and sets cpu to a pointer to the cpu
    cpu_controller->process_table = create_process_table(); // Creates a process table and sets process_table to a pointer to the process_table
    cpu_controller->scheduler = scheduler;                  // Sets scheduler to the scheduler pointer passed
    // Creates a process iterator and set process_iterator to a pointer to this process iterator
    cpu_controller->process_iterator = create_process_iterator(
        file_data,                             // The process read form the file
        string_len(file_data),                 // The number of characters in file_data
        cpu_controller->process_table,         // A pointer to the process table
        &(cpu_controller->cpu->current_time)); // A pointer to the current time

    // Returns the CPU_controller created
    return cpu_controller;
}

/* run_CPU function */
void run_CPU(CPU_controller *self)
{
    /* Declares variables */
    int PID;
    pcb_t *process_running;
    Process_str process_str;

    // While there are process that have not entered the system yet or a process was previously run, keep looping the loop
    while (!(self->process_iterator->read_all_processes && PID == -1))
    {
        /* Processing new processes entering system */
        // Add any new processes that have arrived to the system into the process table and store an array of the PIDs of the new processes
        int *new_processes_array = new_processes_array = self->process_iterator->next_processes(self->process_iterator);
        // Updates the scheduler with the new processes
        self->scheduler->update_add(self->scheduler, new_processes_array, self->process_table, self->cpu->current_time);
        // Free the memory allocated for the array of new processes
        free(new_processes_array);

        /* Schedules processes */
        // Gets the PIDs of the process in the process table this is to be run
        PID = self->scheduler->schedule(self->scheduler, self->process_table, self->cpu->current_time);
        // Set the process running to the process returned by the scheduler
        self->cpu->process_running = PID;

        /* Set the process running to the running state */
        // If the CPU is not idle
        if (self->cpu->process_running != -1)
        {
            // Get the pointer to pcb of the process running
            process_running = (pcb_t *)find_by_PID(self->process_table, self->cpu->process_running);
            // If the process is not already in running state
            if (process_running->state != RUNNING)
            {
                // Set the process in to running state
                process_running->state = RUNNING;
                // Prints a message saying that the process is in running state to standard output
                printf("Time %i: %s is in the running state.\n", self->cpu->current_time, process_running->process_name);
            }
        }

        /* Running the process */
        sleep(1); // Assume all processes simply sleep

        // Increments the current time tracked by the CPU
        self->cpu->current_time++;

        /* Set the process that was run to exit state if it has completed running */
        // If the CPU was not idle
        if (PID != -1)
        {
            // Get the pointer to the pcb of process that was run
            process_running = (pcb_t *)find_by_PID(self->process_table, self->cpu->process_running);
            // Decrements the remaining time of the process
            process_running->remainingTime--;

            // If the remaining time of the process is 0
            if (!process_running->remainingTime)
            {
                // Sets the state of the process that ran to exit state
                process_running->state = EXIT;
                // Print a message saying that the process has finished executing to standard output
                printf("Time %i: %s has finished execution.\n", self->cpu->current_time, process_running->process_name);

                // Generates the output string of the process since it has finished running
                process_str = generate_process_str(self->process_iterator, process_running);
                // Appends the output string of the process to the end of the result_str and set the results_len to the new length of the result_str
                self->results_len = append_to_str(&self->result_str, self->results_len, process_str.str, process_str.len);
                // Free the memory allocated for the output string in process_str
                free(process_str.str);
            }
        }

        /* Remove terminated processes */
        // Remove all the process in exit state from the process table
        remove_terminated_processes(self->process_table);
    }
}

/* shutdown_CPU function */
void shutdown_CPU(CPU_controller *self, char task_num)
{
    /* Creates/opens the file */
    // Generates the filename of the output file
    char filename[] = {'r', 'e', 's', 'u', 'l', 't', 's', '-', task_num, '.', 't', 'x', 't'};
    /* Creates/opens the output file
     * Everything in the file will be cleared if it already exists
     */
    int results_file = open(filename, O_WRONLY | O_CREAT, 0664);
    // If the output file can't created or opened
    if (filename < 0)
    {
        // Print error message
        perror("Error creating/writing to results file");
        // Exit the program with the status code 1
        exit(1);
    }

    /* Writes the output to the file */
    // Move the file pointer to the start of the file
    lseek(results_file, 0, SEEK_SET);
    // Write the output to the file
    write(results_file, self->result_str, self->results_len);

    /* Closes the file */
    close(results_file);

    /* Free memory */
    free(self->result_str);       // Free the memory allocated for the result_str
    free(self->cpu);              // Free the memory allocated for the CPU
    free(self->process_table);    // Free the memory allocated for the process table
    free(self->process_iterator); // Free the memory allocated for the process iterator
    free(self);
}