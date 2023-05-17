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
	int flag;	// para uso na busca em largura e profundidade, se necessario
	int aberto; // vale true se a sala em questao esta aberta
	int via;	// use este campo se precisar computar algum caminho etc.
	int dist;	// use este campo se precisar computar alguma distância etc.
	NO *inicio;
} VERTICE;

// ICARO: A PARTIR DAQUI IMPLEMENTAMOS NOSSAS FUNÇÕES E CÓDIGOS AUXILIARES ANTES DA FUNÇÃO PRINCIPAL CAMINHO

NO *ponteiro_aux;

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
	fila->fim, fila->inicio = NULL;
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
		fila->inicio, fila->fim = no;
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

    // Ajuste situacional: contendo somente um elemento na fila tanto inicio quanto fim recebem apontam para o próximo elemento, caso tenha 2 ou + somente inicio aponta para proxímo.
    if(fila->inicio != fila->fim)
        fila->inicio = fila->inicio->prox;
    else
        fila->fim = fila->inicio->prox, fila->inicio = fila->inicio->prox;

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
	VERTICE *grafo = (VERTICE *)malloc(sizeof(VERTICE) *v); // aloca memória para o grafo

	for (int i = 0; i < v; i++)
	{
		NO *novo_no = (NO *)malloc(sizeof(NO));
		novo_no->adj = NULL, novo_no->peso = NULL, novo_no->prox = NULL;
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


void zeraFlags(VERTICE *g, int v)
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
        g[i].flag = 0;
	}
	g[origem - 1].dist = 0;
	g[origem - 1].flag = 1;
}

/*
// Realiza a busca usando Dijkstra porém com vértice objetivo sendo o vertice que contém a chave.
int buscaChave(VERTICE *g, int v, int origem, int chave)
{
	FILA *fila = criaFilaVazia();
	insereFila(fila, origem);

	while (!filaVazia(fila))
	{
		int vertice = pegaFila(fila);
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

					g[adj->adj - 1].flag = 1;
					insereFila(fila, adj->adj);

				}
				adj = adj->prox;
			}
			g[vertice].flag = 2;
		}
	}

	if (g[chave - 1].via != -1)
		return g[chave - 1].dist;
    else
        return 0;
}
*/

NO *gravaPercurso(VERTICE *g, int vertice, NO *ultimoElemento)
{   
    NO *no = (NO*) malloc(sizeof(NO));
    no->adj = vertice;
    no->peso =  g[vertice -1].dist;
    no->prox = NULL;
	ultimoElemento = no;
    int via = g[vertice -1].via;

    while(via != -1)
    {
        NO *aux = (NO*) malloc(sizeof(NO));
        aux->adj = via;
        aux->peso =  g[via -1].dist;
        aux->prox = no;
        no = aux;
        via = g[via -1].via;
    }
    return no;
}

// Realiza a busca usando Dijkstra da forma convêncional (computando a menor distância em todos os vértices alcançáveis a partir do vértice origem).
int buscaDijkstra(VERTICE *g, int v, int origem, int fim)
{
	FILA *fila = criaFilaVazia();
	insereFila(fila, origem);
	while (!filaVazia(fila))
	{
		int vertice = pegaFila(fila);
		if (g[vertice - 1].flag == 1)
		{
			NO *adj = g[vertice - 1].inicio;
			while (adj)
			{
				if (g[adj->adj - 1].aberto)
				{
					if ((g[vertice - 1].dist + adj->peso) < g[adj->adj - 1].dist)
					{
						g[adj->adj - 1].via = vertice;
						g[adj->adj - 1].dist = g[vertice - 1].dist + adj->peso;
					}	
                    
                    g[adj->adj - 1].flag = 1;
					insereFila(fila, adj->adj);

				}
				adj = adj->prox;
			}
			g[vertice].flag = 2;
		}
	}
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
	NO *percursoComChave = NULL, *percursoSemChave = NULL, *aux = NULL;
	int distanciaSemChave, distanciaComChave;

    distanciaComChave = buscaDijkstra(g, N, inicio, chave);

	if (distanciaComChave != 0);
	{
        percursoComChave = gravaPercurso(g, chave, aux);
		abrirSalas(g, N);
        inicializaGrafoAdj(g, N, chave);
        g[chave - 1].dist = distanciaComChave;
		distanciaComChave = buscaDijkstra(g, N, chave, fim);
        percursoComChave = gravaPercurso(g, fim);
        aux = aux->prox;
        while(aux2)
        {
            aux2 = aux2->prox;
        }
        aux2->prox = percursoComChave;
	}
    


	// Aqui temos que realizar uma nova inicialização no grafo para deixa ele "zerado"
	// e assim realizar novamente a busca porém sem usar a chave.

	buscaDijkstra(g, N, inicio, fim);

	percursoSemChave->adj = fim;
	percursoSemChave->peso, distanciaSemChave = g[fim - 1].dist;
	int vAnterior = g[fim - 1].via;

	while (vAnterior != -1)
	{
		aux->peso = g[vAnterior - 1].dist;
		aux->adj = vAnterior;
		aux->prox = percursoSemChave;
		percursoSemChave = aux;

		vAnterior = g[vAnterior - 1].via;
	}

	free(g);
	if (distanciaComChave > distanciaSemChave)
		return percursoSemChave;
	else
		return percursoComChave;
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
	int N = 10; // grafo de 3 vértices numerados de 1..3
	int A = 15;
	int aberto[] = {1, 4, 5, 6, 10}; // todos abertos
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
	// inicializaGrafoAdj(grafo, N, inicio);
	imprimeGrafoAdj(grafo, N);
}

// por favor nao inclua nenhum código abaixo da função main()