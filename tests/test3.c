#include <stdio.h>
#include <stdlib.h>

int main(){
    int* ptr = (int*)calloc(4,sizeof(int));
    ptr[0] = 1;
    ptr = (int*)realloc(ptr, 8 * sizeof(int));
    ptr[4] = 2;
    printf("test: %d %d\n", ptr[0], ptr[4]);
    free (ptr);
    return 0;
}