#!/bin/bash

FILE_LOCATION="/homes/rylankasitz/cis520/Proj4/pthread"
OUTPUT_LOCATION=$FILE_LOCATION/"output_files"

cd $FILE_LOCATION
make

mkdir -p $OUTPUT_LOCATION
rm $OUTPUT_LOCATION/*
rm $FILE_LOCATION/stats.txt

THREADS=( 32 16 8 4 2 1 )

for t in ${THREADS[@]}
do
    echo "Running for $t threads"
    sbatch --cpus-per-task=$t --output=$OUTPUT_LOCATION/$t"_threads.output" $FILE_LOCATION/pthreads_solution.sh
done