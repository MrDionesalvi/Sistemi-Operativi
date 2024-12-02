#!/bin/bash

# Scrivere uno script bash che prenda in input un pathname, e che calcoli la media dell'altezza

FILE="anagrafica.csv"
sum=0

if [ -f $FILE ]; then
    awk -F, '{sum+=$4} END {print "Media altezza: " sum/NR}' $FILE
else
    echo "Il file non esiste"
fi