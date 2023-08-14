#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 100
#define MAX_RANGE 100

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair*, int, pair);

int cmp (const void *a, const void *b);
int insert_int (int *tab, int n, int new_element);
int get_domain(pair *tab,int n,int *array);
// Case 1:

// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair*, int);

// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair*, int);

// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair*, int);

// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair*, int);

// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair*, int);

// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair*, int);

// find max value of x for xRy
int find_max_value(pair *tab,int n){
    int max=tab[0].first;
    for(int i=1;i<n;i++){
        if(tab[i].first>max){
            max=tab[i].first;
        }
    }
    return max;
}

// count unique values of x for xRy
int count_unique(pair *tab,int n){
    int cnt=0;
    int max= find_max_value(tab,n);
    int array[max+1];
    memset(array,0,(max+1)*sizeof(int));
    for(int i=0;i<n;i++){
        if(array[tab[i].first]==0) {
            array[tab[i].first]=1;
            cnt++;
        }
    }
    return cnt;
}
//check if element in array
int in_array(int *tab,int n,int val){
    for(int i=0;i<n;i++){
        if(tab[i]==val) return 1;
    }
    return 0;
}

// fill unique values of X in array
void unique_values(pair *tab,int n,int *array){
    int j=0;
    for(int i=0;i<n;i++){
        if(in_array(array,n,tab[i].first)==0){
            array[j]=tab[i].first;
            j++;
        }
    }
}

int is_reflexive(pair *tab,int n){
    int unique=count_unique(tab,n);
    int cnt=0;
    for(int i=0;i<n;i++){
        if(tab[i].first==tab[i].second) cnt++;
    }
    return cnt==unique;
}

