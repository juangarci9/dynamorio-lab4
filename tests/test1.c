#include <stdio.h>
#include <stdlib.h>

int main(){
    int* ptr = (int*)malloc(4*sizeof(int));
    ptr[0] = 1;
    printf("test: %d\n", ptr[0]);
    free (ptr);
    return 0;
}