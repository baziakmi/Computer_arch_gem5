#include <stdio.h>
#define N 10

int main(){
    int sum = 0;

    for(int i = 1; i <= N; i++){
        sum += i;
    }

    printf("Sum = %d\n", sum);
    return 0;
}