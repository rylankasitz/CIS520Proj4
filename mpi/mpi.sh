#!/bin/bash
#SBATCH --mem=10G
#SBATCH --time=01:00:00 
#SBATCH --constraint=elves

dir=$(dirname $2)
file=$dir/$3".csv"
touch $file

i=0
while [ $i -lt $1 ]
do
    mpirun -np $3 $dir/mpi.exe $3 $file
    let "i++"
done

fcontents=$(cat $file)
rm $file
echo "$3,$fcontents" >> $dir/$3"temp.csv"
