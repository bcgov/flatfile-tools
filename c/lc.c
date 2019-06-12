/* count lines in a file in C; an illustrative example since
 can do this in Linux with
     wc -l  */
#include"misc.h"
#define MAXCHAR 1024*1024

int main(int argc, char ** argv){
  if(argc < 2){
	  printf("lc.c: row count for file\n\tUsage: lc [filename]\n");
	  exit(1);
  }
  FILE *fp;
  char str[MAXCHAR];
  char* filename = argv[1];
  fp = fopen(filename, "r");

  if (fp == NULL){
    printf("Error: could not open file: %s\n", filename);
    return 1;
  }

  long int lc = 0;
  while(fgets(str, MAXCHAR, fp) != NULL) lc ++;
  fclose(fp);

  printf("lines %ld\n", lc);
  return 0;
}