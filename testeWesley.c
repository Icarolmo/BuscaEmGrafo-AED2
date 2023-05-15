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

// Estrutura nó da Fila em lista ligada que será utilizada no algoritmo de busca (Dijkstra).
typedef struct noFila
{
    int vertice;
    struct noFila *prox;
} NO_FILA;

// Estrutura FILA implementada em lista ligada.
typedef struct fila
{
    NO_FILA *inicio;
    NO_FILA *fim;
} FILA;

// ICARO: Acima está a estrutura ideal (no meu entender) para a FILA que será necessária no algoritmo de Dijkstra.
// A partir daqui realizei a implementação de algumas funções para manipular a FILA para utilizarmos dentro do algoritmo de busca (Dijkstra).

// Retorna uma estrutura FILA vazia (fila->inicio, fila->fim = NULL)
FILA *criaFilaVazia()
{
    FILA *fila = (FILA *)malloc(sizeof(FILA *));
    fila->fim = NULL, fila->inicio = NULL;
    return fila;
}

// Verifica se a FILA está vazia, retorna TRUE se estiver vazia e FALSE se tiver um ou mais elementos.
int filaVazia(FILA *fila)
{
    if (fila->inicio)
        return 0;
    else
        return 1;
}

// Insere o elemento, no caso o vertice, na FILA.
void insereFila(FILA *fila, int vertice)
{
    NO_FILA *no = (NO_FILA *)malloc(sizeof(NO_FILA *));
    no->vertice = vertice;
    no->prox = NULL;
    if (filaVazia(fila))
        fila->inicio = no, fila->fim = no;
    else
    {
        fila->fim->prox = no;
        fila->fim = no;
    }
}

// Pega o primeiro vértice (apontado pelo fila->inicio->vertice) da FILA.
int pegaFila(FILA *fila)
{
    int vertice = fila->inicio->vertice;
    NO_FILA *aux = fila->inicio;
    fila->inicio = fila->inicio->prox;
    free(aux);
    return vertice;
}

