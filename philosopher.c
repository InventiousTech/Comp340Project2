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

void *philosopher_loop(void *param)
{
	int help_time;

	/* seed random generator */
	srandom((unsigned)time(NULL));

	while (1) {

	  sem_wait(&ta_sem);
	  while(waiting_students > 0){
	    sem_post(&students_sem);

			/* Help student */
			help_time = (int)((random() % MAX_SLEEP_TIME) + 1);
	    help_student(help_time);
			
			/* Notify student they are finished being helped */
			sem_post(&help_sem[student_number]);
	  }
	}
}

