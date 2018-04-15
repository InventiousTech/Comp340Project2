/*
 *  philosopher.c
 *  Author:       Isaac Hendrickson
 *  Date:         April 10th, 2018
 
 *  Purpose:      This program implements the philosophers for Task 1 of Project 2 in COMP 340
 *                In this problem, there are 5 philosophers and 5 chopsticks at the table.
 *                Each philosopher may only eat if he can grab both chopsticks adjacent to him.
 *                Our method for preventing deadlock is to alternate whether the philosopher
 *                chooses the chopstick to his left or right first. Even numbered philosophers choose
 *                the chopstick to their left (lower number), odd philosophers choose to the right (higher number).
 * 
 *  Note:         This program uses two mutex locks. One to protect the random-number choosing, and one
 *                to ensure that the information printed to the console is legible (no interference from other philosophers).
 */

#include "dp.h"
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>

void *philosopher_loop(void *param)
{
  /* The ID of this philosopher */
  int *phil_number = (int *)param;
  
  /* Timing Variables */
  struct timespec wait_start, wait_end;
  double wait_time;
  int sleep_time;
  int i;
  
  /* The two chopsticks adjacent to this philosopher.  */
  /* stick1 is the first stick the philosopher picks up, stick2 is the second */
  int stick1, stick2; 
  stick1 = ((*phil_number%2)==0)?(*phil_number):((*phil_number+1) % NUM_OF_PHILOSOPHERS);
  stick2 = ((*phil_number%2)!=0)?(*phil_number):((*phil_number+1) % NUM_OF_PHILOSOPHERS);
  
  /* Loop Thinking->Hungry->Eating 5 times */
  for (i=0; i<NUM_OF_LOOPS; i++){
    
    // ****** THINKING **************
    state[*phil_number] = THINKING;
    sleep_time = get_next_number();
    pthread_mutex_lock(&mutex_print);
    print_time();
    printf("Philosopher %d THINKING for %d seconds\n", *phil_number, sleep_time);
    pthread_mutex_unlock(&mutex_print);
    sleep(sleep_time);
    // ******************************
    
    
    // ********** HUNGRY ************
    /* Print out to console */
    state[*phil_number] = HUNGRY;
    clock_gettime(CLOCK_MONOTONIC, &wait_start);
    pthread_mutex_lock(&mutex_print);
    print_time();
    printf("Philosopher %d HUNGRY\n", *phil_number);
    pthread_mutex_unlock(&mutex_print);
    
    /* Acquire Chopsticks, stick1 then stick2 */
    if (verbose) printf("Philosopher %d locking chopstick %d\n", *phil_number, stick1);
    sem_wait(&sem_vars[stick1]);
    if (verbose) printf("Philosopher %d locking chopstick %d\n", *phil_number, stick2);
    sem_wait(&sem_vars[stick2]);
    if (verbose) printf("Philosopher %d got both locks\n", *phil_number);
    clock_gettime(CLOCK_MONOTONIC, &wait_end);
    // ******************************
    
    // *** record timing ****
    wait_time = ((double)wait_end.tv_sec + (double)wait_end.tv_nsec/1000000000.0) - ((double)wait_start.tv_sec + (double)wait_start.tv_nsec/1000000000.0);
    pthread_mutex_lock(&mutex_time);
    if (wait_time > max_wait_time)
      max_wait_time = wait_time;
    wait_time_array[wait_index] = wait_time;
    wait_index++;
    pthread_mutex_unlock(&mutex_time);
    // **********************
   
   
    // ********* EATING *************
    state[*phil_number] = EATING;
    sleep_time = get_next_number();
    pthread_mutex_lock(&mutex_print);
    print_time();
    printf("Philosopher %d EATING   for %d seconds\n", *phil_number, sleep_time);
    pthread_mutex_unlock(&mutex_print);
    sleep(sleep_time);
    sem_post(&sem_vars[stick2]);
    sem_post(&sem_vars[stick1]);
    // ******************************
  }
}

