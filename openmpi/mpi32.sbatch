#!/bin/bash
#SBATCH --mem-per-cpu=1G
#SBATCH --time=01:00:00 
#SBATCH --cpus-per-task=32
#SBATCH --output=openmpi.out
#SBATCH --error=openmpi.err 
/homes/loenen/cis520/CIS520Proj4/openmpi/openmpi
