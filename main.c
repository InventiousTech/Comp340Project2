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
  
  // Check to see if we got a filename for random numbers
  if (argc < 2){
    printf("ERROR: Missing Arguments. Usage is \"task1 <filename> [-v]\"\n\tThe -v option enables verbose output\n");
    return 1;
  }
  
  // Set verbosity
  if ((argc >2) && (strcmp(argv[2], "-v")==0)){
    verbose = 1;
  }
  
  init();
  
  // Load random numbers from file
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

  printf("[ time  ] Action");
  create_philosophers();
  
  for (i = 0; i < NUM_OF_PHILOSOPHERS; i++)
    pthread_join(philosophers[i], NULL);
  
  
	for (i=0; i<NUM_OF_PHILOSOPHERS; i++)
		sem_destroy(&sem_vars[i]);
	
  return 0;
}
