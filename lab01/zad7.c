#include <stdio.h>
#include <math.h>

int main(void){
    int a,b,n;
    printf("Podaj przedzial: ");
    scanf("%d %d",&a,&b);
    printf("Podaj liczbe punktow: ");
    scanf("%d",&n);

    float delta=(float)(b-a)/(float)n;
    float x,y;
    float field=0;

    for(int i=1;i<=n;i++){
        x=i*delta;
        y=sin(x);
        field+=y*delta;
    }
    printf("Pole: %f",field);

    return 0;
}