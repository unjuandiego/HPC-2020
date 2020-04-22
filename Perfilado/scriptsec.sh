#!/bin/bash
echo "Compilando archivo fuente"
gcc -o secuencial secuencial2.c
for i in 1000 2000
do
for j in 1 2 3 4 
do
echo Ejecutando con matrices de $i X $i
./secuencial $i 
done
done