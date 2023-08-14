#include <stdio.h>
#include <stdlib.h>

enum Drive_type {combustion, electric, d_ndef};
enum Fuel_type {gasoline,diesel,gas,hydrogen,f_d_ndef};

// a)
typedef struct {
    char emmision[10];
    enum Fuel_type fuel_t;

}Comb_data;

typedef struct {
    enum Battery_Type bat_t;
    float capacity;
}El_data;


typedef struct {
    char *owner;
    Date valid_date;
    enum Drive_type d_type;
    union{
        Comb_data cs;
        El_data es;
    }drv_union;
}Vehicle;

typedef struct {int d; int m; int y;} Date;

// b)

int cmp(Date *d1, Date *d2) {
    if (d1->y != d2->y)
        return (d1->y < d2->y) ? -1 : 1;
    else if (d1->m != d2->m)
        return (d1->m < d2->m) ? -1 : 1;
    else
        return (d1->d < d2->d) ? -1 : (d1->d > d2->d);
}


int delayed(const Vehicle *tv, int size, const Date *base_date, char ***pdelayed_owners){
    int len=0;
    char ** delayed_owners=malloc(size*100*sizeof(char));
    for(int i=0;i<size;i++){
        if(cmp(&tv[i].valid_date,base_date)<0){
            if(len>100){
                **delayed_owners= realloc(size*2*len*sizeof(char));
            }
            delayed_owners[len]=tv[i].owner;
            len++;

        }
    }
    *pdelayed_owners=delayed_owners;
    return len;
}


// c)

void print_owners(const char **t_owners, size_t size){
    for(int i=0;i<size;i++){
        printf("%s",t_owners[i]);
    }
}

// d)

void init_vehicle(Vehicle *v){
    v->owner=NULL;
    v->valid_date.d=1;
    v->valid_date.m=1;
    v->valid_date.y=1970;
    v->d_type=d_ndef;
}

// e)
int put_data(Vehicle *v,enum Drive_type type, const void *data){
    if(type==combustion && v->d_type!=combustion){
        Comb_data *d=(Comb_data*)data;
        v->drv_union.cs.emmision=d->emmision;
        v->drv_union.cs.fuel_t=d->fuel_t;
    }
    else if (type==electric && v->d_type!=electric){
        El_data *d=(El_data*)data;
        v->drv_union.es.bat_t=d->bat_t;
        v->drv_union.es.capacity=d->capacity;
    }
    else if (v->d_type==d_ndef){
        return 1;
    }
    return 0;
}


// f)

void new_owner(Vehicle *v, const char *str){
    size_t len=sizeof(*str);
    char *p= malloc(len+1);
    strcpy(p,str);
    v->owner=p;
    free((void*)str);
}










