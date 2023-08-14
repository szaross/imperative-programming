#include <stdio.h>
#include <stdlib.h>

typedef struct _matrix{
    int rows;
    int cols;
    double **val;
    double *storage;
} Matrix;

//zad 1

void apply_to_rows(const Matrix *pm, void (*vect_fun)(double*,int)){
    for(int i=0;i<pm->rows;i++){
        vect_fun(pm->val[i],pm->cols);
    }
}

void multiply(double *row,int size){
    for(int i=0;i<size;i++){
        row[i]*=10;
    }
}

apply_to_rows(pm,multipy);

//

void inplace_add_scalar(Matrix *pm,double v){
    for(int i=0;i<pm->rows;i++){
        for(int j=0;j<pm->cols;j++){
            pm->val[i][j]+=v;
        }
    }
}

//
int swap_cols(Matrix *m,int c1,int c2){
    if(c1<0 || c1 >= m->cols || c2<0 || c2>=m->cols || m==NULL){
        return 0;
    }
    double tmp;
    for(int i=0;i<m->rows;i++){
        tmp=m->val[i][c1];
        m->val[i][c1]=m->val[i][c2];
        m->val[i][c2]=tmp;
    }

    return 1;
}
//

void max_element_in_rows(double *maxdata, const Matrix *pm){
    double max_tmp;
    for(int i=0;i<pm->rows;i++){
        max_tmp=pm->val[i][0];
        for(int j=1;j<pm->cols;j++){
            if(max_tmp<pm->val[i][j]){
                max_tmp=pm->val[i][j];
            }
        }
        maxdata[i]=max_tmp;
    }
}
//

void print_row(double *row, size_t n){
    for(int i=0;i<n;i++){
        printf("%f ",row[i]);
    }
}

//

int compar(const void *a,const void *b){
    double * p1 = (double *)a;
    double * p2 = (double *)b;
    return *p1-*p2;
}

void find_row(Matrix M, double key, double tol){
    for(int i=0;i<M.rows;i++){
        if(fabs(M.val[i][0]-key)<tol){
            print_row(p,M.cols);
        }
    }
}



















