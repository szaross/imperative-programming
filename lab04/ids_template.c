#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

#define MAX_LEN 10240

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char identifiers[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int find_idents() {
    char input[MAX_LEN];
    int start=0;
    int id_count=0;
    int block_com=0;
    while (1) {
        if (fgets(input, MAX_LEN, stdin) == NULL) break;
        for(int i=0; i+1<MAX_LEN;i++){
            if(input[i]=='\0') break;
            if(block_com==1){
                while(!(input[i]=='*' && input[i+1]=='/')) {
                    i++;
                    if (i + 1 >= MAX_LEN) break;
                }
                if(i+1!=MAX_LEN) block_com=0;
                else break;
                i+=2;
            }
            if(input[i]=='/' && input[i+1]=='/') break;
            if(input[i]=='/' && input[i+1]=='*'){
                block_com=1;
                i+=2;
                while(!(input[i]=='*' && input[i+1]=='/')) {
                    i++;
                    if(i+1==MAX_LEN) break;
                }
                if(i+1!=MAX_LEN) block_com=0;
                else break;
                i+=2;
            }
            if(input[i]=='"'){
                i++;
                while(input[i]!='"') {
                    if(input[i]=='\\'){
                        i++;
                        while(isalnum(input[i])) {
                            i++;
                            if(i+1>=MAX_LEN) break;
                        }
                    }
                    i++;
                    if(i+1>=MAX_LEN) break;
                }
                i++;
            }
            if(input[i]=='\''){
                i+=3;

            }
            if(input[i]=='\\' && isalpha(input[i+1])){
                i+=2;
                continue;
            }
            if(isalpha(input[i]) || input[i]=='_'){
                start=i;
                while(isalnum(input[i+1]) || input[i+1]=='_') i++;

                strncpy(identifiers[id_count],input+start,i-start+1);
                id_count++;
            }

        }
    }

    // ZAMIENIC NA QSORT
    char temp[MAX_ID_LEN];
    int length=id_count;

//    for (int i = 0; i < (length - 1); i++)
//    {
//        int j_min = i;
//        for (int j = i + 1; j < length; j++)
//            if (strcmp(identifiers[j], identifiers[j_min]) < 0)
//                j_min = j;
//
//        if (j_min != i)
//        {
//            strcpy(temp, identifiers[i]);
//            strcpy(identifiers[i], identifiers[j_min]);
//            strcpy(identifiers[j_min], temp);
//        }
//    }

    qsort(identifiers,length,sizeof(char),cmp); // NIE DZIALA!

    for(int j=0;j<length;j++) {
        for (int i = 0; identifiers[j][i];i++){
            identifiers[j][i] = tolower(identifiers[j][i]);
        }
}

    char unique[MAX_IDS][MAX_ID_LEN];
    int uniq_len=0;
    for(int j=0;j+1<length;j++) {
        if(strcmp(identifiers[j],identifiers[j+1])!=0){
            strcpy(unique[uniq_len],identifiers[j]);
            uniq_len++;
        }
    }

    strcpy(unique[uniq_len],identifiers[length-1]);
    uniq_len++;

    int res=uniq_len;

    for(int i=0;i<32;i++) {
        for(int j=0;j<uniq_len;j++){
            if(strcmp(keywords[i],unique[j])==0) {
                res--;
            }
        }
    }

//    for(int i=0;i<uniq_len;i++){
//        printf("%s\n",unique[i]);
//    }
    return res;
}

int cmp(const void* first_arg, const void* second_arg) {
    printf("here");
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
    printf("%c %c",*a,*b);
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
    printf("1");
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(identifiers[a], identifiers[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}