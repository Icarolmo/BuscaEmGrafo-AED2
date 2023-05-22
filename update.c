//--------------------------------------------------------------
// NOMES DOS RESPONSÁVEIS: Icaro Lima de Oliveira E Wesley Gabriel Rosa Fernandes
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


// ICARO: A PARTIR DAQUI IMPLEMENTAMOS NOSSAS FUNÇÕES E CÓDIGOS AUXILIARES ANTES DA FUNÇÃO PRINCIPAL CAMINHO

// Estrutura nó da Fila que será utilizada no algoritmo de busca (Dijkstra).
typedef struct noFila
{
    int vertice;
    int distancia;
} NO_FILA;

// Retorna uma estrutura NO_FILA vazia
NO_FILA *criaFilaVazia(int tamanho)
{
    NO_FILA *fila = (NO_FILA *) malloc(sizeof(NO_FILA) * tamanho);
    for(int i = 0; i < tamanho; i++)
        fila[i].distancia = 0, fila[i].vertice = -1;   

    return fila;
}

// Verifica se a FILA está vazia, retorna TRUE se estiver vazia e FALSE se tiver um ou mais elementos.
int filaVazia(NO_FILA *fila)
{
    if( fila[0].vertice != -1)
        return 0;
    else
        return 1;
}

// Insere o elemento, no caso o vertice, na FILA.
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

// Pega o primeiro vértice (apontado pelo fila->inicio->vertice) da FILA.
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

// Realiza abertura de todos os vertices, chamada somente no momento de descoberta do vértice chave.
void abrirSalas(VERTICE *g, int N)
{
    int i;
    for (i = 0; i < N; i++)
    {
        g[i].aberto = 1;
    }
}

// Possível necessidade, por via das dúvidas deixei aqui.
void inicializarSalas(VERTICE *g, int N, int *aberto)
{
    int i;
    for (i = 0; i < N; i++)
    {
        g[i].aberto = aberto[i];
    }
}

// Cria um grafo em lista de adjacencias e já o inicializa variáveis padrão.
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


// Inicializa variáveis distância e via do grafo, respectivamente com infinito e -1 (também inicializa o vertice origem).
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

NO *ultimoElemento;
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



// Realiza a busca usando Dijkstra da forma convêncional (computando a menor distância em todos os vértices alcançáveis a partir do vértice origem).
int buscaDijkstra(VERTICE *g, int origem, int fim, int v)
{
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
        return g[fim - 1].dist;
    else
        return 0;
}

// funcao principal
NO *caminho(int N, int A, int *ijpeso, int *aberto, int inicio, int fim, int chave);
//------------------------------------------
// O EP consiste em implementar esta funcao
// e outras funcoes auxiliares que esta
// necessitar
//------------------------------------------
NO *caminho(int N, int A, int *ijpeso, int *aberto, int inicio, int fim, int chave)
{
    VERTICE *g = criaGrafoAdj(N, A, ijpeso, aberto);
    inicializaGrafoAdj(g, N, inicio);

    NO *percursoComChave = NULL, *percursoSemChave = NULL, *aux = NULL;
    int distanciaSemChave, distanciaComChave;

    distanciaComChave = buscaDijkstra(g, inicio, chave, N);

    if (distanciaComChave != 0)
    {
        percursoComChave = gravaPercurso(g, chave, 1);
        abrirSalas(g, N);
        inicializaGrafoAdj(g, N, chave);
        g[chave - 1].dist = distanciaComChave;
        distanciaComChave = buscaDijkstra(g, chave, fim, N);
        aux = gravaPercurso(g, fim, 0); 
        ultimoElemento->prox = aux->prox;
    }
    else
    {
        distanciaComChave = 2147483647 / 2;
    }

    inicializarSalas(g, N, aberto);
    inicializaGrafoAdj(g, N, inicio);
    distanciaSemChave = buscaDijkstra(g, inicio, fim, N);

    if (distanciaSemChave != 0)
        percursoSemChave = gravaPercurso(g, fim, 0);
    else
        distanciaSemChave = 2147483647 / 2;

    free(g);
    if (distanciaComChave > distanciaSemChave)
    {
        free(percursoComChave);
        return percursoSemChave;
    }
    else
    {
        free(percursoSemChave);
        return percursoComChave;
    }
}

// Aqui finalizaria o EP.

//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
//---------------------------------------------------------
int main()
{
	// // exemplo de teste trivial
    /*
	int N = 3; // grafo de 3 vértices numerados de 1..3
	int A = 3;
	int aberto[] = {1, 1, 1}; // todos abertos
	int inicio = 1;
	int fim = 3;
	int chave = 2;
	int ijpeso[] = {1, 2, 10, 2, 3, 20, 3, 1, 10};
    */
    int N = 9; // grafo de 3 vértices numerados de 1..3
    int A = 10;
    int aberto[] = {0, 1, 1, 1, 1, 1, 1, 1, 1}; // todos abertos
    int inicio = 7;
    int fim = 4;
    int chave = 6;
    int ijpeso[] = {
        1, 2, 1,
        1, 3, 1,
        2, 6, 1,
        2, 7, 30,
        3, 7, 80,
        3, 4, 20,
        4, 9, 80,
        5, 8, 10,
        6, 7, 1,
        7, 9, 80};

	// // o EP sera testado com uma serie de chamadas como esta
	NO *teste = NULL;
	teste = caminho(N, A, ijpeso, aberto, inicio, fim, chave);


    return 0;
}

// por favor nao inclua nenhum código abaixo da função main()