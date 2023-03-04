#include <stdio.h>


int dosk(int n);

int main(void){
    int a,b;
    printf("Podaj przedzial: ");
    scanf("%d %d",&a,&b);

    for(int i=a;i<=b;i++){
        if(dosk(i)==1){
            printf("%d\n",i);
        }
    }
    return 0;
}

int dosk(int n) {
    int suma = 0;
    for (int i = 1; i < n; i++) {
        if (n % i == 0) {
            suma += i;
        }
    }
    return (n == suma);
}