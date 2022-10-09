/* Import required header file */
#include "../h/scheduler.h"
#include "../h/priority_queue.h"
#include "../h/queue.h"

/* Custom process storage structure */
typedef struct
{
    Priority_queue *deadline_pq; // Priority queue for process selection when scheduler is trying to met deadline
    Queue *end_q;                // Queue for process whose deadline can't be met and hence are run at the end
} Process_storage_structure;

/**
 * @brief Constructor for the deadline based scheduler
 *
 * @return Scheduler* A pointer to the scheduler
 */
Scheduler *DB();

/**
 * @brief Cost function of a process used to determine which process will be run next
 * @details The cost function is calculated using the formula: ((deadline + entry time) - current time)^(remaining time).
 * @details The formula favors processes in the following order (1 - lowest cost and 3 - highest cost):
 * @details 1. Low remaining time and low ((deadline + entry time) - current time) value
 * @details 2. Low remaining time and high ((deadline + entry time) - current time) value
 * @details 3. High remaining time and high ((deadline + entry time) - current time) value
 *
 * @param remaining_time The amount of more time needed on the CPU for the process to be completed
 * @param deadline The deadline of the process
 * @param entry_time The time the process entered the system
 * @param current_time The current time, which is the time the CPU has run for
 * @return int The cost of the process
 */
int cost_function(int remaining_time, int deadline, int entry_time, int current_time);

/**
 * @brief Moves all process in a priority queue whose deadline can no longer be met to a separate queue
 * @attention This function is for the deadline based scheduling algorithm
 *
 * @param pq The priority queue
 * @param q The queue
 * @param process_table A pointer to the linked list representing the process table
 * @param current_time The current time, which is the time the CPU has run for
 */
void DB_move_priority_to_queue(Priority_queue *pq, Queue *q, Linked_list *process_table, int current_time);

/**
 * @brief Recalculates the cost of all process in a priority queue
 *
 * @param pq The priority queue
 * @param process_table A pointer to the linked list representing the process table
 * @param current_time The current time, which is the time the CPU has run for
 */
void update_costs(Priority_queue *pq, Linked_list *process_table, int current_time);