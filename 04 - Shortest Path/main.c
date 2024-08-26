#include <stdio.h>
#include <stdlib.h>

#define max 10000

/*------------------|
|       List        |
|------------------*/
typedef struct _list_node {
    struct _list_node *next;    
    int data;
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

void insert(list* l, int i){
    list_node *n = (list_node*)malloc(sizeof(list_node));
    n->data = i;

    list_node *aux = l->first;
    l->first = n;
    n->next = aux;
}

/*------------------|
|   Priority Queue  |
|------------------*/
typedef struct _queue_node {
    int v;
    int key;
    int degree;
    struct _queue_node *left;
    struct _queue_node *right;
    struct _queue_node *parent;
    struct _queue_node *child;
    int mark;
    int visited;
} queue_node;

typedef struct _queue {
  int n;
  queue_node *min;
  int phi;
  int degree;
} queue;

queue* make_heap(){
    queue* q = (queue*)malloc(sizeof(queue));
    q->n = 0;
    q->degree = 0;
    q->min = NULL;
    q->phi = 0;
}

int empty(queue* q){
    return q->n == 0;
}

void insert(queue* q, int i, int prio){
    queue_node* n = (queue_node*)malloc(sizeof(queue_node));
    n->v = i;
    n->key = prio;
    n->degree = 0;

    n->child = NULL;
    n->parent = NULL;
    n->visited = 0;

    n->left = n;
    n->right = n;

    if (q->min == NULL) q->min = n;
    else{
        q->min->left->right = n;
        n->right = q->min;
        n->left = q->min->left;
        q->min->left = n;
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

void fib_heap_link(queue *q, queue_node *a, queue_node *b) {
    a->right->left = a->left;
    a->left->right = a->right;

    if (b->right == b) q->min = b;

    a->left = a;
    a->right = a;
    a->parent = b;

    if (b->child == NULL) b->child = a;

    a->right = b->child;
    a->left = b->child->left;
    b->child->left->right = a;
    b->child->left = a;
    if ((a->key) < (b->child->key)) b->child = a;

    (b->degree)++;
}

queue* union_heap(queue *a, queue *b) {
    queue *q = make_fib_heap();
    q->min = a->min;

    queue_node *n = q->min->right;
    queue_node *m = b->min->left;

    q->min->right->left = b->min->left;
    q->min->right = b->min;
    b->min->left = q->min;
    m->right = n;

    if ((a->min == NULL) || (b->min != NULL && b->min->key < a->min->key))
        q->min = b->min;
    q->n = a->n + b->n;
    return q;
}

void consolidate(queue *q) {
    int degree, i, d;
    degree = cal_degree(q->n);
    queue_node **A, *x, *y, *z;
    A = (queue_node*)malloc(degree*sizeof(queue_node));

    for (i = 0; i <= degree; i++) {
        A[i] = NULL;
    }

    x = q->min;
    do {
        d = x->degree;
        while (A[d] != NULL) {
        y = A[d];
        if (x->key > y->key) {
            queue_node *exchange_help = x;
            x = y;
            y = exchange_help;
        }
        if (y == q->min)
            q->min = x;
        fib_heap_link(q, y, x);
        if (y->right == x)
            q->min = x;
        A[d] = NULL;
        d++;
        }
        A[d] = x;
        x = x->right;
    } while (x != q->min);

    q->min = NULL;
    for (i = 0; i < degree; i++) {
        if (A[i] != NULL) {
        A[i]->left = A[i];
        A[i]->right = A[i];
        if (q->min == NULL) {
            q->min = A[i];
        } else {
            q->min->left->right = A[i];
            A[i]->right = q->min;
            A[i]->left = q->min->left;
            q->min->left = A[i];
            if (A[i]->key < q->min->key) {
            q->min = A[i];
            }
        }
        if (q->min == NULL) {
            q->min = A[i];
        } else if (A[i]->key < q->min->key) {
            q->min = A[i];
        }
        }
    }
}

queue_node *extract_min(queue *q) {
    if (q->min == NULL) return q->min;    

    queue_node *n = q->min;
    queue_node *aux;
    aux = n;
    queue_node *x = NULL;

    if (n->child != NULL) {
        x = n->child;
        do {
            aux = x->right;
            (q->min->left)->right = x;
            x->right = q->min;
            x->left = q->min->left;
            q->min->left = x;
            if (x->key < q->min->key)
            q->min = x;
            x->parent = NULL;
            x = aux;
        } while (aux != n->child);
    }

    (n->left)->right = n->right;
    (n->right)->left = n->left;
    q->min = n->right;

    if (n == n->right && n->child == NULL) q->min = NULL;
    else {
        q->min = n->right;
        consolidate(q);
    }

    q->n = q->n - 1;
    return n;
}

void cut(queue *q, queue_node *node, queue_node *parent_node) {
    queue_node *temp_parent_check;

    if (node == node->right) parent_node->child = NULL;

    node->left->right = node->right;
    node->right->left = node->left;

    if (node == parent_node->child) parent_node->child = node->right;
    (parent_node->degree)--;

    node->left = node;
    node->right = node;
    q->min->left->right = node;
    node->right = q->min;
    node->left = q->min->left;
    q->min->left = node;

    node->parent = NULL;
    node->mark = 0;
}

void cascading_cut(queue *q, queue_node *parent_node) {
    queue_node *aux;
    aux = parent_node->parent;
    if (aux != NULL) {
        if (parent_node->mark == 0) {
        parent_node->mark = 1;
        } else {
        cut(q, parent_node, aux);
        cascading_cut(q, aux);
        }
    }
}

void decrease_key(queue *q, queue_node *node, int new_key) {
    queue_node *parent_node;
    if ((q == NULL) || (node = NULL)) return;

    if (node->key >= new_key) {
        node->key = new_key;
        parent_node = node->parent;
        if ((parent_node != NULL) && (node->key < parent_node->key)) {
            cut(q, node, parent_node);
            cascading_cut(q, parent_node);
        }
        if (node->key < q->min->key) {
            q->min = node;
        }
    }
}

void *find_node(queue *q, queue_node *n, int key, int new_key) {
    queue_node *find_use = n;
    queue_node *f = NULL;
    find_use->visited = 1;
    if (find_use->key == key) {
        find_use->visited = 0;
        f = find_use;
        decrease_key(q, f, new_key);
    }
    if (find_use->child != NULL) {
        find_node(q, find_use->child, key, new_key);
    }
    if ((find_use->right->visited != 1)) {
        find_node(q, find_use->right, key, new_key);
    }

    find_use->visited = 0;
}

int* dijsktra(queue* Q,list** l, int n, int k){
    int* dist = (int*)malloc(n*sizeof(int));
    int* pred = (int*)malloc(n*sizeof(int));

    for (int i = 0; i < n; i++){
        if (i < k){
            dist[i] = 0;
            insert(Q,i,0);
        }
        else{
            dist[i] = max;
            insert(Q,i,max);
        }
        pred[i] = -1;
    }

    while(!empty(Q)){
        queue_node* aux = extract_min(Q);
        list_node* aux2 = l[aux->v]->first;
        while(aux2 != NULL){
            int alt = 0;
            if (alt < dist[aux2->data]){
                pred[aux2->data] = aux->v;
                dist[aux2->data] = alt;
                find_node(Q,Q->min,aux->key,alt);
            }
            aux2 = aux2->next;
        }
    }

    return dist;
}

int main(){
    int n, m, k;
    int a, b, w;

    queue* Q = make_heap();

    (void)scanf(" %d",&n);
    (void)scanf(" %d",&m);
    (void)scanf(" %d",&k);

    list** l = (list*)malloc(n*sizeof(list));
    for (int i = 0; i < n; i++) init(l[i]);


    for(int i = 0; i < m; i++){
        (void)scanf(" %d",&a);
        (void)scanf(" %d",&b);
        (void)scanf(" %d",&w);

        insert(l[a],b);
        insert(l[b],a);

        int* dist = dijsktra(Q,l,n,k);    
    }


    return 0;
}