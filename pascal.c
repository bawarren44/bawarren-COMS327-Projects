#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * Author: Brad Warren
 * Id Number: 184030907
 * Email: bawarren@iastate.edu
 */

/*
 *  Builds the first N rows of Pascal's triangle, where N is given as a parameter. This should allocate memory for an array of N pointers,
 *  and initialize pointer i to point to an array of i+1 unsigned longs (because row i of Pascal's triangle will contain exactly i+1 non-zero entries).
 *  Return a null pointer (and release any memory allocated) if any memory allocation request fails.
 */
unsigned long** build_Pascal(unsigned N){
    int count = 1;
    unsigned long **ptr = malloc(sizeof(unsigned long*)*N);
    if(ptr == NULL){
        return ptr;
    }

    for(int i = 0; i < (int) N; i++){
        *(ptr + i) = malloc(sizeof(unsigned long)*N);
        if(*(ptr + i) == NULL){
            return NULL;
        }
        *(*(ptr + i) + 0) = 1;
    }
    *(*(ptr + 1) + 1) = 1;

    for(int i = 2; i < (int) N; i++){
        while(count < i){
            *(*(ptr + i) + count) = *(*(ptr + i - 1) + count - 1) + *(*(ptr + i - 1) + count);
            count++;
        }
        *(*(ptr + i) + count) = 1;
        count = 1;
    }
    return ptr;
}

/*
 * Displays the first N rows of Pascal's triangle, where N and the triangle are given as parameters.
 * Output should be organized into columns, so that the width of each column is just wide enough to display the largest integer in that column.
 */
void show_Pascal(unsigned long** P, unsigned N){
    int checkNumber = (int)N/2;
    int count = 0;
    unsigned long n = *(*(P + N -1) + checkNumber);
    while(n != 0)
    {
        n /= 10;
        ++count;
    }
    count++;
    printf("%*lu \n", count, *(*(P + 0) + 0));
    printf("%*lu", count, *(*(P + 1) + 0));
    printf("%*lu \n", count, *(*(P + 1) + 1));
    for(int i = 2; (int)i < N; i++){
        for(int j = 0; (int)j <= i; j++){
            printf("%*lu", count, *(*(P + i) + j));
            if(i == j){
                printf("\n");
            }
        }
    }
}

/*
 * Destroys (free the allocated memory for) the first N rows of Pascal's triangle.
 */
void destroy_Pascal(unsigned long** P, unsigned N){
    for(int i = 0; i < (int)N; i++){
        free(P[i]);
    }
    free(P);
}


int main(){
    int N = 0;
    printf("Please type number to build Pascal Triangle, then press enter\n");
    scanf("%d", &N);
    unsigned long **p = build_Pascal(N);
    show_Pascal(p, N);
    destroy_Pascal(p, N);
}