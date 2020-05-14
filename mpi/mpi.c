#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <sys/time.h>

#define NUM_OF_ENTRIES 1000000
#define NUM_OF_CHARACTERS 2100
#define WIKI_DATA_FILE "/homes/dan/625/wiki_dump.txt"

char wiki_characters[NUM_OF_ENTRIES][NUM_OF_CHARACTERS];
int lines[NUM_OF_ENTRIES];
int thread_count = 32;

void init_arrays() {
    int i, j;

    for(i = 0; i < NUM_OF_ENTRIES; i++) {
        for(j= 0; j < NUM_OF_CHARACTERS; j++) {
            wiki_characters[i][j] = 0; 
        }
    }
    
    for (i = 0; i < NUM_OF_ENTRIES; i++ ) {
        lines[i] = 0;
	}
}

int read_into_memory()
{
    FILE * file = fopen(WIKI_DATA_FILE, "r");
	
    if (file == NULL) {
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
    return 0;
}

void print_results()
{
	for (int i = 0; i < NUM_OF_ENTRIES-1; i++) {
		printf("%d-%d: %d\n", i, i+1, lines[i]-lines[i+1]);
	}
}

void print_times(struct timeval t1, struct timeval t2, struct timeval t3, struct timeval t4, struct timeval t5, char* file)
{
	double elapsedTime;
  
	FILE * fp = fopen (file,"a");

    elapsedTime = (t4.tv_sec - t3.tv_sec) * 1000.0;
    elapsedTime += (t4.tv_usec - t3.tv_usec) / 1000.0;
    fprintf(fp, "%f,", elapsedTime);

    elapsedTime = (t5.tv_sec - t1.tv_sec) * 1000.0;
    elapsedTime += (t5.tv_usec - t1.tv_usec) / 1000.0;
    fprintf(fp, "%f,", elapsedTime);

	fclose(fp);
}

void * count_array(int * rank) {
    int local_lines[NUM_OF_ENTRIES];
    int i, j;
    int id = *rank;

    int start = id * (NUM_OF_ENTRIES / thread_count);
	int end = start + (NUM_OF_ENTRIES / thread_count);

    if (id == thread_count - 1) {
        end = NUM_OF_ENTRIES - 1;
    }
    
    for (i = 0; i < NUM_OF_ENTRIES; i++ ) {
        local_lines[i] = 0;
    }
    
    for (i = start; i < end; i++) {
        for (j = 0; j < NUM_OF_CHARACTERS; j++ ) {
            local_lines[i] += (int) wiki_characters[i][j];
        }
    }

    for (i = start; i < end; i++ ) {
        lines[i] = local_lines[i];
    }
}

int main(int argc, char* argv[])
{  
    struct timeval t1, t2, t3, t4, t5;
    double elapsedTime;
    int size, rank;
    MPI_Status status;

	gettimeofday(&t1, NULL);

	// serial code begins
    init_arrays();
	// serial code ends

    gettimeofday(&t2, NULL);
    
    // parallel code begins
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    thread_count = size;

    if(rank == 0) {
        if(read_into_memory() == -1) {
            printf("FILE NOT FOUND");
            return -1;
        }
		gettimeofday(&t3, NULL);
    }
    

    /*
        Inspired by
        https://github.com/bnweddle/CIS520-Project4
    */
    // parallel code begins
    MPI_Bcast(lines, NUM_OF_ENTRIES, MPI_INT, 0, MPI_COMM_WORLD);
    count_array(&rank);
	MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    // parallel code ends
    
    gettimeofday(&t4, NULL);
    
    print_results();
    printf("Main: program completed. Exiting.\n");

	gettimeofday(&t5, NULL);
    
    print_times(t1, t2, t3, t4, t5, argv[2]);
	
    return 0;
}