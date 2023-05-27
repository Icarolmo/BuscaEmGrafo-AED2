//--------------------------------------------------------------
// NOMES DOS RESPONSÁVEIS: Icaro Lima de Oliveira e Wesley Gabriel Rosa Fernandes
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int grupo()
{
	return 10;
}

// ######### ESCREVA O NROUSP AQUI
int nroUSP1()
{
	return 13718662;
}

int nroUSP2()
{
	return 13716893;
}

// elemento das listas de adjacência e de resposta - NAO ALTERE ESTA DEFINICAO
typedef struct estr
{
    int adj;  // elemento
    int peso; // custo (não precisa ser usado na resposta)
    struct estr *prox;
} NO;

// vertices do grafo (salas) - use este tipo ao criar o grafo  - NAO ALTERE ESTA DEFINICAO
typedef struct
{
    int flag;   // para uso na busca em largura e profundidade, se necessario
    int aberto; // vale true se a sala em questao esta aberta
    int via;    // use este campo se precisar computar algum caminho etc.
    int dist;   // use este campo se precisar computar alguma distância etc.
    NO *inicio;
} VERTICE;

// Nó que irá conter o vertice e seu respectivo peso na fila - Será um array de NO_FILA com tamanho igual a Nº de vertices, utilizaremos na busca Dijkstra.
typedef struct noFila
{
    int vertice;
    int distancia;
} NO_FILA;

// Cria array de structs NO_FILA com tamanho igual ao parâmetro tamanho e inicializa suas variáveis com valores nulos (vertice = -1 e distancia = 0).
NO_FILA *criaFilaVazia(int tamanho)
{
    NO_FILA *fila = (NO_FILA *) malloc(sizeof(NO_FILA) * tamanho);
    for(int i = 0; i < tamanho; i++)
        fila[i].distancia = 0, fila[i].vertice = -1;   

    return fila;
}

// Verifica se existe algum vertice na FILA - Retorna FALSE se existir um ou mais vertices e TRUE se estiver vazia.
int filaVazia(NO_FILA *fila)
{
    if( fila[0].vertice != -1)
        return 0;
    else
        return 1;
}

// Insere vertice na FILA 
void insereFila(NO_FILA *fila, int vertice, int distancia)
{   
    if (filaVazia(fila))
        fila[0].distancia = distancia, fila[0].vertice = vertice;
    else
    {   
        int procurandoPosicao = 1;
        int cont = 0;
        while(procurandoPosicao)
        {   
            if(fila[cont].vertice == -1)
            {
                fila[cont].distancia = distancia, fila[cont].vertice = vertice;
                procurandoPosicao = 0;
            }
            cont ++;
        }
    }
}

// Pega o vertice de maior prioridade (vertice com menor distancia) na fila. 
int pegaFila(NO_FILA *fila, int tamanho)
{   
    int menor_distancia = 2147483647;
    int indice = 0;
    for(int i = 0; i < tamanho; i++)
    {
        if((fila[i].distancia < menor_distancia) && !(fila[i].vertice == -1))
        {
            indice = i;
            menor_distancia = fila[i].distancia;
        }
    }
    
    int vertice = fila[indice].vertice;

    for(int i = indice + 1; i < tamanho; i++)
    {
        fila[i - 1].distancia = fila[i].distancia, fila[i - 1].vertice = fila[i].vertice;
        fila[i].distancia = 0, fila[i].vertice = -1;
    }

    return vertice;
}

// Realiza abertura de todos os vertices.
void abrirSalas(VERTICE *g, int N)
{
    int i;
    for (i = 0; i < N; i++)
    {
        g[i].aberto = 1;
    }
}

// Inicializa a variável aberto dos vértices com seus valores iniciais (valores passados pelo parâmetro int *aberto recebidos na função caminho).
void inicializarSalas(VERTICE *g, int N, int *aberto)
{
    int i;
    for (i = 0; i < N; i++)
    {
        g[i].aberto = aberto[i];
    }
}

