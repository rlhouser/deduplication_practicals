#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "../cacheutils.h"

#define  num_seconds 240

#define pagesize 4096
#define filesize 3667682
#define pages 896

char map[pages*pagesize];

size_t write_file(char* buf){

        maccess(buf);
        size_t t = rdtsc();
        memwrite(buf);
        sched_yield();
        size_t delta = rdtsc() - t;
        return delta;
}

int main(int argc, char** argv)
{
  if (argc != 3){
  	exit(!fprintf(stderr,"  usage: ./check_file_corruption <offset> <reference file> \n"
	"example: ./check_file_corruption 0 vmlinux\n"));
  }
  char infile[100];
  size_t offset = 0;
  if (!sscanf(argv[1],"%li",&offset))
    exit(!printf("offset error\n"));
  if (!sscanf(argv[2],"%s",infile))
    exit(!fprintf(stderr,"filename error\n"));
  size_t times[pages];  
  printf("Initializing\n");
  FILE *fptr_i = fopen(infile, "rb");
//  char *tmp = fgets(&map[offset], filesize, fptr_i); 
  int t;
  for (int i = offset; i < filesize + offset; i++){
	t = fread(&map[i], 1, 1, fptr_i);
  }
  //  for (int i = filesize + offset - 15; i < filesize + offset; i++){
  //      printf("%c", map[i]);
  // }
  fclose(fptr_i);
  time_t start = time(NULL);
  time_t elapsed = time(NULL) - start;
  while(elapsed < num_seconds){
        printf("%zu\n", elapsed);
        sleep(30);
        elapsed = time(NULL) - start;
  }
  for (int i = pages-1; i >=0; i--){
    times[i] = write_file(&map[i*4096] + offset);
  }
  FILE *fprt_o = fopen("check_corruption.csv", "w");
  for (int i = pages; i >0; --i){
	fprintf(fprt_o, "%zu\n",  times[i]);
  }
  fclose(fprt_o);
  return 1;
}

