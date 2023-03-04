#include <stdio.h>


int silnia(int n);

int main(void){
    int n=0;
    printf("Podaj liczbe: ");
    scanf("%d",&n);
    printf("%d",silnia(n));
    return 0;
}


int silnia(int n) {
    if (n > 1) {
        return silnia(n - 1) * n;;
    }
    return 1;

}