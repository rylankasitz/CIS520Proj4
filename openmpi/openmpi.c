#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

#define WIKI_ARRAY_SIZE 50 /* The number of wiki entries. Should be 1000000 for production. */
#define WIKI_STRING_SIZE 2003 /* The number of characters in each wiki entry. Must account for newline and terminating characters. */

/* All of the wiki entries. */
char wiki_array[WIKI_ARRAY_SIZE][WIKI_STRING_SIZE];


int num_threads = 8;

/* Initialize the results array to all zero */
void init_array()
{
  int i, j;

  for(i = 0; i < WIKI_ARRAY_SIZE; i++)
  {
    for(j= 0; j < WIKI_STRING_SIZE; j++)
    {
      results_array[i][j] = 'f';
    }
  }
}

/* Read all of the wiki entries and words into local data structures from thier resprctive files. */
int read_to_memory()
{
    /* Read the wiki article into memory line by line. */
    FILE *file = fopen("/homes/dan/625/wiki_dump.txt", "r");

    if(file == NULL)
    {
        printf("fail");
        return -1;
    }

    /* Read each wiki line into memory. */
    int line_num = 0;
    char line[WIKI_STRING_SIZE];
    while(fgets(line, WIKI_STRING_SIZE, file) != NULL)
    {
        strcpy(wiki_array[line_num], line);
        line_num++;
    }
    fclose(file);
}