#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 10000

void print_matrix(int** a, int n, int m){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            printf("%d ",a[i][j]);
        }
        printf("\n");
    }
}

/*------------------|
|       Queue       |
|------------------*/
typedef struct _queue_node{
    int value;
    struct _queue_node* next;
} queue_node;

typedef struct _queue{
    queue_node* first;
    queue_node* last;
    int size;
} queue;


void init_queue(queue* q){
    q->first = NULL;
    q->last = NULL;
    q->size = 0;
}

int empty_queue(queue* q){
    return q->size == 0;
}

void push(queue* q, int i){
    queue_node* n = (queue_node*)malloc(sizeof(queue_node));

    n->value = i;
    n->next = NULL;

    if(empty_queue(q)){
        q->first = n;
        q->last = n;
        q->size++;
        return;
    }

    q->last->next = n;
    q->last = n;
    q->size++;
}

queue_node* pop(queue* q){
    queue_node* n = q->first;
    q->first = n->next;
    q->size--;
    return n;
}

/*-----------------|
|   Edmonds-Karp   |
|-----------------*/
int max_flow(int** g, int** cap, int n, int s, int t){
    int flow = 0;

    do{
        queue* q = (queue*)malloc(sizeof(queue));
        init_queue(q);
        push(q,s);
        int* pred = (int*)malloc(n*sizeof(int));
        for (int i = 0; i < n; i++){ pred[i] = -1; }

        while(empty_queue(q) != 1 && pred[t] == -1){
            queue_node* cur = pop(q);
            for(int i = 0; i < n; i++){
                if (pred[i] == -1 &&  i != s && cap[cur->value][i] > g[cur->value][i]){
                    pred[i] = cur->value;
                    push(q,i);
                }
            }
        }

        if(pred[t] == -1) break;

        int df = MAX;
        int f = t;
        int e = pred[t];
        while (e != -1){
            df = fmin(df, cap[e][f] - g[e][f]);

            f = e;
            e = pred[f];
        }


        f = t;
        e = pred[t];
        while (e != -1){
            g[e][f] += df;
            g[f][e] +- df;

            f = e;
            e = pred[f];
        }

        flow += df;


    } while(1);


    return flow;
}

int main(){
    int n, k, m;

    (void)scanf(" %d",&n);
    (void)scanf(" %d",&k);
    (void)scanf(" %d",&m);

    int** g = (int**)malloc((n+k+2)*sizeof(int*));
    int** cap = (int**)malloc((n+k+2)*sizeof(int*));

    int s = n+k;
    int t = n+k+1;

    for (int i = 0; i < (n+k+2); i++){
        g[i] = (int*)malloc((n+k+2)*sizeof(int));
        cap[i] = (int*)malloc((n+k+2)*sizeof(int));
        for (int j = 0; j < (n+k+2); j++){
            g[i][j] = -1;
            cap[i][j] = -1;
        }
    }

    for (int i = 0; i < n; i++){
        cap[s][i] = 1;
        g[i][s] = 1;
        g[s][i] = 0;
    }

    for (int i = n; i < n+k; i++){
        cap[i][t] = 1;
        g[t][i] = 1;
        g[i][t] = 0;
    }

    for(int i = 0; i < m; i++){
        int a, b;

        (void)scanf(" %d",&a);
        (void)scanf(" %d",&b);

        cap[a][b] = MAX;
        g[b][a] = MAX;
        g[a][b] = 0;
    }

    int max = max_flow(g,cap,(n+k+2),s,t);

    printf("%d\n",max);
    
    return 0;
}