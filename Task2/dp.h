/**
 * Header file for dining philosophers
 */

#ifndef DP_H
#define DP_H

#define _POSIX_C_SOURCE 200809L

#include <pthread.h>
#include <semaphore.h>  
#include <stdlib.h>
#include <unistd.h>

// the number of philosophers
#define NUM_OF_PHILOSOPHERS		5

// The number of times each philosopher loops
#define NUM_OF_LOOPS  5

// the maximum number of random numbers
#define MAX_LENGTH 500

// the array holding the list of random numbers 
int rand_numbers[MAX_LENGTH];

//function for getting a random number from the list
int get_next_number();

//mutex lock to use in order to protect the order of random numbers
pthread_mutex_t mutex_rand;

// Mutex to make sure only one process prints info at a time
pthread_mutex_t mutex_time;

//position of next random number 
int rand_position;

// the state of each philosopher (THINKING, HUNGRY, EATING)
enum {THINKING, HUNGRY, EATING} state[NUM_OF_PHILOSOPHERS];

// Timing variables
struct timespec begin, end;
double elapsed_time;
void print_time();
double max_wait_time, average_wait_time;
double wait_time_array[NUM_OF_PHILOSOPHERS*NUM_OF_LOOPS];
int wait_index;

// Verbosity
int verbose;

// the id of each philosopher (0 .. NUMBER - 1)
int phil_id[NUM_OF_PHILOSOPHERS];

// semaphore variables and associated mutex lock
sem_t		sem_vars[NUM_OF_PHILOSOPHERS];
pthread_mutex_t 	mutex_lock;

//function that simulates the philosopher operation
void *philosopher(void *param);

//function for the philosopher to pickup the chopsticks
void pickup_chopsticks(int number);

//function for the philosopher to return the chopsticks
void return_chopsticks(int number);

#endif
