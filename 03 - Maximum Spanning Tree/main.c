#include <stdio.h>
#include <stdlib.h>

/*------------------|
|     Edge List     |
|------------------*/
typedef struct _edge{
    struct _edge *next;
    int a;
    int b;
    int weight;
} edge;

typedef struct _list{
    edge* first;
    int (*cmp)(edge*, edge*);
} list;

int cmp_lexografical(edge* n, edge* m){
    if (n->a > m->a) return 1;
    if (n->a == m->a){
        if (n->b > m->b) return 1;
        else return -1;
    }
    return -1;
}

int cmp_weight(edge* n, edge* m){
    if (n->weight > m->weight) return -1;
    else if (n->weight == m->weight) return cmp_lexografical(n,m);
    return 1;
}


void init(list* l, int (*cmp)(edge*, edge*)){
    l->first = NULL;
    l->cmp = cmp;
}

int empty(list* l){
    return l->first == NULL;
}

void pop(list* l){
    edge* aux = l->first;
    l->first = aux->next;
    free(aux);
}

void destroy(list* l){
    while(!empty(l)){
        pop(l);
    }
}

void insert(list* l, int a, int b, int w){
    edge *n = (edge*)malloc(sizeof(edge));
    n->a = a;
    n->b = b;
    n->weight = w;

    if (empty(l)) {
        l->first = n;
        n->next = NULL;
        return;
    }

    edge *sHead = l->first;
    edge *sLast = NULL;
    while (sHead != NULL && l->cmp(n,sHead) >= 0) {
        sLast = sHead;
        sHead = sHead->next;
    }

    if (sLast == NULL){
        l->first = n;
        n->next = sHead;
    }
    else{
        sLast->next = n;
        n->next = sHead;
    }
}

void list_print(list* l){
    edge *n = l->first;
    while(n != NULL){
        printf("%d %d\n",n->a,n->b);
        n = n->next;
    }
}


/*------------------|
|     Union-Find    |
|------------------*/
typedef struct _list_node{
    struct _list_node* parent;
    int data;
    int rank;
} list_node;

list_node* make_set(int data){
    list_node* n = (list_node*)malloc(sizeof(list_node));
    n->data = data;
    n->rank = 1;
    n->parent = NULL;

    return n;
}

list_node* find(list_node* i){
    if(i->parent == NULL) return i;
    else {
        list_node* parent = find(i->parent);
        i->parent = parent;
        return parent;
    }
}

void _union(list_node* a, list_node* b){
    if (a->rank < b->rank) a->parent = b;
    else if(a->rank > b->rank) b->parent = a;
    else {
        a->parent = b;
        b->rank++;
    }
}


/*------------------|
|      Kruskal      |
|------------------*/
void kruskal(int n,list* weight_list){
    list_node* vertices = (list_node*)malloc(n*sizeof(list_node));
    list* edges_order = (list*)malloc(sizeof(list));
    init(edges_order,&cmp_lexografical);

    for (int i = 0; i < n; i++){
        vertices[i] = *make_set(i);
    }
    
    while(!empty(weight_list)){
        int a = weight_list->first->a;
        int b = weight_list->first->b;

        list_node* a_parent = find(&vertices[a]);
        list_node* b_parent = find(&vertices[b]);

        if (a_parent != b_parent){
            _union(a_parent,b_parent);
            insert(edges_order,a,b,0);
        }

        pop(weight_list);
    }

    list_print(edges_order);

    free(vertices);
    free(edges_order);
}

int main(){
    int n, m;
    int a, b, w;

    (void)scanf(" %i", &n);
    (void)scanf(" %i", &m);

    list* weight_list = (list*)malloc(sizeof(list));
    init(weight_list,&cmp_weight);

    for(int i = 0; i < m; i++){
        (void)scanf(" %i", &a);
        (void)scanf(" %i", &b);
        (void)scanf(" %i", &w);

        insert(weight_list,a,b,w);
    }
    kruskal(n, weight_list);

    free(weight_list);

    return 0;
}