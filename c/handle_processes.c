/* Import required header file */
#include "../h/handle_processes.h"
#include "../h/linked_list.h"
#include "../h/int_functions.h"
#include "../h/str_functions.h"
/* Imports required libraries */
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* read_process_data function */
char *read_process_data(char *filename)
{
    /* Open the file */
    // Open the file in read only mode
    int file = open(filename, O_RDONLY);
    // If there is an error opening the file
    if (file < 0)
    {
        // Print an error message
        perror("Error opening processes file");
        // Exit the program with the status code 1
        exit(1);
    }

    /* Create the buffer */
    // Get the length of the file
    int length = (int)lseek(file, 0, SEEK_END);
    // Create a buffer to store the contents of the file
    char *buffer = (char *)malloc(sizeof(char) * (length + 1));

    /* Read the file */
    // Move the file pointer to the start of the file
    lseek(file, 0, SEEK_SET);
    // Read the contents of the file into the buffer
    read(file, buffer, length);

    /* Close the file */
    close(file);

    /* Terminate the buffer */
    buffer[length] = '\0';

    /* Return the buffer */
    return buffer;
}

/* create_process_table function */
Linked_list *create_process_table()
{
    /* Create the process table */
    // Creates the linked list used to implement the process table
    Linked_list *process_table = create_linked_list();

    /* Return the process table */
    return process_table;
}

/* remove_terminated_processes function */
void remove_terminated_processes(Linked_list *process_table)
{
    /* Declare and initialise variables */
    int i = 0; // Set the start index, i, to 0

    /* Remove the processes in exit state */
    // While i is less than the length of the process table
    while (i < ll_len(process_table))
    {
        // If the process is in exit state
        if (((pcb_t *)get_at_index(process_table, i))->state == EXIT)
        {
            // Remove the process from the process table
            pcb_t *process = remove_at_index(process_table, i);
            // Free the memory allocated for the process
            free(process);
        }
        else
        {
            // Increment i
            i++;
        }
    }
}

/* find_running_process function */
int find_running_process(Linked_list *process_table)
{
    /* Find the running process */
    // For each process in the process table
    for (int i = 0; i < ll_len(process_table); i++)
    {
        // Get a pointer to the pcb of the process
        pcb_t *process = (pcb_t *)get_at_index(process_table, i);

        // If the process is in running state
        if (process->state == RUNNING)
        {
            // Return the index we are at
            return process->PID;
        }
    }
    // Return -1 if no process is in running state
    return -1;
}

/* find_by_PID function */
pcb_t *find_by_PID(Linked_list *process_table, int PID)
{
    /* Binary search algorithm since the PIDs are generated so that they always increasing
     * This means that the linked is sorted in ascend PID order by default
     */

    /* Declare variables */
    int mid;        // Stores the middle index
    pcb_t *process; // Stores the process at the middle index
    /* Declare and initialise variables */
    int low = 0;
    int high = ll_len(process_table) - 1;

    /* Find process */
    // While low is less than or equal to high
    while (low <= high)
    {
        // Calculate the middle index and set mid to it
        mid = (low + high) / 2;
        // get the pointer to the pcb of the process at the middle index and process to it
        process = (pcb_t *)get_at_index(process_table, mid);

        // If the PID passed in is greater than the PID of the process at the middle index
        if (PID > process->PID)
        {
            // Set low to middle index + 1
            low = mid + 1;
        }
        // Else if the PID passed in is the same as the PID of the process at the middle index
        else if (PID == process->PID)
        {
            // Return a pointer to the process at the middle index
            return process;
        }
        /* Otherwise, set high to middle index - 1
         * This occurs when PCB passed in is smaller than the PID of the process at the middle index
         */
        else
        {
            high = mid - 1;
        }
    }

    /* Process not found */
    return NULL; // Return NULL if the process is not found
}

/* create_process_iterator function */
Process_iterator *create_process_iterator(char *data, int length, Linked_list *process_table, int *current_time)
{
    /* Create the process iterator */
    // Allocate memory for the process iterator
    Process_iterator *iterator = (Process_iterator *)malloc(sizeof(Process_iterator));

    /* Defines the initial values of the variables in the process iterator */
    iterator->yield_pos = 0;                 // Set the yield position to 0
    iterator->data = data;                   // Set the data to the data passed
    iterator->read_all_processes = 0;        // Set the read all processes flag to 0
    iterator->length = length;               // Set the length to the length passed
    iterator->process_table = process_table; // Set the process table to the process table passed
    iterator->current_time = current_time;   // Set the current time to the current time passed

    /* Define the functions in the process iterator */
    iterator->next_processes = next_processes; // Set the next processes function to the next processes function

    /* Return the process iterator */
    return iterator;
}

