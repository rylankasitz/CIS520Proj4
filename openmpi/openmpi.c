#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <omp.h>

#define NUM_OF_ENTRIES 1000000
#define NUM_OF_CHARACTERS 2100
#define WIKI_DATA_FILE "/homes/dan/625/wiki_dump.txt"
#define UTILIZATION_FILE "/homes/loenen/cis520/CIS520Proj4/openmpi/utilization_statistics.txt"

char wiki_characters[NUM_OF_ENTRIES][NUM_OF_CHARACTERS];
int line_counts[NUM_OF_ENTRIES];

int num_threads = 32;

//Initialize both arrays 
void init_arrays()
{
  int i, j;

  for(i = 0; i < NUM_OF_ENTRIES; i++)
  {
    for(j= 0; j < NUM_OF_CHARACTERS; j++)
    {
      wiki_characters[i][j] = 0; 
    }
  }

  for (i = 0; i < NUM_OF_ENTRIES; i++ ) 
	{
		line_counts[i] = 0;
	}
}

//Read wiki data into memory
int read_to_memory()
{ 
    FILE *file = fopen(WIKI_DATA_FILE, "r");

    if(file == NULL)
    {
        printf("fail");
        return -1;
    }

    /* Read each wiki line into memory. */
    int line_num = 0;
    char line[NUM_OF_CHARACTERS];
    while(fgets(line, NUM_OF_CHARACTERS, file) != NULL)
    {
        strcpy(wiki_characters[line_num], line);
        line_num++;
    }
    fclose(file);
}

void *count_array()
{
	int myID;

  omp_set_num_threads(num_threads);

  #pragma omp parallel private(myID)
  {
    int i, j, startPos, endPos;
    int local_line_count[NUM_OF_ENTRIES];

    myID = omp_get_thread_num();
    startPos = (myID) * (NUM_OF_ENTRIES / num_threads);
    endPos = startPos + (NUM_OF_ENTRIES / num_threads);

    if (myID == num_threads - 1)
      endPos = NUM_OF_ENTRIES - 1;

    // init local count array
    for (i = 0; i < NUM_OF_ENTRIES; i++ ) 
    {
      local_line_count[i] = 0;
    }

    // count up our section of the global array
    for (i = startPos; i < endPos; i++) 
    {
      for (j = 0; j < NUM_OF_CHARACTERS; j++ ) 
      {
        local_line_count[i] += (int) wiki_characters[i][j];
      }
    }

    for (i = startPos; i < endPos; i++ ) 
    {
      line_counts[i] = local_line_count[i];
    }
  }
}

void print_results()
{
  int i;
	for (i = 0; i < NUM_OF_ENTRIES-1; i++) {
		printf("%d-%d: %d\n", i, i+1, line_counts[i]-line_counts[i+1]);
	}
}

void print_times(struct timeval t1, struct timeval t2, struct timeval t3, struct timeval t4, struct timeval t5, char* file)
{
	double elapsedTime;
  
	FILE * fp = fopen (file,"a");

  // Threaded time
	elapsedTime = (t4.tv_sec - t3.tv_sec) * 1000.0;
	elapsedTime += (t4.tv_usec - t3.tv_usec) / 1000.0;
  fprintf(fp, "%f,", elapsedTime);

  // Total time
	elapsedTime = (t5.tv_sec - t1.tv_sec) * 1000.0;
	elapsedTime += (t5.tv_usec - t1.tv_usec) / 1000.0;
  fprintf(fp, "%f,", elapsedTime);

	fclose(fp);
}

main(int argc, char *argv[])
{
	int i, rc;
	void *status;
	struct timeval t1, t2, t3, t4, t5;
	double elapsedTime;

  num_threads = atoi(argv[1]);

	gettimeofday(&t1, NULL);
	init_arrays();
	gettimeofday(&t2, NULL);

	if (read_to_memory() == 0) 
	{
		gettimeofday(&t3, NULL);

		count_array();

    gettimeofday(&t4, NULL);

    print_results();
	}
	else 
	{
		return -1;
	}

	gettimeofday(&t5, NULL);

	print_times(t1, t2, t3, t4, t5, argv[2]);

	printf("Main: program completed. Exiting.\n");
}
