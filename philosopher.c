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
  
  // The two chopsticks adjacent to this philosopher. 
  // If the philosopher has an even ID, the lower-numbered stick is first
  // If the philosopher has an odd ID, the higher-numbered stick is first
  int stick1, stick2; 
  stick1 = ((*phil_number%2)==0)?(*phil_number):((*phil_number+1) % NUM_OF_PHILOSOPHERS);
  stick2 = ((*phil_number%2)!=0)?(*phil_number):((*phil_number+1) % NUM_OF_PHILOSOPHERS);
  
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
    
    
    // ********** ACQUIRE CHOPSTICKS ***************
    if (verbose) printf("Philosopher %d locking chopstick %d\n", *phil_number, stick1);
    sem_wait(&sem_vars[stick1]);
    if (verbose) printf("Philosopher %d locking chopstick %d\n", *phil_number, stick2);
    sem_wait(&sem_vars[stick2]);
    if (verbose) printf("Philosopher %d got both locks\n", *phil_number);
    // ***********************************************
   
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

