#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "../cacheutils.h"

#define  infile "infile.txt"
#define  num_seconds 240

#define ROWS 1024
#define COLUMNS 1024
#define DEBUG 1

unsigned int  map[ROWS][COLUMNS];

size_t write_file(unsigned int* buf, int writepos){

        maccess(&buf[writepos]);
        size_t t = rdtsc();
        memwrite(&buf[writepos]);
        sched_yield();
        size_t delta = rdtsc() - t;
        return delta;
}


int main(int argc, char** argv)
{
  if (argc != 3)
    exit(!fprintf(stderr,"  usage: ./receiver <offset> <seed>\n"
                 "example: ./receiver 0 31\n"));
  size_t offset = 0;
  if (!sscanf(argv[1],"%li",&offset))
    exit(!printf("offset error\n"));
  size_t seed = 0;
  if (!sscanf(argv[2],"%li",&seed))
    exit(!printf("seed error\n"));

  printf("Initializing\n");
  for (size_t j = offset; j < COLUMNS; j++){
      map[0][j] = seed++;
    }
  for (size_t i = 1; i < ROWS; i++){
    for (size_t j = 0; j < COLUMNS; j++){
      map[i][j] = seed++;
    }
  }
  if (DEBUG==1){
    FILE *fprt_d = fopen("debug_receiver.txt", "w");
	    for (size_t j = offset; j < COLUMNS; j++){
	      fprintf(fprt_d, "%p\t%i\n",  &map[0][j], map[0][j]);
	    }
	    for (size_t i = 1; i < ROWS; i++){
	      for (size_t j = 0; j < COLUMNS; j++){
	       fprintf(fprt_d, "%p\t%i\n",  &map[i][j], map[i][j]);
	    }
    }
  fclose(fprt_d);
  }

  printf("Waiting for deduplication\n");
  time_t start = time(NULL);
  time_t elapsed = time(NULL) - start;
  while(elapsed < num_seconds){
        printf("%zu\n", elapsed);
        sleep(30);
        elapsed = time(NULL) - start;
  }
  size_t times[ROWS]; 
  for (int i = ROWS; i >0; --i){
    times[i] = write_file(map[i], 0);
  }
  FILE *fprt_o = fopen("received.csv", "w");
  for (int i = ROWS; i >0; --i){
	fprintf(fprt_o, "%zu\n",  times[i]);
  }
  fclose(fprt_o);
  return 1;
}


