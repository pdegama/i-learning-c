//
// Created by parthka on 19/6/22.
//

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

struct threeNum
{
    int n1, n2, n3;
};

int main(int argc, char** args){

    FILE *fptr1;
    fptr1 = fopen("/home/parthka/Videos/Bhool Bhulaiyaa 2.mkv","r");
    if (fptr1 == NULL) {
        printf("No File Exist...\n");
        return 0;
    }

    FILE *fptr2;
    fptr2 = fopen("/home/parthka/Videos/p.mkv","w");
    if (fptr2 == NULL) {
        printf("No File Exist...\n");
        return 0;
    }

    fseek(fptr1, 0L, SEEK_END);
    long int s = ftell(fptr1);
    fseek(fptr1, 0L, SEEK_SET);

    char* fdata = (char*)calloc(s, sizeof(char));
    fread(fdata, s, 1, fptr1);
    fwrite(fdata, s, 1, fptr2);



    /*char ch;

    while (s--) {
        ch = getc(fptr1);
        putc(ch, fptr2);
    }
    */



    fclose(fptr1);
    fclose(fptr2);

    return 0;
}
