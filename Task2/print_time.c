/*
 * Author:    Isaac Hendrickson
 * Date:      April 14th, 2018
 * Purpose:   This file calculates and prints the current time
 */

#include "dp.h"
#include <stdio.h>


void print_time(){
  clock_gettime(CLOCK_MONOTONIC, &end);
  elapsed_time = ((double)end.tv_sec + (double)end.tv_nsec/1000000000.0) - ((double)begin.tv_sec + (double)begin.tv_nsec/1000000000.0);
  printf("[%5.1fs ] ", elapsed_time);
}
