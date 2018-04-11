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
  int phil_number = (int *)param; // The ID of this philosopher

	/* seed random generator */
	srandom((unsigned)time(NULL));

  // Loop Thinking->Hungry->Eating 5 times
  for (int i=0; i<5; i++){
    sleep_time = (int)((random() % MAX_SLEEP_TIME) +1);
    printf("Philosopher %d Thinking for %d seconds...\n", phil_number, sleep_time);
    sleep(sleep_time);
    
    //**************************
    // TODO: Hungry loop
    //**************************
    
    // *************************
    // TODO: Eating
    // *************************
  }
}

