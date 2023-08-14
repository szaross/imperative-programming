    #include <stdio.h>

    #define STACK_SIZE 10

    enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };

    int stack[STACK_SIZE];
    int top = 0;

    int stack_push(int x) {
        if(top==STACK_SIZE) return OVERFLOW;
        stack[top]=x;
        top++;
        return OK;
    }

    int stack_pop(void) {
        if(top==0) return UNDERFLOW;
        top--;
        int res=stack[top];
        return res;
    }

    int stack_state(void) {
        return top;
    }

    // FIFO queue with shifts

    #define QUEUE_SIZE 10

    int queue[QUEUE_SIZE];
    int in = 0, curr_nr = 0;

    int queue_push(int in_nr) { // in_nr clients try to enter the queue
        int b_in=in;
        for(int i=0;i<in_nr;i++){
            curr_nr++;
            if(in<10) {
                queue[in]=curr_nr;
                in++;
            }
        }
        if(b_in+in_nr>10) return OVERFLOW;
        return OK;
    }

    int queue_pop(int out_nr) {
        if(out_nr>in){
            in=0;
            return UNDERFLOW;
        }
        else if(out_nr==in){
            in=0;
            return in;
        }
        for(int i=0;i+out_nr<10;i++){
            queue[i]=queue[i+out_nr];
        }
        in-=out_nr;
        return in;

    }

    int queue_state(void) {
        return in;
    }

    void queue_print(void) {
        for(int i=0;i<in;i++){
            printf("%d ",queue[i]);
        }
    }

    // Queue with cyclic buffer

    #define CBUFF_SIZE 10

    int cbuff[CBUFF_SIZE];
    int out = 0, len = 0;


    int cbuff_push(int cli_nr) {
        if(len==CBUFF_SIZE) return OVERFLOW;
        cbuff[(out+len)%CBUFF_SIZE]=cli_nr;
        len++;
        return OK;
    }

    int cbuff_pop(void) {
        if(len==0) return UNDERFLOW;
        int num=cbuff[out];
        out=(out+1)%CBUFF_SIZE;
        len--;
        return num;
    }

    int cbuff_state(void) {
        return len;
    }

    void cbuff_print(void) {
        for(int i=0;i<len;i++){
            printf("%d ",cbuff[(out+i)%CBUFF_SIZE]);
        }
    }

    int main(void) {
        int to_do, n, client_no, answer;
        scanf("%d", &to_do);
        switch(to_do) {
            case 1: // stack
                do {
                    scanf("%d", &n);
                    if (n > 0) {
                        if ((answer = stack_push(n)) < 0) printf("%d ", answer);
                    } else if (n < 0) {
                        printf("%d ", stack_pop());
                    } else printf("\n%d\n", stack_state());
                } while(n != 0);
                break;
            case 2: // FIFO queue with shifts
                do {
                    scanf("%d", &n);
                    if (n > 0) {
                        if ((answer = queue_push(n)) < 0) printf("%d ", answer);
                    } else if (n < 0) {
                        if ((answer = queue_pop(-n)) < 0) printf("%d ", answer);
                    } else {
                        printf("\n%d\n", queue_state());
                        queue_print();
                    }
                } while(n != 0);
                break;
            case 3: // queue with cyclic buffer
                client_no = 0;
                do {
                    scanf("%d", &n);
                    if (n > 0) {
                        if ((answer = cbuff_push(++client_no)) < 0) printf("%d ", answer);
                    } else if (n < 0) {
                        printf("%d ", cbuff_pop());
                    } else {
                        printf("\n%d\n", cbuff_state());
                        cbuff_print();
                    }
                } while(n != 0);
                break;
            default:
                printf("NOTHING TO DO!\n");
        }
        return 0;
    }