/* next_processes function */
int *next_processes(Process_iterator *self)
{
    /* Declare variables */
    int j; // Used to track the number of processes that have been added to the process table
    /* Declare and initialise variables */
    int i = self->yield_pos;                                          // Used to track the position in the data
    int max_array_size = (self->length - i) + 1;                      // The possible maximum length of the array of PIDs of the added processes
    int *new_processes = (int *)malloc(sizeof(int) * max_array_size); // Allocate memory for the array of PIDs of the added processes

    /* Read the next processes */
    // While we have not reached the end of the data
    for (j = 0; i < self->length; j++)
    {
        /* Create a pcb for the process */
        pcb_t *pcb = (pcb_t *)malloc(sizeof(pcb_t)); // Allocate memory for the pcb

        /* Save the original position */
        int original_i = i;

        /* Read the process name */
        // Copy the string from the data at the index i until it reaches the a space to the process name
        copy_str(self->data, &i, ' ', pcb->process_name);

        /* Reads the entry time */
        // Convert the number from index i to the next space from string to integer and save it to the entry time
        pcb->entryTime = str_to_int(self->data, &i, ' ');

        /* Exit function is the current process can't enter the system yet */
        // If the entry time is not the current time
        if (pcb->entryTime != *(self->current_time))
        {
            /* Clean up */
            // Set the yield position to the original position since the current process will not enter the system
            self->yield_pos = original_i;
            // Free the memory allocated for the pcb
            free(pcb);
            // Add -1 to the end of the array of PIDs of the added processes to indicate the end of the array
            new_processes[j] = -1;

            /* Return the array of PIDs of the added processes */
            return new_processes;
        }

        /* Read the service time */
        // Convert the number from index i to the next space from string to integer and save it to the service time
        pcb->serviceTime = str_to_int(self->data, &i, ' ');
        // Set the remaining time to the service time since the process would not have been running yet
        pcb->remainingTime = pcb->serviceTime;

        /* Read the deadline */
        // Convert the number from index i to the next space from string to integer and save it to the deadline
        pcb->deadline = str_to_int(self->data, &i, '\n');

        /* Set the state of the process */
        pcb->state = READY; // Set the state of the process to ready

        /* Set PID of the process */
        pcb->PID = i; // Set the PID of the process to i since i will always be different for each process

        /* Add the pcb to the process table */
        // Adds the pcb of the pcb to the array of PIDs of the added processes
        new_processes[j] = pcb->PID;
        // Insert the pcb to the process table
        insert_at_index(self->process_table, ll_len(self->process_table), pcb);
        // Print the process that has been added to the process table
        // printf("Time %i: %s has entered the system.\n", *self->current_time, pcb->process_name);
        printf("Time %i: %s has entered the system.\n", *self->current_time, pcb->process_name);
    }

    /* Logic if all the processes have been read */
    // If the read all processes flag is not set to 1 yet
    if (!self->read_all_processes)
    {
        // Free the memory allocated for the file data
        free(self->data);
        // Set the pointer to the file data to a null pointer
        self->data = NULL;
        // Set the read all processes flag to 1
        self->read_all_processes = 1;
        // Set the yield position to the length of the data
        self->yield_pos = self->length;
    }

    /* Mark the end of the array of PIDs of the added processes */
    new_processes[j] = -1; // Add -1 to the end of the array of PIDs of the added processes to indicate the end of the array

    /* Return the array of PIDs of the added processes */
    return new_processes;
}

/* generate_process_str function */
Process_str generate_process_str(Process_iterator *self, pcb_t *process)
{
    /* Process name */
    int name_len = string_len(process->process_name); // Get the length of the process name and save it to the name_len variable

    /* Turnaround time */
    // Calculate the turnaround time and save it to the turnaround_time variable
    int turnaround_time = *self->current_time - process->entryTime;
    // Get the length of the turnaround time and save it to the turnaround_time_len variable
    int turnaround_time_len = int_len(turnaround_time) + 1; // +1 for the null terminator
    // Allocate memory for the turnaround time string
    char *turnaround_time_str = (char *)malloc(sizeof(char) * (turnaround_time_len));
    // Convert the turnaround time to a string and save it to the turnaround_time_str variable
    int_to_str(turnaround_time_str, turnaround_time, turnaround_time_len);

    /* Waiting time */
    // Calculate the waiting time and save it to the waiting_time variable
    int wait_time = turnaround_time - process->serviceTime;
    // Get the length of the waiting time and save it to the waiting_time_len variable
    int wait_time_len = int_len(wait_time) + 1; // +1 for the null terminator
    // Allocate memory for the waiting time string
    char *wait_time_str = (char *)malloc(sizeof(char) * (wait_time_len)); // +1 for the null terminator
    // Convert the waiting time to a string and save it to the waiting_time_str variable
    int_to_str(wait_time_str, wait_time, wait_time_len);

    /* Deadline met */
    // Set the deadline_met_str to the character 1 if the process met its deadline, otherwise set it to the character 0
    char *deadline_met_str = turnaround_time > process->deadline ? "0" : "1";
    // Sets the deadline_met_len to 1 since the deadline met flag is either 0 or 1
    int deadline_met_len = 2; // +1 for the null terminator

    /* Total length */
    /* Add 4 for the spaces, the newline character and terminating null character
     * -1 on each length since the null terminator is already included at the end when we +5
     */
    int total_len = (name_len - 1) + (turnaround_time_len - 1) + (wait_time_len - 1) + (deadline_met_len - 1) + 5;

    /* Create the string */
    // Allocate memory for the string
    char *result_str = (char *)malloc(sizeof(char) * total_len);
    // Creates an array of pointers to the strings
    char *strings[] = {process->process_name, wait_time_str, turnaround_time_str, deadline_met_str};
    // Concatenates the strings together with spaces in between
    concat(result_str, strings, 4, ' ');
    // Add a newline character to the second last character of the string
    result_str[total_len - 2] = '\n';
    // Add a terminating null character to the end of the string
    result_str[total_len - 1] = '\0';

    /* Create the Process_str struct that is returned */
    Process_str process_str;      // Initialize the Process_str struct
    process_str.str = result_str; // Set the string to the result_str variable
    process_str.len = total_len;  // Set the length of the string to the total_len variable

    /* Free the memory allocated */
    free(turnaround_time_str); // Free the memory allocated for the turnaround time string
    free(wait_time_str);       // Free the memory allocated for the waiting time string

    /* Return the Process_str struct */
    return process_str;
}