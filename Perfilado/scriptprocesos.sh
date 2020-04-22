#!/bin/bash
echo "Compilando.. "
gcc -o2 procesos procesos.c
for j in 10 100 200 500 800 1000 2000
do
for i in 1 2 3 4 5 6
do
./procesos $j 
done
done