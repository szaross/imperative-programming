#include <stdio.h>

int main(void){
    int a=0,b=1,n;
    scanf("%d",&n);
    while(a*b<n){
        int tmp=b;
        b=a+b;
        a=tmp;
    }
    int odp = (a*b==n);
    printf("%d",odp);
    return 0;