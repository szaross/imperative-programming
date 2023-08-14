#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

typedef struct
{
    int day;
    int month;
    int year;
} Date;

// 1 bsearch2

#define FOOD_MAX 30       // max. number of goods
#define ART_MAX 15        // max. length of name's name + 1
#define RECORD_MAX 40 // max. line length

typedef struct
{
    char name[20];
    float price;
    int amount;
    Date valid_date;
} Food;

typedef int (*ComparFp)(const void *, const void *);

// compare dates
int cmp_date(const void *d1, const void *d2)
{
    const Date *f1 = (Date *)d1;
    const Date *f2 = (Date *)d2;

    if (f1->year != f2->year)
    {
        return f1->year - f2->year;
    }
    if (f1->month != f2->month)
    {
        return f1->month - f2->month;
    }
    if (f1->day != f2->day)
    {
        return f1->day - f2->day;
    }
    return 0;
}

// compare foods
int cmp(const void *a, const void *b)
{
    const Food *f1 = (Food *)a;
    const Food *f2 = (Food *)b;

    if (f1->price != f2->price)
    {
        if(f1->price > f2->price)
            return 1;
        else
            return -1;
    }

    return cmp_date(&f1->valid_date, &f2->valid_date);
}

// bsearch returning address where to insert new element
void *bsearch2(const void *key, const void *base, size_t nmemb, size_t size, ComparFp compar, char *result)
{
    char * tab = (char *) base;
    for(int i=0;i<nmemb;i++){
        if(compar((const void*)&tab[i*size],key)==0){
            *result='1';
            return (void*)&tab[i*size];
        }
    }
    *result='0';
    return (void *)&tab[nmemb*size];



    size_t i = 0,mid;
    size_t j = nmemb==0 ? 0 : nmemb-1;
    int res;
    while (i < j)
    {
        mid = i + (j - i) / 2;
        void *f1= &tab[mid*size];
        res = compar((const void *)f1, key);
        if (res == 0)
        {
            *result = '1';
            return (void *)f1;
        }
        else if (res > 0)
        {
            i = mid+1;
        }
        else
        {
            j = mid;
        }
    }
    *result = '0';
    return (void *)&tab[i*size];
}

// print goods of given name
void print_art(Food *p, int n, char *art)
{
    for (int i = 0; i <n; i++)
    {
        if (strcmp(p[i].name, art) == 0)
        {
            printf("%.2f %d %02d.%02d.%d\n",p[i].price, p[i].amount, p[i].valid_date.day, p[i].valid_date.month, p[i].valid_date.year);
        }
    }
}

// add record to table if absent
Food *add_record(Food *tab, int *np, ComparFp compar, Food *new)
{
    char res;
    Food *p = bsearch2(new, tab, *np, sizeof(Food), compar, &res);
    if (res == '0')
    {
        for(int i=*np;&tab[i]!=p;i--){
            tab[i]=tab[i-1];
        }
        *p=*new;
        return p;
    }
    else
    {
        p->amount += new->amount;
//        printf("adding %d to %s\n",new->amount,p->name);
        return p;
    }
}

// read no lines of data
// calls add_record if sorted = 1 and just adds element if sorted = 0
int read_goods(Food *tab, int no, FILE *stream, int sorted)
{
    int n = 0;
    for (int i = 0; i < no; i++)
    {
        Food new;
        fscanf(stream, "%s %f %d %d.%d.%d", new.name, &new.price, &new.amount, &new.valid_date.day, &new.valid_date.month, &new.valid_date.year);
        if (sorted)
        {
            Food *p=add_record(tab, &n, cmp, &new);
            if(p==&tab[n]){
                n++;
            }
        }

        else
        {
            tab[i] = new;
            n++;
        }

    }
    if (sorted)
        return n+1;
    else
        return n;
}

int cmp_qs(const void *a, const void *b)
{
    Food *fa = (Food *)a, *fb = (Food *)b;
    return cmp_date(&fa->valid_date, &fb->valid_date);
}

int cmp_bs(const void *a, const void *b)
{
    Date *pd = (Date *)a;
    Food *fb = (Food *)b;
    return cmp_date(pd, &fb->valid_date);
}

