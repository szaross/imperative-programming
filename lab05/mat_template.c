#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
    double sum;
    for(int i=0;i<m;i++){
        for(int j=0;j<n;j++){
            sum=0;
            for(int k=0;k<p;k++){
                sum=sum+(A[i][k]*B[k][j]);
            }
            AB[i][j]=sum;
        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    double sub;
    double det=1;
    int flag=0;
    for (int i=0;i<n;i++){
        for (int j=i+1;j<n;j++){
            if(A[i][i]==0) {
                det=0;
                flag=1;
                break;
            }
            sub=A[j][i]/A[i][i];
            for (int k=0;k<n;k++){
                A[j][k]=A[j][k]-(sub*A[i][k]);
            }
        }
        if (flag) break;
    }
    for(int i=0;i<n;i++){
        det*=A[i][i];
    }
    if(det==0) return NAN;
    return det;
};

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], double b[], double x[], const int n, const double eps) {
     // initialize permutation vector
    int perm[n];
    int num_of_per=0;
    for(int i=0;i<n;i++) perm[i]=i;

    // look for a swap available
    double max=A[0][0];
    int ind=0;
    for(int i=1;i<n;i++){
        if(fabs(A[i][0])>fabs(max) && A[i][i]!=0) {
            max=A[i][0];
            ind=i;
        }
    }
    // swap
    if(ind!=0){
        perm[0]=ind;
        perm[ind]=0;
        num_of_per++;
    }

    double det=1;

    // triangulation
    double sub;
    int tmp, flag=0;
    for (int i=0;i<n;i++){
        for (int j=i+1;j<n;j++){

            // swap rows
            int l=i;
            max=A[perm[l]][i];
            ind=i;
            while(l<n){
                if(fabs(A[perm[l]][i])>fabs(max) && A[perm[l]][i]!=0) {
                    max=A[perm[l]][i];
                    ind=l;
                }
                l++;
            }

            if (ind!=i){
                tmp=perm[ind];
                perm[ind]=perm[i];
                perm[i]=tmp;
                num_of_per++;
            }

            //
            if(fabs(A[perm[i]][i])<eps) {
                return 0;
            }
            //

            // gauss
            sub=A[perm[j]][i]/A[perm[i]][i];
            for (int k=0;k<n;k++){
                A[perm[j]][k]=A[perm[j]][k]-(sub*A[perm[i]][k]);
            }
            b[perm[j]]=b[perm[j]]-(sub*b[perm[i]]);
        }
    }


    // determinant
    for(int i=0;i<n;i++) det*=A[perm[i]][i];


    // check if b!=NULL
    int b_nonzero=1;
    for(int i=0;i<n;i++){
        if(b[i]==0){
            b_nonzero=0;
            break;
        }
    }

    // Ax=b solution //triangulation upwards
    if(det!=0 && b_nonzero){
        for (int i=n-1;i>-1;i--){
            for (int j=i-1;j>-1;j--){
                sub=A[perm[j]][i]/A[perm[i]][i];
                for (int k=0;k<n;k++){
                    A[perm[j]][k]=A[perm[j]][k]-(sub*A[perm[i]][k]);
                }
                b[perm[j]]=b[perm[j]]-(sub*b[perm[i]]);
            }
        }

        // put solution to vector x
        for(int i=0;i<n;i++) x[i]=b[perm[i]]/A[perm[i]][i];

        // check for negative zeros
        for(int i=0;i<n;i++) {
            if(x[i]==0) x[i]=0;
        }

    }
    //

    // change determinant sign considering permutations
    if(num_of_per%2!=0){
        det=-det;
    }

    return det;
}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {

    // initialize identity matrix
    double I[n][n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i==j) I[i][j]=1;
            else I[i][j]=0;
        }
    }
    // initialize permutation vector
    int perm[n];
    int num_of_per=0;
    for(int i=0;i<n;i++) {
        perm[i]=i;
    }
    // look for a swap available
    double max=A[0][0];
    int ind=0;
    for(int i=1;i<n;i++){
        if(A[i][0]>max && A[i][i]!=0) {
            max=A[i][0];
            ind=i;
        }
    }
    // swap
    if(ind!=0){
        perm[0]=ind;
        perm[ind]=0;
        num_of_per++;
    }

    double det=1;

    // triangulation
    double sub;
    int tmp;
    for (int i=0;i<n;i++){
        for (int j=i+1;j<n;j++){


            // swap rows
            int l=i;
            max=A[perm[l]][i];
            ind=i;
            while(l<n){
                if(fabs(A[perm[l]][i])>fabs(max) && A[perm[l]][i]!=0) {
                    max=A[perm[l]][i];
                    ind=l;
                }
                l++;
            }

            if (ind!=i){
                tmp=perm[ind];
                perm[ind]=perm[i];
                perm[i]=tmp;
                num_of_per++;
            }

            //
            if(fabs(A[perm[i]][i])<eps) {
                return 0;
            }
            //
            sub=A[perm[j]][i]/A[perm[i]][i];
            for (int k=0;k<n;k++){
                A[perm[j]][k]=A[perm[j]][k]-(sub*A[perm[i]][k]);
                I[perm[j]][k]=I[perm[j]][k]-(sub*I[perm[i]][k]);
            }
        }
    }

    // determinant
    for(int i=0;i<n;i++) det*=A[perm[i]][i];

    // triangulation upwards
    if(det!=0){
        for (int i=n-1;i>-1;i--){
            for (int j=i-1;j>-1;j--){
                sub=A[perm[j]][i]/A[perm[i]][i];
                for (int k=0;k<n;k++){
                    A[perm[j]][k]=A[perm[j]][k]-(sub*A[perm[i]][k]);
                    I[perm[j]][k]=I[perm[j]][k]-(sub*I[perm[i]][k]);
                }
            }
        }
    }

    // finish transforming A into identity matrix
    for(int i=0;i<n;i++){
        sub=A[perm[i]][i];
        A[perm[i]][i]/=sub;
        for(int j=0;j<n;j++){
            I[perm[i]][j]=I[perm[i]][j]/sub;
        }
    }

    // copy I-matrix to B in the right order
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            B[i][j]=I[perm[i]][j];
        }
    }

    // change determinant sign considering permutations
    if(num_of_per%2!=0){
        det=-det;
    }

    return det;
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

