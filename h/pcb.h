// Prevent multiple declarations when included in multiple times
#ifndef PCB_H_INCLUDED
#define PCB_H_INCLUDED

/* Special enumerated data type for process state */
typedef enum
{
    READY,
    RUNNING,
    EXIT
} process_state_t;

/* C data structure used as process control block. The scheduler should create one instance per
 * running process in the system
 */
typedef struct
{
    int PID; // Process ID

    char process_name[11]; // A string that identifies the process

    /* Times are measured in seconds */
    int entryTime;     // The time process entered the system
    int serviceTime;   // The total CPU time required by the process
    int remainingTime; // Remaining service time until completion
    int deadline;      // Deadline by which we want the task completed

    process_state_t state; // Current process start (e.g. READY)
} pcb_t;

#endif