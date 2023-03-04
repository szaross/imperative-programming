#include <stdio.h>
#include <math.h>

int is_prime(int n);
int is_nonincreasing(int n);

int main(void){
    int n;
    printf("Podaj liczbe: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++){
        if(is_prime(i)==1 && is_nonincreasing(i)==1){
            printf("%d ",i);
        }
    }
    return 0;
}

int is_prime(int n){
    if(n<=1) return 0;
    if(n==2 || n==3) return 1;
    if(n%2==0 || n%3==0) return 0;

    int i=5;
    while(i<=ceil(sqrt(n))){
        if(n%i==0) return 0;
        i+=2;
        if(n%i==0) return 0;
        i+=4;
    }
    return 1;
}

int is_nonincreasing(int n){
    int flag=1,d;
    while(n!=0 && flag==1){
        d=n%10;
        n/=10;
        if(d<n%10){
            flag=0;
        }
    }
    return flag;
}