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
void DFS_AUX(list* l, int i, int *visited,int *color, int paint, int *f_bipart){
    visited[i] = 1;
    color[i] = paint;

    list_node* n = l[i].first;
    while(n != NULL){
        if (visited[n->data] == 0) DFS_AUX(l,n->data,visited,color,-paint,f_bipart);
        else if (visited[n->data] == 1 && color[n->data] == color[i]) *f_bipart = 0;
        n = n->next;
    }

    visited[i] = 2;
}

int DFS(list* l, int n){
    int* visited = (int*)malloc(n*sizeof(int));
    int* color = (int*)malloc(n*sizeof(int));

    int f_bipart = 1;

    for (int i = 0; i < n; i++){
        visited[i] = 0;
        color[i] = 0;
    }

    for(int i = 0; i < n; i++){
        if (visited[i] == 0) {
            DFS_AUX(l,i,visited,color, 1, &f_bipart);
        }
    }

    free(visited);
    free(color);

    return f_bipart;
}



/*=================|
|       Main       |
|=================*/
int main(){
    int n, m;
    int a, b;

    (void)scanf(" %i", &n);
    (void)scanf(" %i", &m);

    list* edges = (list*)malloc(m*sizeof(list));

    for (int i = 0; i < m; i++){
        init(&edges[i]);
    }

    for (int i = 0; i < m; i++){
        (void)scanf(" %i", &a);
        (void)scanf(" %i", &b);

        insert(&edges[a],b);
        insert(&edges[b],a);
    }

    if (DFS(edges,n)) printf("Eh possivel produzir com duas linhas de producao.\n");
    else printf("Nao eh possivel produzir com duas linhas de producao.\n");

    for (int i = 0; i < m; i++){
        destroy(&edges[i]);
    }
    free(edges);

    return 0;
}