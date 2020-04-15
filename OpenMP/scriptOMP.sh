#!/bin/bash
echo "Compilando archivo fuente"
gcc -o ompsec2 -fopenmp secuencialOMP.c 
for i in 10 100 200 500 800 
do
for j in 1 2 3 4 5 
do
echo Ejecutando con matrices de $i X $i
./ompsec2 $i 
done
done