// Destroi a FILA liberando seu espaço em memória.
void destroiFila(FILA *fila)
{
    free(fila);
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

// Cria um grafo em lista de adjacencias e já o inicializa variáveis padrão.
VERTICE *criaGrafoAdj(int v, int a, int *ijpeso, int *aberto) // WESLEY: fiz algumas alterações nessa função, agora os campos adj e peso estão começãndo com valor -1, a função foi testada e funciona
{
    VERTICE *grafo = (VERTICE *)malloc(sizeof(VERTICE) * (v + 1)); // aloca memória para o grafo

    for (int i = 1; i <= v; i++)
    {
        NO *novo_no = (NO *)malloc(sizeof(NO));
        novo_no->adj = -1;
        novo_no->peso = -1;
        novo_no->prox = NULL;
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
        novo_no->prox = grafo[origem].inicio;
        grafo[origem].inicio = novo_no;

        // cria um novo nó para a lista de adjacência do destino (grafo não direcionado)
        novo_no = (NO *)malloc(sizeof(NO));
        novo_no->adj = origem;
        novo_no->peso = peso;
        novo_no->prox = grafo[destino].inicio;
        grafo[destino].inicio = novo_no;
    }

    return grafo;
}

/* Cria grafo transposto do grafo principal (necessario a análise da necessidade da/do função/grafo)
VERTICE *criaGrafoTran(int v, int a, int *ijpeso, int *aberto)
{
    VERTICE *gTrans = (VERTICE *) malloc(sizeof(VERTICE *)*v);
    int i, j;
    for(i = 0; i<v; i++)
    {
        gTrans[i].aberto = aberto[i];
        gTrans[i].inicio = NULL;
        for (j = 1; j < a * 3; j = j + 3)
        {
            if (ijpeso[j] == i)
            {
                NO *aux = (NO *)malloc(sizeof(NO *));
                aux->adj = ijpeso[j - 1];
                aux->peso = ijpeso[j + 1];
                aux->prox = gTrans[i].inicio;
                gTrans[i].inicio = aux;
            }
        }
    }
    return gTrans;
}
*/

void zeraFlags(VERTICE *g, int v) // FUNCIONANDO CORRETAMENTE
{
    int i;
    for (i = 0; i < v; i++)
    {
        g[i].flag = 0;
    }
}

// Inicializa variáveis distância e via do grafo, respectivamente com infinito e -1 (também inicializa o vertice origem).
void inicializaGrafoAdj(VERTICE *g, int v, int origem) // ICARO: função para inicialização do grafo, retirada de dentro da função de busca.
{
    int i;
    for (i = 0; i < v; i++)
    {
        g[i].dist = 2147483647 / 2;
        g[i].via = -1;
    }
    g[origem].dist = 0;
    g[origem].flag = 1;
} // FAZER TESTES

void imprimeGrafoInicializado(VERTICE *g, int v, int origem)
{
    int i;
    for (i = 0; i < v; i++)
    {
        if (i == origem)
        {
            printf("\nO vertice %d eh a origem e tem distancia %d, via %d, flag %d e o booleano de aberto %d\n\n", i, g[i].dist, g[i].via, g[i].flag, g[i].aberto);
        }
        else
        {
            printf("O vertice %d tem distancia %d, via %d, flag %d e o booleano de aberto %d\n", i, g[i].dist, g[i].via, g[i].flag, g[i].aberto);
        }
    }
}

void imprimeGrafoAdj(VERTICE *grafo, int V) // WESLEY: Criei essa função para verificar se a função criaGrafoAdj estava funcionando
{
    int i;
    for (i = 1; i < V; i++)
    {
        printf("Vertice %d: ", i);
        NO *atual = grafo[i].inicio;
        while (atual != NULL)
        {
            printf("(%d, %d) ", atual->adj, atual->peso); // segmentation fault
            atual = atual->prox;
        }
        printf("\n");
    }
}

void imprimeFlagVertices(VERTICE *grafo, int V)
{

    int i;

    for (i = 0; i < V; i++)
    {
        printf("\nO vertice %d tem a flag %d", V, grafo[i].flag);
    }
}
// Aqui finalizaria o EP.

//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
//---------------------------------------------------------
int main()
{

    // // aqui vc pode incluir codigo de teste

    // // exemplo de teste trivial

    // int N = 3; // grafo de 3 vértices numerados de 1..3
    // int A = 3;
    // int aberto[] = {1, 1, 1}; // todos abertos
    // int inicio = 1;
    // int fim = 3;
    // int chave = 2;
    // int ijpeso[] = {1, 2, 10, 2, 3, 20, 3, 1, 10};

    // // o EP sera testado com uma serie de chamadas como esta
    // NO *teste = NULL;
    // teste = caminho(N, A, ijpeso, aberto, inicio, fim, chave);
    // return teste;

    // WESLEY: aqui criei um novo grafo para que possamos testar o algoritmo, comentei o teste original no código acima
    // A imagem do grafo se encontra um pouco depois a implementação das características do novo grafo
    int N = 10;
    int A = 15;
    int aberto[] = {1, 0, 0, 1, 1, 1, 0, 0, 0, 1}; // todos abertos
    int inicio = 4;
    int fim = 9;
    int chave = 10;
    int ijpeso[] = {
        1, 2, 4,
        1, 3, 2,
        1, 5, 3,
        2, 4, 2,
        2, 10, 2,
        3, 9, 1,
        4, 5, 8,
        4, 7, 2,
        4, 10, 7,
        5, 6, 3,
        5, 7, 9,
        6, 9, 1,
        7, 8, 2,
        8, 9, 3,
        8, 10, 2};

    VERTICE *grafo = criaGrafoAdj(N, A, ijpeso, aberto);

    // WESLEY: Aqui estou fazendo os testes que discutimos e criei algumas funções void auxiliares para printar as informações que queremos
    inicializaGrafoAdj(grafo, N, inicio);
    zeraFlags(grafo, N);

    // imprimeFlagVertices(grafo, N); // função void auxiliar, pode ser deletada
    imprimeGrafoInicializado(grafo, N, inicio);
}

// por favor nao inclua nenhum código abaixo da função main()
