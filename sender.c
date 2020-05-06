#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "cacheutils.h"

#define  infile "infile.txt"
#define  num_seconds 600

#define ROWS 1024
#define COLUMNS 1024
#define DEBUG 1
#define MESSAGE_LEN 15

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

  if (argc != 4)
    exit(!fprintf(stderr,"  usage: ./sender <offset> <seed> <infile>\n"
                 "example: ./sender 0 31\n"));
  
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
  char filename[100];
  if (!sscanf(argv[3],"%s",filename))
    exit(!fprintf(stderr,"filename error\n"));

  char buff[MESSAGE_LEN];
  for (size_t i = 0; i < MESSAGE_LEN; i++){
 	buff[i] = '0';
  }
  FILE *fp = fopen(filename, "r");
  char *tmp = fgets(buff, MESSAGE_LEN, fp);
  fclose(fp);
  char encoded[MESSAGE_LEN*8];
  for(size_t i = 0; i < MESSAGE_LEN; i++){
    //printf("%c\t", buff[i]);
    for (size_t j = 0; j < 8; j++){
      encoded[i*8 + j] = (buff[i] >> j) & 0x01;
    //  printf("%i", encoded[i*8 + j]); 
     }
//     printf("\n");
  }
  for (size_t i = 0; i < MESSAGE_LEN*8; i++){
    for (size_t j = 0; j < 8; j++){
      if (encoded[i] == 1){
        map[i*8 + j][offset] = map[i*8 + j][offset] + 1;
      }
    } 
  }
    if(DEBUG == 1){
    FILE *fprt_d = fopen("debug_sender.txt", "w");
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
  return 1;
}