// Cria um grafo em lista de adjacencias já inicializado deixando apenas vias e distancias vazias.
VERTICE *criaGrafoAdj(int v, int a, int *ijpeso, int *aberto) // WESLEY: fiz algumas alterações nessa função, agora os campos adj e peso estão começãndo com valor -1, a função foi testada e funciona
{
    VERTICE *grafo = (VERTICE *)malloc(sizeof(VERTICE) * v); // aloca memória para o grafo

    for (int i = 0; i < v; i++)
    {
        NO *novo_no = (NO *)malloc(sizeof(NO));
        novo_no->adj = (int) NULL, novo_no->peso = (int) NULL, novo_no->prox = NULL;
        grafo[i].inicio = novo_no;
        grafo[i].aberto = aberto[i];
    }

    // percorre o vetor ijpeso para criar as arestas
    for (int i = 0; i < a; i++)
    {
        int origem = ijpeso[i * 3 + 0];
        int destino = ijpeso[i * 3 + 1];
        int peso = ijpeso[i * 3 + 2];

        // cria um novo nó para a lista de adjacência da origem
        NO *novo_no = (NO *)malloc(sizeof(NO));
        novo_no->adj = destino;
        novo_no->peso = peso;
        novo_no->prox = grafo[origem - 1].inicio;
        grafo[origem - 1].inicio = novo_no;

        // cria um novo nó para a lista de adjacência do destino (grafo não direcionado)
        NO *novo_no2 = (NO *)malloc(sizeof(NO));
        novo_no2->adj = origem;
        novo_no2->peso = peso;
        novo_no2->prox = grafo[destino - 1].inicio;
        grafo[destino - 1].inicio = novo_no2;
    }

    return grafo;
}


// Inicializa vias e distancias (respectivamente com -1 e infinito, exe)
void inicializaGrafoAdj(VERTICE *g, int v, int origem) // ICARO: função para inicialização do grafo, retirada de dentro da função de busca.
{
    int i;
    for (i = 0; i < v; i++)
    {
        g[i].dist = 2147483647 / 2;
        g[i].via = -1;
        g[i].flag = 0;
    }
    g[origem - 1].dist = 0;
    g[origem - 1].flag = 1;
}

// Variável global auxiliar na gravação do percurso para concatenar a 1ª parte (inicio->chave) com a 2ª parte (chave->fim) da busca Dijkstra.
NO *ultimoElemento;

// Retorna percurso computado pela buscaDijkstra do vertice inicio até o vertice passado como parâmetro na função. 
NO *gravaPercurso(VERTICE *g, int vertice, int grava_ultimo_elemento)
{
    NO *no = (NO *)malloc(sizeof(NO));
    no->adj = vertice;
    no->peso = g[vertice - 1].dist;
    no->prox = NULL;

    if(grava_ultimo_elemento == 1)
        ultimoElemento = no;

    int via = g[vertice - 1].via;

    while (via != -1)
    {
        NO *aux = (NO *)malloc(sizeof(NO));
        aux->adj = via;
        aux->peso = g[via - 1].dist;
        aux->prox = no;
        no = aux;
        via = g[via - 1].via;
    }
    return no;
}



