#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int generate(int a, int b);

int main(void){
    srand(time(0));
    int x=rand()%101,n=101,a=0,b=100;
    printf("x: %d\n",x);
    while(x!=n){
        n=generate(a,b+1);
        printf("Guessed: %d  [%d,%d]\n",n,a,b);
        if(n>x) b=n;
        else if(n<x) a=n;
    }
    return 0;
}

int generate(int a, int b){
    return (rand()%(b-a))+a;

}