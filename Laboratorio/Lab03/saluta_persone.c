#include <stdio.h>
#include <stdlib.h>


int main(int argc, char const *argv[]) {
    for(int i = 1; i < argc; i++) {
        printf("Ciao %s!\n", argv[i]);
    }
}