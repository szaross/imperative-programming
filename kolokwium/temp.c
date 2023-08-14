#include <stdio.h>
#include <stdlib.h>

//while (fgets(c_buff, BUF_SIZE, stdin) != NULL)
//{
//    for (char *ptr = strtok(c_buff, " \n"); ptr; ptr = strtok(NULL, " \n"))
//    {
//        // do smth
//    }
//
//}




// zad 1

typedef struct _matrix{
    int rows;
    int cols;
    double**data;
}Matrix;

int create_matrix(Matrix*pmatrix,int rows,int cols);


int get(Matrix*pm,int row,int col, double*pvalue){
    if(row<0 || row>pm->rows - 1 || col<0 || col>pm->cols - 1 || pm->data==NULL){
        return 0;
    }
    *pvalue=pm->data[row][col];
    return 1;
}

int set(Matrix*pm,int row,int col, double value){
    if(row<0 || row>pm->rows - 1 || col<0 || col>pm->cols - 1 || pm->data==NULL){
        return 0;
    }
    pm->data[row][col]=value;
    return 1;
}

void create_identity_matrix(Matrix*pm,int size){
    if(create_matrix(pm,size,size)){
        for(int i=0;i<size;i++){
            pm->data[i][i]=1;
        }
    }
}

double dot_prod_vect(const double *v1,const double *v2,int size){
    double product=0.0;
    for(int i=0;i<size;i++){
        product+=v1[i]*v2[i];
    }
    return product;
}

void mult_vect(double *result,const Matrix*pm,const double *vect){
    for(int i=0;i<pm->rows;i++){
        result[i]=dot_prod_vect(vect,pm->data[i],pm->cols);
    }
}

void max_element_in_rows(double*maxdata,const Matrix*pm){
    double max;
    for(int i=0;i<pm->rows;i++){
        max=pm->data[i][0];
        for(int j=1;j<pm->cols;j++){
            if(max<pm->data[i][j]){
                max=pm->data[i][j];
            }
        }
        maxdata[i]=max;
    }
}

/////


// zad 2

typedef struct {
    double *results;
    int len;
} Data;

typedef struct tagQueueElement{
    Data data;
    struct tagQueueElement *next;
} QueueElement;

typedef struct tagQueue{
    QueueElement *head;
    QueueElement *tail;
} Queue;


void free_queue(Queue *pqueue){
    QueueElement *h=pqueue->head;
    QueueElement *tmp;
    while(h->next!=NULL){
        tmp=h;
        free(tmp);
        h=h->next;
    }
    free(h);
    pqueue->head=NULL;
    pqueue->tail=NULL;
}


void push_copy(Queue *pqueue,const Data* pdata){
    QueueElement *new= (QueueElement *)malloc(sizeof(QueueElement));
    new->data=*pdata;
    new->next=NULL;


    if(pqueue->tail==NULL){
        pqueue->tail=new;
        pqueue->head=new;
    }
    else {
        pqueue->tail->next = new;
        pqueue->tail = new;
    }
}

int peek(const Queue *pqueue,Data *pdata){
    if(pqueue->head==NULL){
        return 0;
    }
    *pdata=pqueue->head->data;
    return 1;
}

int pop(Queue *pqueue,Data *pdata){
    if(pqueue->head==NULL){
        return -1;
    }
    *pdata=pqueue->head->data;
    QueueElement *tmp=pqueue->head;
    pqueue->head=pqueue->head->next;

    if(pqueue->head==NULL){
        pqueue->tail=NULL;
        return 0;
    }
    free(tmp);
    return 1;
}

int get_total_count(const Queue *pqueue){
    int sum=0;
    QueueElement *p=pqueue->head;

    while(p!=NULL){
        sum+=p->data.len;
        p=p->next;
    }
    return sum;
}


typedef enum  {car,bus,truck} VehicleType;
typedef enum  {electric,hybrid,combustion}EngineType ;

struct sCar{
    int capacity;
    float engine_power;
};

struct sBus{
    int sit_capacity;
    int stand_capacity;
};

struct sTruck{
    float liters;
    float axis_pressure;
};

struct Vehicle{
    char* owner;
    int date[3];
    VehicleType Vtype;
    EngineType Etype;
    union {
        struct sCar scar;
        struct sBus sbus;
        struct sTruck struck;
    };
};






