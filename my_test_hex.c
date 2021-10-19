#include <stdio.h>
#include <stdlib.h>

int main(){
    int Hex = 15;
    printf("------hex----%x, --------dec----%d\n", Hex, Hex);
    if(Hex == 0xf){
        printf("15 == 0xf \n");

    }else{
        printf("15 != 0xf \n");
    }

    return 0;
}