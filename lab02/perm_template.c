#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 100

// Returns an integer from [a,b] using library function rand() and operator %
// if a > b return INT_MIN
// if b - a > RAND_MAX return INT_MAX
// if a == b return a
// else return integer from [a,b]
int rand_from_interval(int a, int b) {
    if(a>b) return INT_MAX;
    if(b-a>RAND_MAX) return INT_MAX;
    if(a==b) return a;
    else return (rand()%(b-a+1))+a;
}

void swap (int array[], int i, int k) {
    int tmp=array[i];
    array[i]=array[k];
    array[k]=tmp;
}

// random permutation of integers from [0, n-1]
void rand_permutation(int n, int array[]) {
    if(n>=1){
        int k;
        for(int i=0;i<n;i++){
            array[i]=i;
        }
        for(int i=0;i<n-1;i++){
            k=rand_from_interval(i,n-1);
            swap(array,i,k);
        }
    }

}

// bubble sort (increasing order)
// returns number of iterations of the external loop
// after which the array is sorted
// for { 0 1 2 3 7 4 5 6 } -> 1,
// for { 1 2 3 7 4 5 6 0 } -> 7,
// for { 0 1 2 3 4 5 6 7 } -> 0.
int bubble_sort(int n, int array[]) {
    int count=0,flag=0;
    for(int i=0;i<n-1;i++){
        flag=0;
        for(int j=0;j<n-1;j++){
            if(array[j]>array[j+1]){
                swap(array,j,j+1);
                flag=1;
            }
        }
        if(flag==0) return count;
        count++;
    }
    return count;
}

int main(void) {
    int to_do, seed;
    int a, b, m, n;
    int array[N];

    scanf("%d %d", &to_do, &seed);
    srand((unsigned) seed); // set the seed

    switch(to_do) {
        case 1:
            scanf("%d %d %d",&a, &b, &m);
            for(int i = 0; i < m; ++i) {
                printf("%d ", rand_from_interval(a, b));
            }
            printf("\n");
            break;
        case 2:
            scanf("%d", &n);
            rand_permutation(n, array);
            for(int i = 0; i < n; ++i) printf("%d ", array[i]);
            printf("\n");
            break;
        case 3:
            scanf("%d", &n);
            rand_permutation(n, array);
            printf("%d\n", bubble_sort(n, array));
            break;
        default:
            printf("NOTHING TO DO!\n");
            break;
    }
    return 0;
}

