#!/bin/bash
# This is a comment!

for sp in 10 100 1000 10000 100000 1000000
do
    for n in 10 100 500 1000 2000 5000 10000
    do
        for j in 1 2 3 4 5 6 7 8 9 10
        do
            ./test7 $sp $n tiemposparalelo.txt
        done
    done
done
