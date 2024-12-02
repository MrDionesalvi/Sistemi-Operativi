#!/bin/bash

echo "Rimuovo code di messaggi IPC..."
for i in $(ipcs -q | grep 0x | awk '{print $2}'); do
    echo "Rimuovo coda di messaggi con ID $i"
    ipcrm -q $i
done

echo "Rimuovo segmenti di memoria condivisa IPC..."
for i in $(ipcs -m | grep 0x | awk '{print $2}'); do
    echo "Rimuovo segmento di memoria condivisa con ID $i"
    ipcrm -m $i
done

echo "Rimuovo semafori IPC..."
for i in $(ipcs -s | grep 0x | awk '{print $2}'); do
    echo "Rimuovo semaforo con ID $i"
    ipcrm -s $i
done

echo "Pulizia completata!"