#!/bin/bash
# Scrivere uno script bash che prenda in input un pathname, e che verifichi se si 
# tratta di una directory o di un file. Nel primo caso lo script deve elencare i file 
# contenuti nella directory, mentre nel secondo deve visualizzare il contenuto del 
# file.

if [ -d $1 ]; then
    echo "Elenco dei file contenuti nella directory:"
    ls $1
elif [ -f $1 ]; then
    cat $1
else
    echo "Il file non esiste"
fi