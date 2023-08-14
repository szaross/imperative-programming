#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    if (count[va] == count[vb]) return va - vb;	return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    // sort according to second char if counts and the first char equal
    if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
    // sort according to first char if counts equal
    if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
    return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
    *nl = 0;
    *nw = 0;
    *nc = 0;
    char input[128];
    int newline=1;
    while (1) {
        if(fgets(input, 128, stdin)==NULL) break;
        for (int i=0;i<128;i++) {
            if (input[i] == '\0') break;
            else if (input[i] == '\n') {
                (*nl)++;
                (*nc)++;
                newline=1;
                if(i!=0){
                    (*nw)++;
                }
            }
            else if (input[i]==' '){
                int j=i;
                while(input[j]==' '){
                    j++;
                }
                if(j<127){
                    (*nc)++;
                    if(newline==0) (*nw)++;
                }
                else break;

            }
            else if (input[i]=='\t'){
                int j=i;
                while(input[j]=='\t'){
                    (*nc)++;
                    j++;
                }
                if(j<127){
                    if(newline==0) (*nw)++;
                }
                else break;

            }
            else {
                (*nc)++;
                newline=0;

            }
        }
    }
}


void char_count(int char_no, int *n_char, int *cnt) {
    int occur[LAST_CHAR-FIRST_CHAR]={0};
    char input[128];

    while (1) {
        if(fgets(input, 128, stdin)==NULL) break;
        for (int i=0; i<128; i++) {
            if (input[i] == '\0') break;
            occur[input[i]-FIRST_CHAR]++;
        }
    }

    int chars[LAST_CHAR-FIRST_CHAR];
    for(int i=0;i<LAST_CHAR-FIRST_CHAR;i++) chars[i]=i;

    for(int i=LAST_CHAR-FIRST_CHAR-1;i>-1;i--){
        for(int j=0;j+1<i;j++){
            if(occur[j]<occur[j+1]){
                int tmp=occur[j+1];
                occur[j+1]=occur[j];
                occur[j]=tmp;

                tmp=chars[j+1];
                chars[j+1]=chars[j];
                chars[j]=tmp;

            }
        }
    }

    *n_char=(chars[char_no-1]+FIRST_CHAR);
    *cnt=occur[char_no-1];

}

void bigram_count(int bigram_no, int bigram[]) {
    int occur[LAST_CHAR-FIRST_CHAR][LAST_CHAR-FIRST_CHAR]={};


    char input[128];

    while (1) {
        if(fgets(input, 128, stdin)==NULL) break;
        for (int i=0; i+1<128; i++) {
            if (input[i] == '\0') break;
            else if (!(isalpha(input[i]) && isalpha(input[i+1]))) continue;
            else occur[input[i]-FIRST_CHAR][input[i+1]-FIRST_CHAR]++;
        }
    }

    int max=0;
    int c1,c2;
    for (int i=0;i<bigram_no;i++){
        for (int m=0;m<LAST_CHAR-FIRST_CHAR;m++){
            for (int n = 0; n < LAST_CHAR-FIRST_CHAR; n++) {
                if (occur[m][n]>max){
                    max=occur[m][n];
                    c1=m;
                    c2=n;
                }
            }
        }
        if(i!=bigram_no-1) {
            occur[c1][c2]=0;
            max=0;
        }
    }




    bigram[0]=c1+FIRST_CHAR;
    bigram[1]=c2+FIRST_CHAR;
    bigram[2]=max;
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
    *line_comment_counter=0;
    *block_comment_counter=0;
    char input[128];
    int blok=0,cudz=0;
    while (1) {
        if(fgets(input, 128, stdin)==NULL) break;
        for(int i=0;i+1<128;i++){
            if (input[i] == '\0') break;
            if (cudz==1){
                if (input[i]=='"'){
                    cudz=0;
                }
                continue;
            }
            else if(blok==1) {
                while(!(input[i]=='*' && input[i+1]=='/')) {
                    i++;
                    if (input[i] == '\0') break;
                    if (i + 1 >= 128) break;
                }
                if(i+1!=128 && input[i] != '\0') {
                    blok=0;
                    count[0]++;
                    i+=1;
                }
                else break;

            }
            else{
                if (input[i]=='"'){
                    cudz=1;
                    continue;
                }
                if (input[i]=='/' && input[i+1]=='/'){
                    count[1]++;
                    break;
                }
                else if (input[i]=='/' && input[i+1]=='*'){
                    blok=1;
                    i+=1;
                }
            }
        }
    }

    *line_comment_counter=count[1];
    *block_comment_counter=count[0];

}

#define MAX_LINE 128

int read_int() {
    char line[MAX_LINE];
    fgets(line, MAX_LINE, stdin); // to get the whole line
    return (int)strtol(line, NULL, 10);
}

int main(void) {
    int to_do;
    int nl, nw, nc, char_no, n_char, cnt;
    int line_comment_counter, block_comment_counter;
    int bigram[3];

    to_do = read_int();
    switch (to_do) {
        case 1: // wc()
            wc (&nl, &nw, &nc);
            printf("%d %d %d\n", nl, nw, nc);
            break;
        case 2: // char_count()
            char_no = read_int();
            char_count(char_no, &n_char, &cnt);
            printf("%c %d\n", n_char, cnt);
            break;
        case 3: // bigram_count()
            char_no = read_int();
            bigram_count(char_no, bigram);
            printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
            break;
        case 4:
            find_comments(&line_comment_counter, &block_comment_counter);
            printf("%d %d\n", block_comment_counter, line_comment_counter);
            break;
        default:
            printf("NOTHING TO DO FOR %d\n", to_do);
            break;
    }
    return 0;
}