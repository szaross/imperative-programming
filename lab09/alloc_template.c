#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <limits.h>

#define TAB_SIZE 1000
#define BUF_SIZE 1000

int get(int cols, int row, int col, const int *A)
{
    int index = (row)*cols + col;
    return index;
}

void set(int cols, int row, int col, int *A, int value)
{
    int index = get(cols, row, col, A);
    A[index] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, int *A, int *B, int *AB)
{
    int sum;
    int indexA, indexB;
    for (int i = 0; i < rowsA; i++)
    {
        for (int j = 0; j < colsB; j++)
        {
            sum = 0;
            for (int k = 0; k < colsA; k++)
            {
                indexA = get(colsA, i, k, A);
                indexB = get(colsB, k, j, B);
                sum = sum + (A[indexA] * B[indexB]);
            }
            set(colsB, i, j, AB, sum);
        }
    }
}

void read_mat(int rows, int cols, int *t)
{
    int val;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            scanf("%d", &val);
            set(cols, i, j, t, val);
        }
    }
}

void print_mat(int rows, int cols, int *t)
{
    int index;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            index = get(cols, i, j, t);
            printf("%d ", t[index]);
        }
        printf("\n");
    }
}

int read_char_lines(char *array[])
{
    char c_buff[BUF_SIZE];
    int lines=0;
    size_t len;

    while(fgets(c_buff,BUF_SIZE,stdin)!=NULL){
        len=strlen(c_buff);
        array[lines]= malloc((len+1)* sizeof(char));
        array[lines]= strdup(c_buff);
        array[lines][len-1]='\0';
        lines++;
    }

    return lines;
}

void write_char_line(char *array[], int n)
{
    printf("%s\n",array[n]);
}

void delete_lines(char *array[])
{
    for(int i=0;array[i]!=NULL;i++){
        free(array[i]);
    }
}

int read_int_lines_cont(int *ptr_array[])
{
    char c_buff[BUF_SIZE];
    int lines = 0;
    int cnt=0;

    while (fgets(c_buff, BUF_SIZE, stdin) != NULL)
    {
        ptr_array[lines + 1] = &ptr_array[0][cnt];
        for (char *ptr = strtok(c_buff, " \n"); ptr; ptr = strtok(NULL, " \n"))
        {
            ptr_array[0][cnt] = strtol(strdup(ptr), 0, 10);
            cnt++;
        }
        ptr_array[0][cnt] = INT_MAX; // wartownik
        cnt++;
        lines++;
    }
    return lines;
}

void write_int_line_cont(int *ptr_array[], int n)
{
    int i = 0;
    while (ptr_array[n+1][i] != INT_MAX)
    {
        printf("%d ", ptr_array[n+1][i]);
        i++;
    }
    printf("\n");
}

typedef struct
{
    int *values;
    int len;
    double average;
} line_type;

int read_int_lines(line_type lines_array[])
{
    char c_buff[BUF_SIZE];
    int lines = 0;
    int cnt,sum;

    while (fgets(c_buff, BUF_SIZE, stdin) != NULL)
    {
        lines_array[lines].values = (int*) malloc(0);
        cnt=0;
        sum=0;
        for (char *ptr = strtok(c_buff, " \n"); ptr; ptr = strtok(NULL, " \n"))
        {
            // poprawic realokacje (*2)
            lines_array[lines].values = (int*) realloc(lines_array[lines].values,(cnt+1) * sizeof(int));
            lines_array[lines].values[cnt] = strtol(strdup(ptr), 0, 10);
            sum+=lines_array[lines].values[cnt];
            cnt++;
        }
        lines_array[lines].len=cnt;
        lines_array[lines].average=(double)sum/cnt;
        lines++;
    }
    return lines;
}

void write_int_line(line_type lines_array[], int n)
{
    for(int i=0;i<lines_array[n].len;i++){
        printf("%d ",lines_array[n].values[i]);
    }
    printf("\n%.2f\n",lines_array[n].average);
}

void delete_int_lines(line_type array[], int line_count)
{
    for(int i=0;i<line_count;i++){
        free(array[i].values);
    }
}

int cmp(const void *a, const void *b)
{
    line_type * l1=(line_type *)a;
    line_type * l2=(line_type *)b;
    if(l1->average>l2->average){
        return 1;
    }
    else if(l1->average<l2->average){
        return -1;
    }
    else
        return 0;
}

void sort_by_average(line_type lines_array[], int line_count)
{
    qsort(lines_array,line_count, sizeof(line_type),cmp);
}

