#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ceaser_decrypt(char* string, int k){
    for(int i = 0; i < strlen(string); i++){
        string[i] = string[i] - k;
    }
}

int main(){
char encoded[] = "Kn\"eqtuq\"hqtpkueg\"wp)kpvtqfw|kqpg\"cn\"nkpiwciikq\"fk\"rtqitcooc|kqpg\"E\"g\"cnnc\"rtqitcooc|kqpg\"korgtcvkxc0\"Wp\"pwogtq\"eqpukuvgpvg\"fk\"qtg\"gb\"wvknk||cvq\"rgt\"uxqnigtg\"gugtekvc|kqpk\"kp\"ncdqtcvqtkq\"hkpcnk||cvg\"c\"rtgpfgtg\"eqphkfgp|c\"eqp\"kn\"nkpiwciikq\"vtcokvg\"nc\"tgcnk||c|kqpg\"fk\"rtqitcook\"kp\"nkpiwciikq\"E0";         
    ceaser_decrypt(encoded, 2);
    printf("Decoded: %s\n", encoded);
}