#!/bin/bash

# Scrivere uno script bash che prenda in input due pathname, e che concateni il contenuto
# del primo file al secondo file. Se il secondo file non esiste, lo script deve crearlo.

FILE_1="1.txt"
FILE_2="2.txt"
FILE_OUT="all.txt"

if [ -f $FILE_1 ] && [ -f $FILE_2 ]; then
    cat $FILE_1 $FILE_2 > $FILE_OUT
    echo "File concatenato con successo"
elif [ -f $FILE_1 ] && [ ! -f $FILE_2 ]; then
    cat $FILE_1 > $FILE_OUT
    echo "File concatenato con successo"
else
    echo "Errore: uno dei due file non esiste"
fi