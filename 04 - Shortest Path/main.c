#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define max 10000

/*------------------|
|       List        |
|------------------*/
typedef struct _list_node {
    struct _list_node *next;    
    int data;
    int weight;
} list_node;

typedef struct _list {
    list_node* first;
} list;

void init(list* l){
    l->first = NULL;
}

int empty(list* l){
    return l->first == NULL;
}

void destroy(list* l){
    while(!empty(l)){
        list_node *aux = l->first;
        l->first = aux->next;
        free(aux);
    }
}

void insert(list* l, int i, int w){
    list_node *n = (list_node*)malloc(sizeof(list_node));
    n->data = i;
    n->weight = w;

    list_node *aux = l->first;
    l->first = n;
    n->next = aux;
}

/*------------------|
|   Priority Queue  |
|------------------*/
typedef struct _fibheap_node {
    int value;
    int key;
    int degree;
    struct _fibheap_node *left;
    struct _fibheap_node *right;
    struct _fibheap_node *parent;
    struct _fibheap_node *child;
    int mark;
    int visited;
} fibheap_node;

typedef struct _fibheap {
  int n;
  fibheap_node *min;
  int phi;
  int degree;
} fibheap;

fibheap* make_heap(){
    fibheap* q = (fibheap*)malloc(sizeof(fibheap));
    q->n = 0;
    q->degree = 0;
    q->min = NULL;
    q->phi = 0;

    return q;
}

int fibheap_empty(fibheap* q){
    return q->n == 0;
}

void fibheap_insert(fibheap* q, int value, int key){
    //Allocation of new node
    fibheap_node* n = (fibheap_node*)malloc(sizeof(fibheap_node));
    n->value = value;
    n->key = key;
    n->degree = 0;

    n->child = NULL;
    n->parent = NULL;
    n->visited = 0;
    n->mark = 0;

    n->left = n;
    n->right = n;

    //If heap empty then new node is min
    if (q->min == NULL) q->min = n;
    //Inserting into root list
    else{
        q->min->left->right = n;
        n->right = q->min;
        n->left = q->min->left;
        q->min->left = n;

        //Updating min pointer
        if (n->key < q->min->key) q->min = n;
    }
    q->n++;
}

int cal_degree(int n) {
    int count = 0;
    while (n > 0) {
        n = n / 2;
        count++;
    }
    return count;
}

void fib_heap_link(fibheap *q, fibheap_node *a, fibheap_node *b) {
    //Remove a from root list
    a->right->left = a->left;
    a->left->right = a->right;

    //Only b left in root, then b is min node
    if (b->right == b) q->min = b;

    //A parent is B
    a->left = a;
    a->right = a;
    a->parent = b;

    //Case of B with no child
    if (b->child == NULL) b->child = a;

    //Insert A into B child list
    a->right = b->child;
    a->left = b->child->left;
    b->child->left->right = a;
    b->child->left = a;

    //Update child pointer of B
    if (a->key < b->child->key) b->child = a;

    b->degree++;
}

void consolidate(fibheap *q) {
    int degree, d;
    degree = cal_degree(q->n);
    fibheap_node **A;
    A = (fibheap_node**)malloc((degree+1)*sizeof(fibheap_node*));

    for (int i = 0; i <= degree; i++) {
        A[i] = NULL;
    }

    fibheap_node*x, *y;
    x = q->min;
    do {
        d = x->degree;
        while (A[d] != NULL && A[d]->value != x->value) {
            y = A[d];
            if (x->key > y->key) {
                fibheap_node *aux = x;
                x = y;
                y = aux;
            }

            if (y == q->min) q->min = y->right;
            fib_heap_link(q, y, x);

            A[d] = NULL;
            d++;
        }
        A[d] = x;
        x = x->right;
    } while (x != q->min);

    q->min = NULL;
    for (int i = 0; i <= degree; i++){
        if (A[i] != NULL) {
            //Add A[i] to root list
            A[i]->left = A[i];
            A[i]->right = A[i];
            if (q->min == NULL) q->min = A[i];
            else {
                q->min->left->right = A[i];
                A[i]->right = q->min;
                A[i]->left = q->min->left;
                q->min->left = A[i];

                //Update min
                if (A[i]->key < q->min->key) q->min = A[i];
            }
        }
    }

    free(A);
}

