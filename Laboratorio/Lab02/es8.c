#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int index = 1;
    for(int i = 1; i <= 10; i++){
        for(int j = i; j <= 10*i; j += i){
            printf("%6d", j);
        }
        printf("\n");
    }

}

