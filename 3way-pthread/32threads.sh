#!/bin/bash
#SBATCH --mem-per-cpu=1G
#SBATCH --time=01:00:00 
#SBATCH --cpus-per-task=32
#SBATCH --output=pthreads_solution.out
#SBATCH --error=pthreads_solution.err 
/homes/rylankasitz/cis520/Proj4/3way-pthread/pthreads_solution