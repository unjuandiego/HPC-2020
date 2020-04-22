#!/bin/bash
echo "Compilando.. "
gcc -o hilos hilos.c -pthread
for j in 1000 2000
do
for i in 1 2 3 4 5 6
do
./hilos $j
done
done