// finds the value of goods due on 'curr_date'
float value(Food *food_tab, size_t n, Date curr_date, int days)
{
    float sum=0;
    qsort(food_tab,n,sizeof(Food),cmp_qs);

    // set times
    struct tm tm_exp_date={.tm_year=curr_date.year-1900,.tm_mday=curr_date.day,.tm_mon=curr_date.month-1};
    time_t exp_date;
    exp_date= mktime(&tm_exp_date);
    int seconds=days*24*60*60;
    exp_date+=seconds;
    struct tm tm_date= *localtime(&exp_date);
    Date exp_date2={.day=tm_date.tm_mday,.month=tm_date.tm_mon+1,.year=tm_date.tm_year+1900};
//    printf("exp_date2: %d %d %d\n",exp_date2.day,exp_date2.month,exp_date2.year);
    char res;
    //find the element in array
    Food *p= (Food *) bsearch(&exp_date2,food_tab,n,sizeof(Food),cmp_bs);
//    Food *p= bsearch2(&exp_date2,food_tab,n,sizeof(Food),cmp_bs,&res);

//    printf("found %s %.2f %d.00 %02d.%02d.%d\n",p->name, p->price, p->amount, p->valid_date.day, p->valid_date.month, p->valid_date.year);

    if(p!=NULL) {

        // look for rest of items with exp_date <-
        Food *p1 = p;
        for (; p1 != &food_tab[0]; --p1) {
            if (cmp_date(&exp_date2, &p1->valid_date) == 0) {
                sum += p1->price * (float) p1->amount;
            } else
                break;
        }
        if (cmp_date(&exp_date2, &food_tab[0].valid_date) == 0) {
            sum += food_tab[0].price * (float) food_tab[0].amount;
        }


        // look for rest of items with exp_date ->
        Food *p2 = ++p;
        for (; p2 != &food_tab[n-1]; ++p2) {
            if (cmp_date(&exp_date2, &p2->valid_date) == 0) {
                sum += p2->price * (float) p2->amount;
            } else
                break;
        }
        if (cmp_date(&exp_date2, &food_tab[n-1].valid_date) == 0) {
            sum += food_tab[n-1].price * (float) food_tab[n-1].amount;
        }
    }

    return sum;
}

/////////////////////////////////////////////////////////////////
// 3 Succession

#define CHILD_MAX 20

enum Sex
{
    F,
    M
};
enum BOOL
{
    no,
    yes
};

struct Bit_data
{
    enum Sex sex : 1;
    enum BOOL in_line : 1;
};

typedef struct
{
    char *name;
    struct Bit_data bits;
    Date born;
    char *parent;
} Person;

typedef struct
{
    char *par_name;
    int index;
} Parent;

const Date primo_date = {28, 10, 2011}; // new succession act

int cmp_primo(Person *person1, Person *person2) {
    if (person1->bits.sex == person2->bits.sex) return 0;
    if (person1->bits.sex == F && cmp_date(&person1->born, &primo_date) > 0) return 0;
    if (person2->bits.sex == F && cmp_date(&person2->born, &primo_date) > 0) return 0;
    return person2->bits.sex - person1->bits.sex;
}

// compare persons
int cmp_person(const void *a, const void *b) {
    Person *pa = (Person*)a, *pb = (Person*)b;
    if (pa->parent == NULL) return -1;
    if (pb->parent == NULL) return 1;
    int result = strcmp(pa->parent, pb->parent);
    if (result == 0){
        result = cmp_primo(pa, pb);
        if (result == 0) return cmp_date(&pa->born, &pb->born);
        return result;
    }
    return result;
}

int cmp_parents(const void *a,const void *b){
    Person * p1=(Person *)a;
    Person * p2=(Person *)b;
    return strcmp(p1->name,p2->parent);
}

int cmp_age(const void *a,const void *b){
    Person * p1=(Person *)a;
    Person * p2=(Person *)b;
    return cmp_date(&p1->born,&p2->born);
}

int fill_indices_tab(Parent *idx_tab, Person *pers_tab, int size)
{
    Parent a;
    int parents=0;
    int index;
    Person *p;
    for(int i=0;i<size;i++){
        a.par_name=pers_tab[i].parent;
        p=bsearch(&pers_tab[i],pers_tab,size,sizeof(Person),cmp_parents);
        if(p!=NULL){
            // find the first child, *p doesn't have to be the first one
            index=p-pers_tab;
//            printf("Found children of %s on index %d\n",pers_tab[i].name,(int)index);
            for(;index>0;index--){
                if(strcmp(pers_tab[i].name,pers_tab[index].parent)!=0){
                    break;
                }
            }
            index++;

            idx_tab[parents].par_name=pers_tab[i].name;
            idx_tab[parents].index=(int)index;
            parents++;
        }
    }

    return parents;
}

void persons_shifts(Person *person_tab, int size, Parent *idx_tab, int no_parents)
{
    int no_children;

    for(int k=0;k<size;k++){
        for(int i=0;i<no_parents;i++){
            if(strcmp(person_tab[k].name,idx_tab[i].par_name)==0){

                // find the number of children
                no_children=0;
                for(int j=idx_tab[i].index;j<size;j++){
                    if(strcmp(idx_tab[i].par_name,person_tab[j].parent)!=0){
                        break;
                    }
                    no_children++;
                }

                Person tmp[no_children];
                memcpy(tmp,person_tab+idx_tab[i].index,sizeof(Person)*no_children);

                memcpy(person_tab+k+1+no_children,person_tab+k+1, sizeof(Person)*(idx_tab[i].index-k-1));
                memcpy(person_tab+k+1,tmp, sizeof(Person)*no_children);
                for(int l=0;l<no_parents;l++){
                    if(idx_tab[i].index>idx_tab[l].index)
                        idx_tab[l].index+=no_children;
                }

                idx_tab[i].index=k+1;


            }
        }
    }
}

