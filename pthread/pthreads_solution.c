#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define NUM_OF_ENTRIES 1000000
#define NUM_OF_CHARACTERS 2100

#define WIKI_DATA_FILE "/homes/dan/625/wiki_dump.txt"

pthread_mutex_t mutexsum;	// mutex for line_counts

char wiki_characters[NUM_OF_ENTRIES][NUM_OF_CHARACTERS];
int line_counts[NUM_OF_ENTRIES];

int threads_count = 32;

void init_arrays()
{
	int i, j; 

	pthread_mutex_init(&mutexsum, NULL);

	for (i = 0; i < NUM_OF_ENTRIES; i++) 
	{
		for (j = 0; j < NUM_OF_CHARACTERS; j++ ) 
		{
			wiki_characters[i][j] = 0;
		}
	}

	for (i = 0; i < NUM_OF_ENTRIES; i++ ) 
	{
		line_counts[i] = 0;
	}
}

int read_into_memory()
{
	FILE *file = fopen(WIKI_DATA_FILE, "r");

	if(file == NULL)
	{
		printf("fail");
		return -1;
	}

	int line_num = 0;
  	char line[NUM_OF_CHARACTERS];

	while(fgets(line, NUM_OF_CHARACTERS, file) != NULL)
	{
		strcpy(wiki_characters[line_num], line);
		line_num++;
	}

	fclose(file);
}

void *count_array(void *myID)
{
	int i, j;
	int local_line_count[NUM_OF_ENTRIES];

	int startPos = ((int) myID) * (NUM_OF_ENTRIES / threads_count);
	int endPos = startPos + (NUM_OF_ENTRIES / threads_count);

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

	pthread_mutex_lock (&mutexsum);

	for (i = startPos; i < endPos; i++ ) 
	{
		line_counts[i] = local_line_count[i];
	}

	pthread_mutex_unlock (&mutexsum);
	pthread_exit(NULL);
}

void print_results()
{
	for (int i = 0; i < NUM_OF_ENTRIES-1; i++) {
		printf("%d-%d: %d\n", i, i+1, line_counts[i]-line_counts[i+1]);
	}
}

void print_times(struct timeval t1, struct timeval t2, struct timeval t3, struct timeval t4, struct timeval t5, struct timeval t6, char* file)
{
	double elapsedTime;

	FILE * fp = fopen (file, "a");

	// Threaded section
	elapsedTime = (t4.tv_sec - t3.tv_sec) * 1000.0;
	elapsedTime += (t4.tv_usec - t3.tv_usec) / 1000.0;
	fprintf(fp, "%f,", elapsedTime);

	// Total time
	elapsedTime = (t6.tv_sec - t1.tv_sec) * 1000.0;
	elapsedTime += (t6.tv_usec - t1.tv_usec) / 1000.0;
	fprintf(fp, "%f,", elapsedTime);

	fclose(fp);
}

main(int argc, char *argv[]) 
{
	threads_count = atoi(argv[1]);

	int i, rc;
	pthread_t threads[threads_count];
	pthread_attr_t attr;
	void *status;

	struct timeval t1, t2, t3, t4, t5, t6;
	double elapsedTime;
	
	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	gettimeofday(&t1, NULL);
	init_arrays();
	gettimeofday(&t2, NULL);

	if (read_into_memory() == 0) 
	{
		gettimeofday(&t3, NULL);

		for (i = 0; i < threads_count; i++ ) 
		{
			rc = pthread_create(&threads[i], &attr, count_array, (void *)i);
			if (rc) 
			{
				printf("ERROR; return code from pthread_create() is %d\n", rc);
				exit(-1);
			}
		}

		/* Free attribute and wait for the other threads */
		pthread_attr_destroy(&attr);
		for(i=0; i < threads_count; i++) 
		{
			rc = pthread_join(threads[i], &status);
			if (rc) 
			{
				printf("ERROR; return code from pthread_join() is %d\n", rc);
				exit(-1);
			}
		}

		gettimeofday(&t4, NULL);
		print_results();
		gettimeofday(&t5, NULL);
	}
	else 
	{
		return -1;
	}

	gettimeofday(&t6, NULL);

	print_times(t1, t2, t3, t4, t5, t6, argv[2]);

	pthread_mutex_destroy(&mutexsum);
	pthread_exit(NULL);
}

