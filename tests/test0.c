#include <stdio.h>
#include <stdlib.h>

int main(){
    int* ptr1 = (int*)malloc(4*sizeof(int));
    int* ptr2 = (int*)calloc(2,sizeof(int));
    int* ptr3 = (int*)malloc(2*sizeof(int));
    ptr3 = (int*)realloc(ptr3,8*sizeof(int));
//    ptr1[0] = 1;
//    printf("test: %d\n", ptr1[0]);
//    free (ptr1);
//    ptr2[0] = 2;
//    printf("test: %d\n", ptr2[0]);
    free (ptr1);
    free (ptr2);
    free (ptr3);
    return 0;
}