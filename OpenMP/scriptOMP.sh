#!/bin/bash
echo "Compilando archivo fuente"
gcc -o ompsec2 -fopenmp secuencialOMP.c 
for i in 10 100 200 500 800 1000 2000
do
for j in 1 2 3 4 5 6 7 8 9 10
do
echo Ejecutando con matrices de $i X $i
./ompsec2 $i 
done
done
