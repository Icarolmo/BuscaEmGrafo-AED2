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

typedef struct noFila
{ // Estrutura nó da Fila em lista ligada que será implementada no algoritmo de busca.
	int vertice;
	struct noFila *prox;
} NO_FILA;

typedef struct fila
{ // Estrutura da respectiva Fila em lista ligada.
	NO_FILA *inicio;
	NO_FILA *fim;
} FILA;

// ICARO: Acima está a estrutura ideal (no meu entender) para a FILA que será necessária no algoritmo de Dijkstra.
// A partir daqui realizei a implementação de algumas funções para manipular a FILA para utilizarmos dentro do algoritmo de busca (Dijkstra).

FILA *criaFilaVazia()
{ // Cria a estrutura FILA vazia com seu inicio e fim apontando para NULL.
	FILA *fila = (FILA *)malloc(sizeof(FILA *));
	fila->fim, fila->inicio = NULL;
	return fila;
}

int filaVazia(FILA *fila)
{ // Verifica se a estrutura FILA está vazia
	if (fila->inicio)
		return 0;
	else
		return 1;
}

void insereFila(FILA *fila, int vertice)
{ // Insere o elemento, no nosso caso o vertice, na estrutura FILA.
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

int pegaFila(FILA *fila)
{ // Pega o primeiro elemento (vertice que está no inicio da fila) da estrutura FILA.
	int vertice = fila->inicio->vertice;
	NO_FILA *aux = fila->inicio;
	fila->inicio = fila->inicio->prox;
	free(aux);
	return vertice;
}

void destroiFila(FILA *fila)
{ // Destroi a estrutura FILA liberando seu espaço em memória.
	free(fila);
}

// ICARO: A cima estão definidas (se eu não me esqueci de alguma) todas as funções necessárias para operar a estrutura FILA dentro do nosso algoritmo de busca (Dijkstra).

void abrirSalas(VERTICE *g, int N)
{ // Função para abertura de todas as salas (somente caso encontre a chave)
	int i;
	for (i = 0; i < N; i++)
	{
		g[i].aberto = 1;
	}
}

VERTICE *criaGrafoAdj(int v, int a, int *ijpeso, int *aberto)
{ // Cria um grafo em lista de adjacencias e já o inicializa com os parâmetros fornecidos.
	VERTICE *grafo = (VERTICE *)malloc(sizeof(VERTICE *) * v);
	int i, j;
	for (i = 0; i < v; i++)
	{
		grafo[i].aberto = aberto[i];
		grafo[i].inicio = NULL;
		for (j = 0; j < a * 3; j = j + 3)
		{
			if (ijpeso[j] == i)
			{
				// ICARO: pelo fato do grafo não ser dirigido temos que inicializar ambos os vertices como adjacentes
				NO *aux = (NO *)malloc(sizeof(NO *));
				aux->adj = ijpeso[j + 1];
				aux->peso = ijpeso[j + 2];
				aux->prox = grafo[i].inicio;
				grafo[i].inicio = aux;
			}
		}
	}
	return grafo;
}



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

void zeraFlags(VERTICE *g, int v)
{
	int i;
	for(i = 0; i<v; i++)
	{
		g[i].flag = 0;
	}
}

void inicializaGrafoAdj(VERTICE *g, int v, int origem) // ICARO: função para inicialização do grafo, retirada de dentro da função de busca.
{
	int i, j;
	for (i = 0; i < v; i++)
	{
		g[i].dist = 2147483647 / 2; 
		g[i].via = -1;				
	}
	g[origem - 1].dist = 0; 
	g[origem - 1].flag = 1;
}


int buscaChave(VERTICE *g, int v, int origem, int chave)
{
	FILA *fila = criaFilaVazia();
	zeraFlags(g, v);
	g[origem -1].flag = 1;	
	insereFila(fila, origem);

	while (!filaVazia(fila))
	{
		int vertice = pegaFila(fila);
		if (g[vertice - 1].flag == 1)
		{
			NO *adj = g[vertice - 1].inicio;
			while (adj)
			{
				if ((g[vertice - 1].dist + adj->peso) < g[adj->adj - 1].dist)
				{
					g[adj->adj - 1].via = vertice;
					g[adj->adj - 1].dist = g[vertice - 1].dist + adj->peso;
				}

				if(adj->adj == chave){
					destroiFila(fila);
					return g[adj->adj - 1].dist;
				}
				
				g[adj->adj - 1].flag = 1;
				insereFila(fila, adj->adj);
				
				adj = adj->prox;
			}
			g[vertice].flag = 2;
		}
	}
	return 0;
}


void buscaDijkstra(VERTICE *g, int v, int origem, int objetivo, int chave)
{ 	
	FILA *fila = criaFilaVazia();
	zeraFlags(g, v);
	g[origem -1].flag = 1;
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
					g[adj->adj - 1].flag = 1;
					insereFila(fila, adj->adj);

					if ((g[vertice - 1].dist + adj->peso) < g[adj->adj - 1].dist)
					{
						g[adj->adj - 1].via = vertice;
						g[adj->adj - 1].dist = g[vertice - 1].dist + adj->peso;
					}

					if (adj->adj == chave) 
					{ /*ICARO: para a busca do vertice inicio até o fim (sem o inicio partir do vertice chave) vamos precisar realizar essa verificação 
					 	e para reaproveitar essa mesma função para os dois cenários mantive esta verificação mas ainda vou ver um modo mais eficiênte (se possível)*/
						abrirSalas(g, v);
					}

				}
				adj = adj->prox;
			}
			g[vertice].flag = 2;
		}
	}
	
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
	if(buscaChave(g, N, inicio, chave)){
		abrirSalas(g, N);
		buscaDijkstra(g, N, chave, fim, -1);
	} 

	VERTICE *gTran = criaGrafoTran(N, A, ijpeso, aberto);

	buscaDijkstra(g, N, inicio, fim, chave);

	NO *percurso, *aux = { NULL };

	if(g[fim - 1].via == -1) 
		return percurso;

	percurso->peso = g[fim - 1].dist;
	percurso->adj = fim;
	int vAnterior = g[fim - 1].via;

	while(vAnterior != -1)
	{	
		aux->peso = g[vAnterior - 1].dist;
		aux->adj = vAnterior;
		aux->prox = percurso;
		percurso = aux;
		vAnterior = g[vAnterior - 1].via;
	}

	free(g);
	return percurso;
}

	// Aqui finalizaria o EP.

	/*
	 ICARO: o código abaixo não tivemos tempo de discuti sobre, quando possível vemos se faz necessário ou apenas 
	 acima já sana o problema de revolução do caminho para o usuário.
	if (g[fim - 1].dist != 2147483647 / 2)
	{
		int atual = fim - 1;
		while (atual != inicio - 1)
		{
			NO *novo = (NO *)malloc(sizeof(NO));
			novo->adj = atual + 1;
			novo->prox = caminho;
			caminho = novo;
			atual = g[atual].via;
		}
		NO *novo = (NO *)malloc(sizeof(NO));
		novo->adj = atual + 1;
		novo->prox = caminho;
		caminho = novo;
	}

	if (g[chave - 1].dist != 2147483647 / 2)
	{
		abrirSalas(g, N);
	}

	free(g);
	return caminho;
}
	*/


//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
//---------------------------------------------------------
int main()
{

	// aqui vc pode incluir codigo de teste

	// exemplo de teste trivial

	int N = 3; // grafo de 3 vértices numerados de 1..3
	int A = 3;
	int aberto[] = {1, 1, 1}; // todos abertos
	int inicio = 1;
	int fim = 3;
	int chave = 2;
	int ijpeso[] = {1, 2, 10, 2, 3, 20, 3, 1, 10};

	// o EP sera testado com uma serie de chamadas como esta
	NO *teste = NULL;
	teste = caminho(N, A, ijpeso, aberto, inicio, fim, chave);
	return teste;
}

// por favor nao inclua nenhum código abaixo da função main()
