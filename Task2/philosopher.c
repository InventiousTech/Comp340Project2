/*
 *  philosopher.c
 *  Author:       Isaac Hendrickson and James Ahrens
 *  Date:         April 10th, 2018
 
 *  Purpose:      This program implements the philosophers for Task 2 of Project 2 in COMP 340
 *                In this problem, there are N philosophers and N+1 chopsticks at the table.
 *                Each philosopher may only eat if he can grab either both chopsticks adjacent to him, 
 *                or one adjacent chopstick and the one in the center of the table.
 *                If a philosopher cannot secure two chopsticks, he waits on a condition variable before he tries again.
 *                That variable is signalled by every philosopher when he finishes eating, so any time new chopsticks become available, 
 *                every waiting philosopher loops through to test again
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
  
  /* Variable for tracking which chopsticks we picked up */
  enum {LR, LC, RC} held_chopsticks;
  
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
    
    /* Acquire Chopsticks */
    pthread_mutex_lock(&mutex_lock);
    while (1){
      
      /* First try to pick up chopstick 1 */
      if (verbose) printf("Philosopher %d attempting to pickup chopsticks\n", *phil_number);
      if (!sem_trywait(&sem_vars[stick1])){
      
        /* If successful, try chopstick 2 */
        if (verbose) printf("Philosopher %d grabbed chopstick %d\n", *phil_number, stick1);
        if (!sem_trywait(&sem_vars[stick2])){
          /* SUCCESS. break out of the loop */
          if (verbose) printf("Philosopher %d grabbed chopstick %d\n", *phil_number, stick2);
          held_chopsticks = LR;
          break;
        }
        
        /* Otherwise, try the center chopstick */
        else if (!sem_trywait(&sem_central)){
          /* SUCCESS. break out of the loop */
          if (verbose) printf("Philosopher %d grabbed center chopstick\n", *phil_number, stick2);
          held_chopsticks = LC;
          break;
        }
        else{
          if (verbose) printf("Philosopher %d put down chopstick %d\n", *phil_number, stick1);
          sem_post(&sem_vars[stick1]);
        }
      }
      
      /* If we can't get chopstick 1, try chopstick 2 */
      else if (!sem_trywait(&sem_vars[stick2])){
        if (verbose) printf("Philosopher %d grabbed chopstick %d\n", *phil_number, stick2);
        
        /* If we got chopstick 2, try the center one */
        if (!sem_trywait(&sem_central)){
          /* SUCCESS. break out of the loop */
          if (verbose) printf("Philosopher %d grabbed center chopstick\n", *phil_number, stick2);
          held_chopsticks = RC;
          break;
        }
        else{
          if (verbose) printf("Philosopher %d put down chopstick %d\n", *phil_number, stick2);
          sem_post(&sem_vars[stick2]);
        }
      }
      pthread_cond_wait(&cond_var, &mutex_lock);  // If we haven't gotten chopsticks, wait until someone finishes eating
      if (verbose) printf("Philosopher %d just got signalled to check chopsticks\n", *phil_number);
    }
    pthread_mutex_unlock(&mutex_lock);
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
    if (held_chopsticks == LR){
      sem_post(&sem_vars[stick2]);
      sem_post(&sem_vars[stick1]);
    }
    else if (held_chopsticks == LC){
      sem_post(&sem_vars[stick1]);
      sem_post(&sem_central);
    }
    else if (held_chopsticks == RC){
      sem_post(&sem_vars[stick2]);
      sem_post(&sem_central);
    }
    if (verbose) printf("Philosopher %d sending signal that we put chopsticks down\n", *phil_number);
    pthread_cond_broadcast(&cond_var);  // Signal all waiting threads that we just put down chopsticks
    // ******************************
  }
}