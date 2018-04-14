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
  int i;

  // Loop Thinking->Hungry->Eating 5 times
  for (i=0; i<5; i++){
    state[*phil_number] = THINKING;
    sleep_time = get_next_number();
    pthread_mutex_lock(&mutex_time);
    print_time();
    printf("Philosopher %d THINKING for %d seconds\n", *phil_number, sleep_time);
    pthread_mutex_unlock(&mutex_time);
    sleep(sleep_time);
    
    // Spin while hungry and unable to eat
    pthread_mutex_lock(&mutex_time);
    print_time();
    printf("Philosopher %d HUNGRY\n", *phil_number);
    pthread_mutex_unlock(&mutex_time);
    state[*phil_number] = HUNGRY;
    while (1)
    {
      if (!sem_trywait(&sem_vars[(*phil_number==(NUM_OF_PHILOSOPHERS-1))?0:(*phil_number)]))
        if (!sem_trywait(&sem_vars[(*phil_number==(NUM_OF_PHILOSOPHERS-1))?(*phil_number):(*phil_number+1)]))
        {
          break;
        }
	else
	  sem_post(&sem_vars[*phil_number]);
      sleep(1);
    }
   
    state[*phil_number] = EATING;
    sleep_time = get_next_number();
    pthread_mutex_lock(&mutex_time);
    print_time();
    printf("Philosopher %d EATING   for %d seconds\n", *phil_number, sleep_time);
    pthread_mutex_unlock(&mutex_time);
    sleep(sleep_time);
    sem_post(&sem_vars[*phil_number]);
    sem_post(&sem_vars[(*phil_number + 1) % NUM_OF_PHILOSOPHERS]);
  }
}

