#include <stdio.h>
#include <stdlib.h>

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
|        DFS        |
|------------------*/
void DFS_AUX(list* l, int i, int *visited){
    visited[i] = 1;

    list_node* n = l[i].first;
    while(n != NULL){
        if (visited[n->data] == 0) DFS_AUX(l,n->data,visited);
        n = n->next;
    }

    visited[i] = 2;
}

int DFS(list* l, int n){
    int* visited = (int*)malloc(n*sizeof(int));
    int answ = 1;

    for (int i = 0; i < n; i++){
        visited[i] = 0;
    }

    DFS_AUX(l,0,visited);
    for(int i = 0; i < n; i++){
        if (visited[i] == 0) answ = 0;
    }

    free(visited);

    return answ;
}



/*=================|
|       Main       |
|=================*/
int main(){
    int n, m;
    int a, b, d;

    (void)scanf(" %i", &n);
    (void)scanf(" %i", &m);

    list* edges = (list*)malloc(m*sizeof(list));
    list* t_edges = (list*)malloc(m*sizeof(list));

    for (int i = 0; i < m; i++){
        init(&edges[i]);
        init(&t_edges[i]);
    }

    for (int i = 0; i < m; i++){
        (void)scanf(" %i", &a);
        (void)scanf(" %i", &b);
        (void)scanf(" %i", &d);

        insert(&edges[a],b);
        insert(&t_edges[b],a);
        if (d == 2) {
            insert(&edges[b],a);
            insert(&t_edges[a],b);
        }
    }

    if (DFS(edges,n) && DFS(t_edges,n)) printf("Adequado.\n");
    else printf("Inadequado.\n");

    for (int i = 0; i < m; i++){
        destroy(&edges[i]);
        destroy(&t_edges[i]);
    }
    free(edges);
    free(t_edges);

    return 0;
}