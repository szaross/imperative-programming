#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };

#define CBUFF_SIZE 52
#define STACK_SIZE 52

int c_A[CBUFF_SIZE], c_B[CBUFF_SIZE];   // talie kart
int len_A=0,len_B=0,out_A=0,out_B=0;    // zmienne do cbuffa

int table_A[CBUFF_SIZE], table_B[CBUFF_SIZE];   // karty na stole
int top_tA=0,top_tB=0;    // zmienne do stacka

int conflict_count=0;

//STACK
int stack_push(int stack[],int* top,int x) {
    if(*top==STACK_SIZE) return OVERFLOW;
    stack[*top]=x;
    *top=*top+1;
    return OK;
}

int stack_pop(int stack[],int* top) {
    if(*top==0) return UNDERFLOW;
    *top=*top-1;
    int res=stack[*top];
    return res;
}

//int stack_state(void) {
//    return top;
//}


//


// Queue with cyclic buffer implementation
int cbuff_push(int cli_nr,int cbuff[],int* out,int* len) {
    if(*len==CBUFF_SIZE) return OVERFLOW;
    cbuff[(*out+*len)%CBUFF_SIZE]=cli_nr;
    *len=*len+1;
    return OK;
}

int cbuff_pop(int cbuff[],int* out,int* len) {
    if(*len==0) return UNDERFLOW;
    int num=cbuff[*out];
    *out=(*out+1)%CBUFF_SIZE;
    *len=*len-1;
    return num;
}

//int cbuff_state(void) {
//    return len;
//}

void cbuff_print(int cbuff[],int* out,int* len) {
    for(int i=0;i<*len;i++){
        printf("%d ",cbuff[(*out+i)%CBUFF_SIZE]);
    }
    printf("\n");
}
// End of implementation
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

void play_cards(void){  //stawianie karty na stol
    stack_push(table_A,&top_tA,cbuff_pop(c_A,&out_A,&len_A));
    stack_push(table_B,&top_tB,cbuff_pop(c_B,&out_B,&len_B));

}
int confict(int ver) {  //return: 0-bez wojny, 1-wojna
    conflict_count++;
    play_cards();
    int A=table_A[top_tA-1],B=table_B[top_tB-1];
    //printf("A:%d B:%d   A_LEN:%d, B_LEN:%d\n",A,B,len_A,len_B);
    for(int i=48;i>0;i-=4) {
        if (((A / i) + (B / i)) == 1) {
            if (A > B) {
                //printf("A WINS  i: %d\n",i);
                //A wygrywa
                for (int j = 0; j < top_tA; j++) {
                    //printf("DODAWANIE KARTY %d (A) DO REKI A\n", table_A[j]);
                    cbuff_push(table_A[j], c_A, &out_A, &len_A);
                }   // dodanie kart do talii A ze stolu A

                for (int j = 0; j < top_tB; j++) {
                    //printf("DODAWANIE KARTY %d (B) DO REKI A\n", table_B[j]);
                    cbuff_push(table_B[j], c_A, &out_A, &len_A);
                }   // dodanie kart do talii A ze stolu B

                top_tA = 0;
                top_tB = 0;   //zerowanie stolu
                break;

            } else {
                //B wygrywa
                //printf("B WINS  i: %d\n",i);
                for (int j = 0; j < top_tB; j++) {
                    //printf("DODAWANIE KARTY %d (B) DO REKI B\n", table_B[j]);
                    cbuff_push(table_B[j], c_B, &out_B, &len_B);
                }   // dodanie kart do talii B ze stolu B

                for (int j = 0; j < top_tA; j++) {
                    //printf("DODAWANIE KARTY %d (A) DO REKI B\n", table_A[j]);
                    cbuff_push(table_A[j], c_B, &out_B, &len_B);
                }   // dodanie kart do talii B ze stolu A

                top_tA = 0;
                top_tB = 0;   //zerowanie stolu
                break;

            }
        } else if ((A / i == 1 && B / i == 1) || (A < 4 && B < 4)) {
            //printf("WAR i: %d\n",i);
            if (ver == 1) { //uproszczona
                cbuff_push(stack_pop(table_A, &top_tA), c_A, &out_A, &len_A);
                cbuff_push(stack_pop(table_B, &top_tB), c_B, &out_B, &len_B);
                return 0;
            } else {   //standardowa
                return 1;
            }
        }
    }
    // sprawdzenie, czy ktos wygral cala rozgrywke
    if(len_B==0){
        printf("2 %d",conflict_count);
        exit(0);
    }
    else if(len_A==0){
        printf("3\n");
        cbuff_print(c_B,&out_B,&len_B);
        exit(0);
    }
    return 0;
}



void play(int ver,int max_conflicts) {
    int perm[CBUFF_SIZE];
    rand_permutation(CBUFF_SIZE,perm);

    // rozdanie kart
    for(int i=0;i<26;i++){
        cbuff_push(perm[i],c_A,&out_A,&len_A);
    }
    for(int i=26;i<52;i++){
        cbuff_push(perm[i],c_B,&out_B,&len_B);
    }
//    printf("A: %d %d\nB: %d %d\n",out_A,len_A,out_B,len_B);
//    cbuff_print(c_A,&out_A,&len_A);
//    cbuff_print(c_B,&out_B,&len_B);
//    exit(0);

    while(conflict_count<=max_conflicts){

        while(confict(ver)==1){
            //printf("%d\n",conflict_count);
            if(len_A<2 || len_B<2){ // remis, brakuje kart do roztrzygniecia wojny
                printf("1 %d %d",len_A+top_tA,len_B+top_tB);
                exit(0);
            }
            play_cards();

        }
        //printf("%d\n",conflict_count);
    }
    // za duzo konfliktow
    if(conflict_count>=max_conflicts) {
        printf("0 %d %d", len_A+top_tA, len_B+top_tB);
        exit(0);
    }



}

int main(void){
    int ver,seed,max_conflicts;
    scanf("%d %d %d",&seed,&ver,&max_conflicts);
    srand((unsigned) seed);
    play(ver,max_conflicts);

    return 0;
}