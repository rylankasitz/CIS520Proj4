#!/bin/bash
#SBATCH --mem=10G
#SBATCH --time=01:00:00 
#SBATCH --constraint=elves

dir=$(dirname $2)
file=$dir/$SLURM_CPUS_ON_NODE".csv"
touch $file

i=0
while [ $i -lt $1 ]
do
    /homes/loenen/cis520/CIS520Proj4/openmpi/openmpi $SLURM_CPUS_ON_NODE $file
    let "i++"
done

fcontents=$(cat $file)
rm $file
echo "$SLURM_CPUS_ON_NODE,$fcontents" >> $dir/$SLURM_CPUS_ON_NODE"temp.csv"
