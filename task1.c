#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "dp.h"


pthread_t philosophers[NUM_OF_PHILOSOPHERS];

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
    if ( sem_init(&phil_sem[i], 0, 0) == -1)
      printf("error init philosopher semaphore\n");
  }

}

void create_philosophers()
{
  int i;
  
  for (i = 0; i < NUM_OF_PHILOSOPHERS; i++) {
    pthread_create(&philosophers[i], 0, philosopher_loop, (void *)&phil_id[i]);
  }
}

int main(int argc, char** argv)
{
	// First check to see if we got an argument
  if (argc < 2){
    printf("ERROR: Missing Arguments. Usage is \"task2 <filename>\"\n");
    return 1;
  }
  int i;

  // *****************************
  // TODO: Read file with random ints
  // *****************************
  
  init();
  
  create_philosophers();
  
  for (i = 0; i < NUM_OF_PHILOSOPHERS; i++)
    pthread_join(philosophers[i], NULL);
  
  
	for (i=0; i<NUM_OF_PHILOSOPHERS; i++)
		sem_destroy(&phil_sem[i]);
	
  return 0;
}

