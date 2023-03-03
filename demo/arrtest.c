#include "tests.h"
#include <stdio.h>
#include <string.h>
#define ST_OUTSIZE 20
int arrayToBeTested[] =
{ 
    1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
    31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
    51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
    61, 62, 63, 64, 65, 66, 67, 68, 69, 70,
    71, 72, 73, 74, 75, 76, 77, 78, 79, 80 
};

void sample_test(void) {
    
    int outarr[ST_OUTSIZE];
    sample(arrayToBeTested,sizeof(int),80,20,outarr);
    printf("80 -> 20:\n");
    for (int i = 0; i < 20; i++) {
        printf("%2d\t",outarr[i]);
        if ((i + 1) % 5 == 0) putchar('\n');
    }
    putchar('\n');
}

void shuffle_test(void) {
    int arr2[80];
    
    memcpy(arr2, arrayToBeTested, sizeof(int) * 80);
    shuffle(arr2,sizeof(int),80);
    printf("80 -> 80:\n");
    for (int i = 0; i < 80; i++) {
        printf("%2d\t", arr2[i]);
        if ((i + 1) % 10 == 0) putchar('\n');
    }
    putchar('\n');
}
