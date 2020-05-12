#!/bin/bash

FILE_LOCATION="/homes/rylankasitz/cis520/Proj4/pthread"
OUTPUT_LOCATION=$FILE_LOCATION/"output_files"
RUNTIMES_FILE=$FILE_LOCATION/"runtimes.csv"
THREADS=( 20 18 16 14 12 10 8 6 4 2 1 )
ITERATIONS=$1

cd $FILE_LOCATION
make

mkdir -p $OUTPUT_LOCATION
rm $OUTPUT_LOCATION/*
rm $RUNTIMES_FILE
touch $RUNTIMES_FILE

i=0
line="Core Count,"
while [ $i -lt $ITERATIONS ]
do
    line+="Run $i Threaded Sections,Run $i Total,"
    let "i++"
done

echo "$line,Average Threaded Section,Average Total" >> $RUNTIMES_FILE

for t in ${THREADS[@]}
do
    echo "Running for $t threads"
    sbatch --cpus-per-task=$t --output=$OUTPUT_LOCATION/$t"_threads.output" $FILE_LOCATION/pthreads_solution.sh $ITERATIONS $RUNTIMES_FILE
done

echo "Waiting for iterations to finish"
touch $FILE_LOCATION/temp.csv 
while [ $(find $FILE_LOCATION/*temp.csv -type f | wc -l) -le ${#THREADS[@]} ]
do
    sleep 1
done

cat $FILE_LOCATION/*temp.csv >> $RUNTIMES_FILE 
rm $FILE_LOCATION/*temp.csv