int cleaning(Person *person_tab, int n)
{
    int deleted=0;
    for(int i=0;i<n;i++){
        if(person_tab[i].bits.in_line==no){
            // printf("deleting %s\n",person_tab[i].name);
            memcpy(person_tab+i,person_tab+i+1, sizeof(Person)*(n-i-1));
            deleted++;
            i--;
        }
    }
    return n-deleted;
}

void print_person(const Person *p)
{
    printf("%s\n", p->name);
}

int create_list(Person *person_tab, int n)
{
    qsort(person_tab,n,sizeof(Person),cmp_age);
    qsort(person_tab,n,sizeof(Person),cmp_person);
    // printf("\nafter soring:\n");
    // for(int i=0;i<n;i++){
    //     print_person(&person_tab[i]);
    // }

    Parent idx_tab[n];
    // fill indx_tab
    int no_parents = fill_indices_tab(idx_tab,person_tab,n);

    persons_shifts(person_tab,n,idx_tab,no_parents);
    // printf("\nafter shifting:\n");
    // for(int i=0;i<n;i++){
    //     print_person(&person_tab[i]);
    // }
    n = cleaning(person_tab,n);
    // for(int i=0;i<n;i++){
    //     print_person(&person_tab[i]);
    // }

    return n;
}

int main(void)
{
    Person person_tab[] = {
            {"Charles III", {M, no}, {14, 11, 1948}, "Elizabeth II"},
            {"Anne", {F, yes}, {15, 8, 1950}, "Elizabeth II"},
            {"Andrew", {M, yes}, {19, 2, 1960}, "Elizabeth II"},
            {"Edward", {M, yes}, {10, 3, 1964}, "Elizabeth II"},
            {"David", {M, yes}, {3, 11, 1961}, "Margaret"},
            {"Sarah", {F, yes}, {1, 5, 1964}, "Margaret"},
            {"William", {M, yes}, {21, 6, 1982}, "Charles III"},
            {"Henry", {M, yes}, {15, 9, 1984}, "Charles III"},
            {"Peter", {M, yes}, {15, 11, 1977}, "Anne"},
            {"Zara", {F, yes}, {15, 5, 1981}, "Anne"},
            {"Beatrice", {F, yes}, {8, 8, 1988}, "Andrew"},
            {"Eugenie", {F, yes}, {23, 3, 1990}, "Andrew"},
            {"James", {M, yes}, {17, 12, 2007}, "Edward"},
            {"Louise", {F, yes}, {8, 11, 2003}, "Edward"},
            {"Charles", {M, yes}, {1, 7, 1999}, "David"},
            {"Margarita", {F, yes}, {14, 5, 2002}, "David"},
            {"Samuel", {M, yes}, {28, 7, 1996}, "Sarah"},
            {"Arthur", {M, yes}, {6, 5, 2019}, "Sarah"},
            {"George", {M, yes}, {22, 7, 2013}, "William"},
            {"George VI", {M, no}, {14, 12, 1895}, NULL},
            {"Charlotte", {F, yes}, {2, 5, 2015}, "William"},
            {"Louis", {M, yes}, {23, 4, 2018}, "William"},
            {"Archie", {M, yes}, {6, 5, 2019}, "Henry"},
            {"Lilibet", {F, yes}, {4, 6, 2021}, "Henry"},
            {"Savannah", {F, yes}, {29, 12, 2010}, "Peter"},
            {"Isla", {F, yes}, {29, 3, 2012}, "Peter"},
            {"Mia", {F, yes}, {17, 1, 2014}, "Zara"},
            {"Lena", {F, yes}, {18, 6, 2018}, "Zara"},
            {"Elizabeth II", {F, no}, {21, 4, 1925}, "George VI"},
            {"Margaret", {F, no}, {21, 8, 1930}, "George VI"},
            {"Lucas", {M, yes}, {21, 3, 2021}, "Zara"},
            {"Sienna", {F, yes}, {18, 9, 2021}, "Beatrice"},
            {"August", {M, yes}, {9, 2, 2021}, "Eugenie"}};

    int to_do, no;
    int n;
    Food food_tab[FOOD_MAX];
    char buff[ART_MAX];
    fgets(buff, ART_MAX, stdin);
    sscanf(buff, "%d", &to_do);

    switch (to_do)
    {
        case 1: // bsearch2
            fgets(buff, ART_MAX, stdin);
            sscanf(buff, "%d", &no);
            n = read_goods(food_tab, no, stdin, 1);
            scanf("%s", buff);
            print_art(food_tab, n, buff);
            break;
        case 2: // qsort
            fgets(buff, ART_MAX, stdin);
            sscanf(buff, "%d", &no);
            n = read_goods(food_tab, no, stdin, 0);
            Date curr_date;
            int days;
            scanf("%d %d %d", &curr_date.day, &curr_date.month, &curr_date.year);
            scanf("%d", &days);
            printf("%.2f\n", value(food_tab, (size_t)n, curr_date, days));
            break;
        case 3: // succession
            scanf("%d", &no);
            int no_persons = sizeof(person_tab) / sizeof(Person);
            create_list(person_tab, no_persons);
            print_person(person_tab + no - 1);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
    }
    return 0;
}