typedef struct
{
    int r, c, v;
} triplet;

int read_sparse(triplet *triplet_array, int n_triplets)
{
        for(int i=0;i<n_triplets;i++){
            scanf("%d %d %d\n",&triplet_array[i].r,&triplet_array[i].c,&triplet_array[i].v);
        }
        return n_triplets;
}

int cmp_triplets(const void *t1, const void *t2)
{
    triplet * l = (triplet *) t1;
    triplet * r = (triplet *) t2;

    if(l->r-r->r!=0){
        return l->r-r->r;
    }
    if(l->c-r->c!=0){
        return l->c-r->c;
    }
    // should never happen
    return 0;

}

void make_CSR(triplet *triplet_array, int n_triplets, int rows, int *V, int *C, int *R)
{
    qsort(triplet_array,n_triplets, sizeof(triplet),cmp_triplets);
    for(int i=0;i<n_triplets;i++){
//        printf("%d %d %d\n",triplet_array[i].r,triplet_array[i].c,triplet_array[i].v);
        V[i]=triplet_array[i].v;
        C[i]=triplet_array[i].c;
    }

    int count=0;
    R[0]=0;
    for(int i=1;i<rows+1;i++){
        for(int j=0;j<n_triplets;j++){
            if(triplet_array[j].r==i-1) count++;
        }
        R[i]=count;
    }
}

void multiply_by_vector(int rows, const int *V, const int *C, const int *R, const int *x, int *y)
{
    for(int i=0;i<rows;i++){
        y[i]=0;
        for(int j=R[i];j<R[i+1];j++){
            y[i]+=V[j]*x[C[j]];
        }
    }
}

void write_vector(int *v, int n)
{
    for(int i=0;i<n;i++){
        printf("%d ",v[i]);
    }
    printf("\n");
}

void read_vector(int *v, int n)
{
    for(int i=0;i<n;i++){
        scanf("%d",&v[i]);
    }
}

int read_int()
{
    char c_buf[BUF_SIZE];
    fgets(c_buf, BUF_SIZE, stdin);
    return (int)strtol(c_buf, NULL, 10);
}

int main(void)
{
    int to_do = read_int();

    int A[TAB_SIZE], B[TAB_SIZE], AB[TAB_SIZE];
    int n, lines_counter, rowsA, colsA, rowsB, colsB;
    int rows, cols, n_triplets;
    char *char_lines_array[TAB_SIZE] = {NULL};
    int continuous_array[TAB_SIZE];
    int *ptr_array[TAB_SIZE];
    triplet triplet_array[TAB_SIZE];
    int V[TAB_SIZE], C[TAB_SIZE], R[TAB_SIZE];
    int x[TAB_SIZE], y[TAB_SIZE];
    line_type int_lines_array[TAB_SIZE];

    switch (to_do)
    {
    case 1:
        scanf("%d %d", &rowsA, &colsA);
        read_mat(rowsA, colsA, A);
        scanf("%d %d", &rowsB, &colsB);
        read_mat(rowsB, colsB, B);
        prod_mat(rowsA, colsA, colsB, A, B, AB);
        print_mat(rowsA, colsB, AB);
        break;
    case 2:
        n = read_int() - 1; // we count from 1 :)
        ptr_array[0] = continuous_array;
        read_int_lines_cont(ptr_array);
        write_int_line_cont(ptr_array, n);
        break;
    case 3:
        n = read_int() - 1;
        read_char_lines(char_lines_array);
        write_char_line(char_lines_array, n);
        delete_lines(char_lines_array);
        break;
    case 4:
        n = read_int() - 1;
        lines_counter = read_int_lines(int_lines_array);
        sort_by_average(int_lines_array, lines_counter);
        write_int_line(int_lines_array, n);
        delete_int_lines(int_lines_array, lines_counter);
        break;
    case 5:
        scanf("%d %d %d", &rows, &cols, &n_triplets);
        n_triplets = read_sparse(triplet_array, n_triplets);
        read_vector(x, cols);
        make_CSR(triplet_array, n_triplets, rows, V, C, R);
        multiply_by_vector(rows, V, C, R, x, y);
        write_vector(V, n_triplets);
        write_vector(C, n_triplets);
        write_vector(R, rows + 1);
        write_vector(y, rows);
        break;
    default:
        printf("NOTHING TO DO FOR %d\n", to_do);
        break;
    }
    return 0;
}
