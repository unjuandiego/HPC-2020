#!/bin/bash
# This is a comment!

for sp in 10 100 1000 10000 100000 1000000
do
    for n in 10 100 1000 2000 5000 10000 20000
    do
        for j in 1 2 3 4 5 
        do
             ./omp $sp $n tiemposOMP.txt    
        done
    done
done
