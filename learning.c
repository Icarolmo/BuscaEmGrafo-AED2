#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0
typedef int bool;
typedef int TIPOPESO;


typedef struct adjacencia
{
    int vertice;
    TIPOPESO id;
    struct adjacencia *prox;
} ADJACENCIA;

typedef struct vertice 
{
    int flag;
    ADJACENCIA *cab;
} VERTICE;

typedef struct grafo {
    int vertices;
    int arestas;
    VERTICE *adj;
} GRAFO;


typedef struct registro {
    int chave;
} REGISTRO;

typedef struct elemento {
    REGISTRO reg;
    struct elemento *prox;
} ELEMENTO, *PONT;

typedef struct fila {
    PONT inicio;
    PONT fim;
} FILA;

void inicializarFila(FILA* fila){
    fila->inicio = NULL;
    fila->fim = NULL;
}

GRAFO *criaGrafo(int v){
    GRAFO *g = malloc(sizeof(GRAFO));
    g->vertices = v;
    g->arestas = 0;
    g->adj = malloc(v*sizeof(VERTICE));
    int i;
    for (i=0; i<v; i++) {
        g->adj[i].cab = NULL;
        g->adj[i].flag = 0;
    }
    return g;
}



ADJACENCIA *criaAdj(int v, int peso){
    ADJACENCIA *temp = malloc (sizeof(ADJACENCIA));
    temp->vertice = v;
    temp->peso = peso;
    temp->prox = NULL;
    return temp;
}



bool criaAresta(GRAFO *grafo, int vi, int vf, TIPOPESO p){
    if (!grafo) return false;
    if((vf < 0) || (vf >= grafo->vertices)) return false;
    if((vi < 0) || (vi >= grafo->vertices)) return false;

    ADJACENCIA *novo = criaAdj(vf,p);
    novo->prox = grafo->adj[vi].cab;
    grafo->adj[vi].cab = novo;
    grafo->arestas++;
    return true;    
}



int numeroLacos(GRAFO *grafo){
    int i;
    int numeroLacos = 0;
    for(i=0; i<grafo->vertices; i++){
        ADJACENCIA *adj = grafo->adj[i].cab;
        while(adj){
            if(adj->vertice == i){
                numeroLacos++;
            }
            adj = adj->prox;
        }
    }
    return numeroLacos;
}



void removeLacos(GRAFO *grafo){
    int i;
    for(i=0; i<grafo->vertices; i++){
        ADJACENCIA *adj = grafo->adj[i].cab;
        if (adj->vertice == i){
            grafo->adj[i].cab = adj->prox;
        }
        while(adj->prox){
            if(adj->prox->vertice == i){
                grafo->adj[i].cab = adj->prox->prox;
            }
            adj = adj->prox;
        }
    }
}

void destroiAresta(ADJACENCIA *adj){
    if(adj->prox != NULL){
        destroiAresta(adj->prox);
    }
    free(adj->prox);
}

void varreArestas(GRAFO* grafo){
    int i;
    for(i = 0; i<grafo->vertices; i++){
        if(grafo->adj[i].cab != NULL){
            destroiAresta(grafo->adj[i].cab);
            grafo->adj[i].cab = NULL;
        }
    }
    grafo->arestas = 0;
}



GRAFO *criaTransposto(GRAFO *grafo){
    GRAFO *gTran =  criaGrafo(grafo->vertices);
    int i;
    for(i = 0; i<grafo->vertices; i++){
        ADJACENCIA *adj = grafo->adj[i].cab;
        while(adj){
            criaAresta(gTran, adj->vertice, i, adj->peso);
            adj = adj->prox;
        }
    }
    return (gTran);
}


void realizaBusca(GRAFO *g, int v, bool *explorado){
    FILA fila;
    inicializarFila(&fila);
    explorado[v] = true;
}


void buscaLargura(GRAFO *grafo){
    bool explorado[grafo->vertices];
    int i;
    for(i = 0; i<grafo->vertices; i++){
        explorado[i] = false;
    }
    for(i = 0; i<grafo->vertices; i++){
        if(!explorado[i]){
            realizaBusca(grafo, i, explorado);
        }
    }
}


void realizaBuscaRecursiva(GRAFO *grafo, int i){
    grafo->adj[i].flag = 1;
    ADJACENCIA *adj = grafo->adj[i].cab;
    while(adj){
        if(grafo->adj[adj->vertice].flag == 0){
            realizaBuscaRecursiva(grafo, adj->vertice);
        }
        adj = adj->prox; 
    }
    grafo->adj[i].flag = 2;
}


void buscaProfundidade(GRAFO *grafo){
    int i;
    for (i = 0; i<grafo->vertices; i++){
        if(grafo->adj[i].flag == 0){
            realizaBuscaRecursiva(grafo, i);
        }
    }
}


void imprimeGrafo(GRAFO *grafo){
    printf("Vertices: %d. Arestas: %d.\n\n", grafo->vertices, grafo->arestas);
    int i;
    for (i=0; i<grafo->vertices; i++){
        printf("V%d: ", i);
        ADJACENCIA *adj = grafo->adj[i].cab;
        while(adj){
            printf("V%d(%d) ", adj->vertice, adj->peso);
            adj = adj->prox;
        }
        printf("\n");
    }
    printf("\n");
}



void main(){
    int numeroVertices = 7;
    GRAFO *grafo = criaGrafo(numeroVertices);
    criaAresta(grafo, 0, 1, 1);
    criaAresta(grafo, 1, 2, 1);
    criaAresta(grafo, 2, 3, 1);
    criaAresta(grafo, 3, 4, 1);
    criaAresta(grafo, 4, 6, 0);
    criaAresta(grafo, 6, 0, 1);
    criaAresta(grafo, 0, 5, 1);
    criaAresta(grafo, 5, 3, 1);
    criaAresta(grafo, 6, 5, 1);
    criaAresta(grafo, 6, 4, 1);
    criaAresta(grafo, 5, 5, 1);
    criaAresta(grafo, 1, 1, 1);
    criaAresta(grafo, 2, 2, 1);
    criaAresta(grafo, 2, 2, 1);
    criaAresta(grafo, 4, 4, 1);
    imprimeGrafo(grafo);

    /* Exercicio 1
    int lacos = numeroLacos(grafo);
    printf("Número de Lacos no grafo: %d\n\n", lacos);

    */

    /* Exercicio 2:
    if(lacos>0) removeLacos(grafo);
    int remLacos = numeroLacos(grafo);
    printf("Verifica remoção de Lacos no grafo: %d\n\n", remLacos);

    */

    /* Exercicio 3:
    varreArestas(grafo);
    imprimeGrafo(grafo);

    */


   /* Exercício 4:
   GRAFO *grafoTransposto = criaTransposto(grafo);
   imprimeGrafo(grafoTransposto);

    */

}