// Realiza a busca usando Algorit. Dijkstra e retorna 1 se o vertice fim foi alcançavel e 0 se não foi alcançavel
int buscaDijkstra(VERTICE *g, int origem, int fim, int v)
{
    if(origem <= 0 || fim <= 0)
        return 0;
    NO_FILA *fila = criaFilaVazia(v);
    insereFila(fila, origem, 0);

    while (!filaVazia(fila))
    {
        int vertice = pegaFila(fila, v);
        if (g[vertice - 1].flag == 1)
        {
            NO *adj = g[vertice - 1].inicio;
            while (adj)
            {
                if (g[adj->adj - 1].aberto == 1)
                {
                    if ((g[vertice - 1].dist + adj->peso) < g[adj->adj - 1].dist)
                    {
                        g[adj->adj - 1].via = vertice;
                        g[adj->adj - 1].dist = g[vertice - 1].dist + adj->peso;
                    }
                if(g[adj->adj - 1].flag == 0)
                {
                    g[adj->adj - 1].flag = 1;
                    insereFila(fila, adj->adj, g[adj->adj - 1].dist);
                }
                }
                adj = adj->prox;
            }
            g[vertice - 1].flag = 2;
        }
    }

    free(fila);
    if (g[fim - 1].via != -1)
        return 1;
    else
        return 0;
}

NO *caminho(int N, int A, int *ijpeso, int *aberto, int inicio, int fim, int chave);
//------------------------------------------
// O EP consiste em implementar esta funcao
// e outras funcoes auxiliares que esta
// necessitar
//------------------------------------------
NO *caminho(int N, int A, int *ijpeso, int *aberto, int inicio, int fim, int chave)
{
    if((inicio <= 0 || fim <= 0) || (inicio == fim))
        return NULL;
    
    VERTICE *g = criaGrafoAdj(N, A, ijpeso, aberto);
    inicializaGrafoAdj(g, N, inicio);
    
    NO *percursoComChave = NULL, *percursoSemChave = NULL, *aux = NULL;
    int distanciaComChave = 2147483647, distanciaSemChave = 2147483647, existeCaminho = 0;

    existeCaminho = buscaDijkstra(g, inicio, chave, N);

    if (existeCaminho)
    {
        percursoComChave = gravaPercurso(g, chave, 1);
        int dist_aux = g[chave - 1].dist;
        abrirSalas(g, N);
        inicializaGrafoAdj(g, N, chave);
        g[chave - 1].dist = dist_aux;
        existeCaminho = buscaDijkstra(g, chave, fim, N);

        if(existeCaminho)
        {   
            distanciaComChave = g[fim - 1].dist;
            aux = gravaPercurso(g, fim, 0); 
            ultimoElemento->prox = aux->prox;
        }
    }

    inicializarSalas(g, N, aberto);
    inicializaGrafoAdj(g, N, inicio);

    existeCaminho = buscaDijkstra(g, inicio, fim, N);

    if (existeCaminho)
    {   
        distanciaSemChave = g[fim - 1].dist;
        percursoSemChave = gravaPercurso(g, fim, 0);
    }

    free(g);
    
    if((distanciaComChave >= distanciaSemChave) && (distanciaSemChave != 2147483647))
    {
            free(percursoComChave);
            return percursoSemChave;
    }
    else if((distanciaComChave <= distanciaSemChave) && (distanciaComChave != 2147483647))
    {
            free(percursoSemChave);
            return percursoComChave;
    } else {
        free(percursoSemChave);
        free(percursoComChave);
        return NULL;
    }
}

//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
//---------------------------------------------------------
int main()
{
	int N = 9; // grafo de 3 vértices numerados de 1..3
    int A = 10;
    int aberto[] = {0, 1, 1, 1, 1, 1, 1, 1, 1}; // todos abertos
    int inicio = 7;
    int fim = 7;
    int chave = 6;
    int ijpeso[] = {
        1, 2, 30,
        1, 3, 20,
        2, 6, 20,
        2, 7, 30,
        3, 7, 80,
        3, 4, 20,
        4, 9, 80,
        5, 8, 10,
        6, 7, 10,
        7, 9, 80 };

    for(int i = 1; i < 9; i ++){
        inicio = i;
        for(int j = 0; j < 9; j ++){
            fim = j;
            NO *teste = NULL;
            teste = caminho(N, A, ijpeso, aberto, inicio, fim, chave);
            int parada_debug = 0;
        }
    }
    return 0;
}

// por favor nao inclua nenhum código abaixo da função main()