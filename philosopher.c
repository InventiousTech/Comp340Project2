/**
 * 	philosopher.c
 *	Author:		Isaac Hendrickson
 *	Date: 		April 10th, 2018
 *	Purpose:	This program implements the philosophers for Project 2 in COMP 340
 */

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include "dp.h"

#define MAX_SLEEP_TIME 5

void *philosopher_loop(void *param)
{
  int *phil_number = (int *)param; // The ID of this philosopher
  int sleep_time;

  // Loop Thinking->Hungry->Eating 5 times
  for (int i=0; i<5; i++){
    state[*phil_number] = THINKING;
    sleep_time = get_next_number();
    printf("Philosopher %d Thinking for %d seconds...\n", *phil_number, sleep_time);
    sleep(sleep_time);
    
    // Spin while hungry and unable to eat
    printf("Philospher %d is hungry.\n", *phil_number);
    state[*phil_number] = HUNGRY;
    while (1)
    {
      pthread_mutex_lock(&mutex_lock);
      if (!sem_trywait(&sem_vars[*phil_number]))
        if (!sem_trywait(&sem_vars[(*phil_number + 1) % NUM_OF_PHILOSOPHERS]))
        {
          pthread_mutex_unlock(&mutex_lock);
          break;
        }
      pthread_mutex_unlock(&mutex_lock);
      sleep(1);
    }
   
    state[*phil_number] = EATING;
    sleep_time = get_next_number();
    printf("Philosopher %d Eating for %d seconds...\n", *phil_number, sleep_time);
    sleep(sleep_time);
    sem_post(&sem_vars[*phil_number]);
    sem_post(&sem_vars[(*phil_number + 1) % NUM_OF_PHILOSOPHERS]);
  }
}