int is_irreflexive(pair *tab,int n){
    for(int i=0;i<n;i++){
        if(tab[i].first==tab[i].second) return 0;
    }
    return 1;
}
int is_symmetric(pair *tab,int n){
    int first,second,flag;
    for(int i=0;i<n;i++){
        flag=0;
        first=tab[i].first;
        second=tab[i].second;
        for(int j=0;j<n;j++){
            if(i!=j){
                if(first==tab[j].first && second==tab[j].second){
                    flag=1;
                    break;
                }
            }
        }
        if(flag==0){
            return 0;
        }
    }
    return 1;
}
int is_antisymmetric(pair *tab,int n){
    int first,second;
    for(int i=0;i<n;i++){
        if(tab[i].first!=tab[i].second){
            first=tab[i].first;
            second=tab[i].second;
            for(int j=0;j<n;j++){
                if(i!=j){
                    if(tab[j].second==first && tab[j].first==second){
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}
int is_asymmetric(pair *tab,int n){
    int first,second;
    for(int i=0;i<n;i++){
        first=tab[i].first;
        second=tab[i].second;
        for(int j=0;j<n;j++){
            if(tab[j].first==second && tab[j].second==first){
                return 0;
            }
        }
    }
    return 1;
}
int is_transitive(pair *tab,int n){
    pair a;
    int flag=0;
    for(int i=0;i<n;i++){
        a=tab[i];
        for(int j=0;j<n;j++) {
            if (i != j) {
                if (a.first == tab[j].second && a.second == tab[j].first) {
                    for(int k=0;k<n;k++){
                        if(k!=i && k!=j){
                            if(tab[k].first==a.first && tab[k].second==tab[j].second){
                                flag=1;
                                break;
                            }
                        }
                    }
                    if(flag==1){
                        break;
                    }
                    else return 0;
                }
            }
            flag=0;
        }
    }
    return 1;
}


// Case 2:
void print_int_array(int *array, int n);
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair*, int);

// A total order relation is a partial order relation that is connected
int is_total_order(pair*, int);

// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair*, int);

int find_max_elements(pair*, int, int*);
int find_min_elements(pair*, int, int*);

int is_partial_order(pair *tab,int n){
    return is_reflexive(tab,n) && is_transitive(tab,n) && is_antisymmetric(tab,n);
}
int is_total_order(pair *tab,int n){
    return is_partial_order(tab,n) && is_connected(tab,n);
}
int is_connected(pair *tab,int n){
    int flag=0;
    int size=count_unique(tab,n);
    int array[size];
    memset(array,1000000,(size+1)*sizeof(int));
    unique_values(tab,n,array);
//    printf("array:\n");
//    print_int_array(array,size);
    for(int i=0;i<size;i++){
        for(int j=0;j<size;j++){
            if(i!=j){
                for(int k=0;k<n;k++){
                    if((tab[k].first==array[i] && tab[k].second==array[j]) || (tab[k].first==array[j] && tab[k].second==array[i])){
                        flag=1;
                        break;
                    }
                }
                if(flag==0){
                    return 0;
                }
            }
        }
        flag=0;
    }
    return 1;
}
int find_max_elements(pair *tab,int n,int *maxes){
    int j=0;
    int can,flag=0;
    for(int k=0;k<n;k++){
        can=tab[k].first;
        for(int l=0;l<n;l++){
            if(tab[l].first==can && tab[l].second!=can){
                flag=1;
                break;
            }
        }
        if(flag==0){
            j=insert_int(maxes,j,can);
//            maxes[j++]=can;
        }
        flag=0;
    }
    qsort(maxes,j,sizeof(int),cmp);
    return j;
}
int find_min_elements(pair *tab,int n,int *mins){
    int j=0;
    int can,flag=0;
    for(int k=0;k<n;k++){
        can=tab[k].first;
        for(int l=0;l<n;l++){
            if(tab[l].second==can && tab[l].first!=can){
                flag=1;
                break;
            }
        }
        if(flag==0){
            j=insert_int(mins,j,can);
//            mins[j++]=can;
        }
        flag=0;
    }
    qsort(mins,j,sizeof(int),cmp);
    return j;
}
int get_domain(pair *tab,int n,int *array){
    int j=0;
    memset(array,1000000,MAX_REL_SIZE*sizeof(int));
    for(int i=0;i<n;i++){
        j=insert_int(array,j,tab[i].first);
        j=insert_int(array,j,tab[i].second);
    }
    qsort(array,j,sizeof(int),cmp);
    return j;
}


// Case 3:

int composition (pair*, int, pair*, int, pair*);

int composition(pair *tab1,int n1,pair *tab2,int n2, pair *tab3){
    int count=0;
    pair new;
    for(int i=0;i<n1;i++){
        for(int j=0;j<n2;j++){
            if(tab1[i].second==tab2[j].first){
                new.first=tab1[i].first;
                new.second=tab2[j].second;
                count=add_relation(tab3,count,new);
            }
        }
    }
    return count;
}

// Comparator for pair
int cmp (const void *a, const void *b) {
    int int_a = * ( (int*) a );
    int int_b = * ( (int*) b );

    if ( int_a == int_b ) return 0;
    else if ( int_a < int_b ) return -1;
    else return 1;
}

int insert_int (int *tab, int n, int new_element) {
    for(int i=0;i<n;i++){
        if(tab[i]==new_element){
            return n;
        }
    }
    tab[n]=new_element;
    return ++n;
}

// Add pair to existing relation if not already there
int add_relation (pair *tab, int n, pair new_pair) {
    for(int i=0;i<n;i++){
        if(tab[0].first==new_pair.first && tab[0].second==new_pair.second){
            return n;
        }
    }
    tab[n]=new_pair;
    return ++n;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair *relation) {
    int n,len=0;
    pair new;
    scanf("%d\n",&n);
    for(int i=0;i<n;i++){
        scanf("%d %d",&new.first,&new.second);
        len=add_relation(relation,len,new);
    }
    return len;
}

void print_int_array(int *array, int n) {
    printf("%d\n",n);
    for(int i=0;i<n;i++){
        printf("%d ",array[i]);
    }
    printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];
	int max_elements[MAX_REL_SIZE];
	int min_elements[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size, relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

