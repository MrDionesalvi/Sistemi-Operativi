#!/bin/bash

# Compile the code
gcc writer.c -o writer
gcc reader.c -o reader

# Run the writer
./writer < book.txt &

# Run the reader
./reader

# Clean the directory
rm writer reader

# Exit the script
exit 0