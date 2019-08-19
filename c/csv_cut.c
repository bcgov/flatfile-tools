// 20190302 cut.cpp: cut out any line of the csv where FIELD also appears in FIELD for the cohort (selection) file
// this script needs to be deprecated
#include"misc.h"

#define HEAD_ONLY false //true //false //true
#define HEAD_N 100

int main(int argc, char ** argv){

  int i;
  init();
  if(argc < 4) err("cut.cpp: usage:\n\tcut.cpp [name of cohort/select file] [FIELD_NAME] [name of file to cut] .. [name of last file to cut]\n");

  for0(i, argc) printf("\targ %d: %s\n", i, argv[i]);

  char * select_fn = argv[1];
  char * field_name = argv[2];

  printf("file to select from: %s\n", select_fn);
  FILE * select_f = open(select_fn);
  char * h_line = getline(select_f, true); // make this a persistent memory until exit
  printf("head: %s", h_line);
  close(select_f);

  unsigned int h_fi = 0;
  unsigned int h_nf = 0;
  char ** h_f = split(h_line, strlen(h_line), h_nf);
  for0(i, h_nf) if(strcmp(h_f[i], field_name) == 0) h_fi = i;

  printf("SELECTED FIELD INDEX: %d\n", h_fi);
  split_print(h_f, h_nf);
  printf("\n");
  free_split(h_f, h_nf);

  char * d = fread(select_fn); // read the file that represents our cohort selection

  void * root = NULL;

  if(true){
    size_t fs = fsize(select_fn);
    // in-memory getline
    size_t rpos = 0; // beginning of current line
    size_t tpos = 0; // temporary read position
    register unsigned int j;

    size_t p_last = 0;

    printf("\tcount newline..\n");

    long unsigned int n_lin = 0;
    for0(tpos, fs) if(d[tpos] == '\n') n_lin ++;
    tpos = 0;

    printf("\tscanning...\n");
    long unsigned li = 0;

    // for each line
    while(true){
      p_last = tpos = rpos; // remember where we started; start at beginning of present line

      while(d[tpos] != '\n' && tpos < fs) tpos ++; // find the end of the present line

      register size_t len = tpos - rpos; // length of line
      char * line = (char *) alloc_tmp(len + 1);

      /* make a separate copy of the line */
      register size_t cpos = rpos;
      for0(j, len) line[j] = d[cpos ++];
      line[len] = '\0';
      if(li > 0){
        // process line
        char ** words = split(line, len, h_nf);
        if(li < 5 && words != NULL){
          printf("%s --> ", line);
          split_print(words, h_nf);
        }
        if(words != NULL){
          unsigned int m = 0;
          unsigned int sl = strlen(words[h_fi]);
          // add to search tree
          str_map * x = (str_map *)alloc_tmp(sizeof(str_map));
          x->key = (char *) alloc_tmp(sizeof(char) * (sl + 1));
          for(m = 0; m < sl + 1; m ++) x->key[m] = words[h_fi][m];
          x->key[ sl ] = '\0';
          trim(x->key);
          tsearch(x, &root, compare); // insert
          free_split(words, h_nf);
        }
      }

      if(tpos < fs) tpos ++; // we're either on a newline or "end of file"
      rpos = tpos; // advance to next line if we can

      if(p_last == rpos){
        free(line);
        break; // end of file, break out of loop
      }
      free(line); // release line memory

      /* debug purpose: minimal data */
      if(HEAD_ONLY && li >= HEAD_N) break;

      /* progress indicator */
      if(li++ % 10000 ==0) printf("\t\t%.2f%% line %lu of %lu\n", 100. * (float)(li+1)/(float)(n_lin), li, n_lin);
    }
  }

  printf("attempt walk..\n");
  unsigned int c_fi = 0; // field index for matching, in the files to cut

  /* count fields in header for files to cut; assert that number of fields matches */
  unsigned int n_cf = 0;
  printf("files to cut:\n");
  for(i = 3; i < argc; i++){
    printf("\t%s ", argv[i]);
    exists(argv[i]);

    FILE * cut_f = open(argv[i]);
    char * h_line = getline(cut_f, true); // make this a persistent memory until exit
    printf("head: %s", h_line);
    close(cut_f);

    unsigned int h_nf = 0;
    char ** h_f = split(h_line, strlen(h_line), h_nf);
    int j;
    for0(j, h_nf){
      if(strcmp(h_f[j], field_name) == 0){
        if(c_fi ==0) c_fi = j;
        else if(c_fi != j) err("field index mismatch. check first line of each data file");
      }
    }

    split_print(h_f, h_nf);
    free_split(h_f, h_nf);
    printf("\n");
    if(n_cf != 0) if(n_cf != h_nf) err("header mismatch: number of fields");
    n_cf = h_nf;

    printf("SELECTED FIELD INDEX %d\n", c_fi);
  }

  // at this point, found the extract field for all files, and it matched
  for(i = 3; i < argc; i++){
    char * fn = argv[i];
    size_t fs = fsize(fn);

    FILE * of_keep = fopen(strcat(strdup(argv[i]), "_cut.csv"), "wb");
    if(!of_keep){
      printf("Error: failed to open: %s\n", strcat(strdup(argv[i]), "_cut.csv"));
      err("Error.");
    }

    FILE * of_toss = fopen(strcat(strdup(argv[i]), "_dmp.csv"), "wb");
    if(!of_toss){
      printf("Error: failed to open: %s\n", strcat(strdup(argv[i]), "_dmp.csv"));
      err("Error.");
    }

    printf("%s fread %lu ", fn, (long unsigned int)fs);
    time_t start; time(&start);
    char * d = fread(fn);

    time_t end; time(&end);
    float mbs = (float)fs / (1000000. * (float(end - start)));
    printf("\ttime: %d s rate: %f Mb/s\n", int(end-start), mbs);

    // in-memory getline
    size_t rpos = 0; // beginning of current line
    size_t tpos = 0; // temporary read position
    register unsigned int j;

    size_t p_last = 0;

    printf("\tcount newline..\n");

    long unsigned int n_lin = 0;
    for0(tpos, fs) if(d[tpos] == '\n') n_lin ++;
    tpos = 0;

    printf("\tscanning...\n");
    long unsigned li = 0;

    // for each line
    while(true){
      p_last = tpos = rpos; // remember where we started; start at beginning of present line

      while(d[tpos] != '\n' && tpos < fs) tpos ++; // find the end of the present line

      register size_t len = tpos - rpos; // length of line
      char * line = (char *) alloc_tmp(len + 1);

      /* make a separate copy of the line */
      register size_t cpos = rpos;
      for0(j, len) line[j] = d[cpos ++];
      line[len] = '\0';
      if(li > 0){
        // process line
        char ** words = split(line, len, n_cf);
        if(li < 5){
          printf("%s --> ", line);
          split_print(words, n_cf);
        }
        if(words != NULL){
          // search

          if(false){
            str_map * x = (str_map *)alloc_tmp(sizeof(str_map));
            x->key = words[c_fi];
            void * r = tfind(x, &root, compare); // insert
            if(r){
              // match
              fprintf(of_keep, "%s\n", line);
            }
            else{
              // nomatch
              fprintf(of_toss, "%s\n", line);
            }
            free(x);
          }
          else{
            str_map x;
            x.key = words[c_fi];
            void * r = tfind(&x, &root, compare); // insert
            if(r){
              // match
              fprintf(of_keep, "%s\n", line);
            }
          }
          free_split(words, n_cf);
        }
      }

      if(tpos < fs) tpos ++; // we're either on a newline or "end of file"
      rpos = tpos; // advance to next line if we can

      if(p_last == rpos){
        free(line);
        break; // end of file, break out of loop
      }

      free(line); // release line memory

      /* debug purposes: minimal data */
      if(HEAD_ONLY && li >= HEAD_N) break;

      /* progress indicator */
      if(li++ % 100000 ==0) printf("\t\t%.2f%% line %lu of %lu\n", 100. * (float)(li+1)/(float)(n_lin), li, n_lin);

    }
    printf("\t\tdealloc");
    free(d); // release the file buffer

    fclose(of_keep);
    fclose(of_toss);
  }

  return quit(); // don't forget to free any tracked memory locations
}