fibheap_node *extract_min(fibheap *q) {
    fibheap_node *z = q->min;
    if (z != NULL) {
        if (z->child != NULL) {
            fibheap_node *pntr = z;
            fibheap_node *x = z->child;
            do{
                pntr = x->right;

                //Inserting X into root list
                (q->min->left)->right = x;
                x->right = q->min;
                x->left = q->min->left;
                q->min->left = x;

                //Update min pointer
                if (x->key < q->min->key) q->min = x;

                x->parent = NULL;
                x = pntr;
            } while (pntr != z->child);
        }

        //Remove Z from root list
        z->left->right = z->right;
        z->right->left = z->left;
        q->min = z->right;

        if (z == z->right) q->min = NULL;
        else {
            q->min = z->right;
            consolidate(q);
        }
        q->n = q->n - 1; 
    }
    return z;
}

void cut(fibheap *q, fibheap_node *node, fibheap_node *parent_node) {
    //If only child then childs is null
    if (node == node->right) parent_node->child = NULL;

    //Remove node from parent child list
    node->left->right = node->right;
    node->right->left = node->left;
    if (node == parent_node->child) parent_node->child = node->right;
    parent_node->degree--;

    //Add node to root list
    node->left = node;
    node->right = node;
    q->min->left->right = node;
    node->right = q->min;
    node->left = q->min->left;
    q->min->left = node;

    node->parent = NULL;
    node->mark = 0;
}

void cascading_cut(fibheap *q, fibheap_node *parent_node) {
    fibheap_node *aux = parent_node->parent;
    if (aux != NULL) {
        if (parent_node->mark == 0) parent_node->mark = 1;
        else {
            cut(q, parent_node, aux);
            cascading_cut(q, aux);
        }
    }
}

void decrease_key(fibheap *q, fibheap_node *node, int new_key) {
    if (q == NULL || node == NULL || node->key < new_key) return;
    node->key = new_key;
    fibheap_node *parent_node = node->parent;
    if ((parent_node != NULL) && (node->key < parent_node->key)) {
        cut(q, node, parent_node);
        cascading_cut(q, parent_node);
    }
    if (node->key < q->min->key) q->min = node;
}

void find_and_decrease(fibheap *q, fibheap_node *n, int value, int new_key) {
    n->visited = 1;

    if (n->value == value){
        n->visited = 0;
        decrease_key(q, n, new_key);
    }else{
        if (n->child != NULL) find_and_decrease(q, n->child, value, new_key);
        if (n->right->visited != 1) find_and_decrease(q, n->right, value, new_key);

        n->visited = 0;
    }
}

int* dijsktra(fibheap* Q,list* l, int n, int k){
    int* dist = (int*)malloc(n*sizeof(int));
    int* pred = (int*)malloc(n*sizeof(int));

    for (int i = 0; i < n; i++){
        if (i < k){
            dist[i] = 0;
            fibheap_insert(Q,i,0);
        }
        else{
            dist[i] = max;
            fibheap_insert(Q,i,max);
        }
        pred[i] = max;
    }

    while(!fibheap_empty(Q)){
        fibheap_node* aux = extract_min(Q);
        list_node* aux2 = l[aux->value].first;
        while(aux2 != NULL){
            int alt = dist[aux->value] + aux2->weight;
            if (alt < dist[aux2->data]){
                pred[aux2->data] = aux->value;
                dist[aux2->data] = alt;
                find_and_decrease(Q,Q->min,aux2->data,alt);
            } else if (alt == dist[aux2->data]){
                int current = pred[aux2->data];
                while(current >= k) current = pred[current];

                int new = pred[aux->value];
                while(new >= k) new = pred[new];

                if (new < current) {
                    pred[aux2->data] = aux->value;
                    dist[aux2->data] = alt;
                    find_and_decrease(Q,Q->min,aux2->data,alt);
                }
            }
            aux2 = aux2->next;
        }
    }

    return pred;
}

int main(){
    int n, m, k;
    int a, b, w;

    fibheap* Q = make_heap();

    (void)scanf(" %d",&n);
    (void)scanf(" %d",&m);
    (void)scanf(" %d",&k);

    list* l = (list*)malloc(n*sizeof(list));
    for (int i = 0; i < n; i++) {
        init(&l[i]);
    }


    for(int i = 0; i < m; i++){
        (void)scanf(" %d",&a);
        (void)scanf(" %d",&b);
        (void)scanf(" %d",&w);

        insert(&l[a],b,w);
        insert(&l[b],a,w);
    }

    int* pred = dijsktra(Q,l,n,k);

    for (int j = k; j < n; j++){
        int aux = pred[j];
        while(aux >= k) aux = pred[aux];
        printf("%d", aux);
        if (j+1 < n) printf(" ");
    }
    printf("\n");


    return 0;
}