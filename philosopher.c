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
    
    //**************************
    // TODO: Real Hungry loop
    //**************************
    state[*phil_number] = HUNGRY;
    sleep_time = get_next_number();
    printf("Philosopher %d Hungry for %d seconds...\n", *phil_number, sleep_time);
    sleep(sleep_time);
    
    
    // *************************
    // TODO: Eating
    // *************************
    state[*phil_number] = EATING;
    sleep_time = get_next_number();
    printf("Philosopher %d Eating for %d seconds...\n", *phil_number, sleep_time);
    sleep(sleep_time);
  }
}

