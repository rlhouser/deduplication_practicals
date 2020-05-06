#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "cacheutils.h"

#define  infile "infile.txt"
#define  num_seconds 240

#define ROWS 1024
#define COLUMNS 4096

unsigned int  map0[ROWS][COLUMNS];

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
  size_t times[ROWS];  
  printf("Initializing\n");
  for (int i = 0; i < ROWS; i++){
    for (int j = 0; j < COLUMNS; j++){
      map0[i][j] = i*COLUMNS + j;
    }
  }
  printf("Waiting for deduplication\n");
  time_t start = time(NULL);
  time_t elapsed = time(NULL) - start;
  while(elapsed < num_seconds){
        printf("%zu\n", elapsed);
        sleep(30);
        elapsed = time(NULL) - start;
  }
  for (int i = ROWS; i >0; --i){
    times[i] = write_file(map0[i], 0);
  }
  FILE *fprt_o = fopen("calibration.csv", "w");
  for (int i = ROWS; i >0; --i){
	fprintf(fprt_o, "%zu\n",  times[i]);
  }
  fclose(fprt_o);
  return 1;
}

