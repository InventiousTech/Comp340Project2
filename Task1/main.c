/*
 *  main.c
 *  Author:       Isaac Hendrickson and James Ahrens
 *  Date:         April 10th, 2018
 
 *  Purpose:      This file contains the main routine and some other miscellaneous subroutines for solving Project 2 in COMP 340.
 *                This program is our solution to the Dining Philosophers problem. N philosophers are created, each as its own thread
 *                using code found in the philosopher.c file. They each loop through the THINKING -> HUNGRY -> EATING cycle 5 times before
 *                the program exits.
 *
 *  Compile with: make
 *  Run with:     ./dining_philosophers
 * 
 *  Note:         This program uses two mutex locks. One to protect the random-number choosing, and one
 *                to ensure that the information printed to the console is legible (no interference from other philosophers).
 */

#include "dp.h"
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <time.h>



pthread_t philosophers[NUM_OF_PHILOSOPHERS];

void *philosopher_loop(void *param);

/**
 * Initialize all relevant data structures and
 * synchronization objects.
 */
void init()
{
  int i;
  
  if ( pthread_mutex_init(&mutex_rand, NULL) != 0)
    printf("%s\n",strerror(errno));
  
  if (pthread_mutex_init(&mutex_lock, NULL) != 0)
    printf("%s\n", strerror(errno));
     
  for (i = 0; i < NUM_OF_PHILOSOPHERS; i++){
    phil_id[i] = i;
    if ( sem_init(&sem_vars[i], 0, 1) == -1)
      printf("error init philosopher semaphore\n");
  }
  
  rand_position = 0;
  
}

void create_philosophers()
{
  int i;
  
  for (i = 0; i < NUM_OF_PHILOSOPHERS; i++) {
    pthread_create(&philosophers[i], 0, philosopher_loop, (void *)&phil_id[i]);
  }
}

int get_next_number()
{
  int temp;
  pthread_mutex_lock(&mutex_rand);
  temp = rand_numbers[rand_position];
  rand_position++;
  pthread_mutex_unlock(&mutex_rand);
  return temp;  
}

int main(int argc, char** argv)
{
  int i;
  verbose = 0;
  wait_index = 0;
  
  /* Check to see if we got a filename for random numbers */
  if (argc < 2){
    printf("ERROR: Missing Arguments. Usage is \"task1 <filename> [-v]\"\n\tThe -v option enables verbose output\n");
    return 1;
  }
  
  /* Set verbosity */
  if ((argc >2) && (strcmp(argv[2], "-v")==0)){
    verbose = 1;
  }
  
  init();
  
  /* Load random numbers from file */
  FILE *numberFile;
  numberFile = fopen(argv[1], "r");
  if (numberFile == NULL){
    printf("Invalid filename. Exiting.\n");
    exit(1);
  }
  for (i = 0; i < MAX_LENGTH; i++)
    fscanf(numberFile, "%d", &rand_numbers[i]);
  fclose(numberFile);

  /* Record start time so that each operation can print elapsed time */  
  clock_gettime(CLOCK_MONOTONIC, &begin);

  printf("[ time  ] Action\n");
  create_philosophers();
  
  for (i = 0; i < NUM_OF_PHILOSOPHERS; i++)
    pthread_join(philosophers[i], NULL);
  
  /* Calculate and display wait times */
  for (i = 0; i < wait_index; i++){
    average_wait_time += wait_time_array[i];
  }
  average_wait_time = average_wait_time/((double)wait_index);
  printf("Average wait time was: %.4f\n", average_wait_time);
  printf("Maximum wait time was: %.4f\n", max_wait_time);
  
  for (i=0; i<NUM_OF_PHILOSOPHERS; i++)
    sem_destroy(&sem_vars[i]);
  
  return 0;
}
