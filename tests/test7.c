#include <stdio.h>
#include <stdlib.h>

int main(){
    int* ptr1 = (int*)calloc(4,sizeof(int));
    int* ptr2 = (int*)malloc(4*sizeof(int));
    int* ptr3 = (int*)malloc(4*sizeof(int));
//    ptr3[0] = 3;
//    ptr2[0] = 2;
//    ptr1[0] = 1;
    ptr1 = (int*)realloc(ptr1, 8 * sizeof(int));
//    ptr1[4] = 2;
//    printf("ptr1[0]: %d, ptr1[4]: %d\n", ptr1[0], ptr1[4]);
//    printf("ptr2[0]: %d\n", ptr2[0]);
//    printf("ptr3[0]: %d\n", ptr3[0]);
//    free (ptr3);
//    free (ptr2);
//    free (ptr1);
    return 0;
}