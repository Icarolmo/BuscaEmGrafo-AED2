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
VERTICE *criaGrafoAdj(int v, int a, int *ijpeso, int *aberto)
{ 
	VERTICE *grafo = (VERTICE *)malloc(sizeof(VERTICE *) * v);
	int i, j;
	for(i = 0; i < v; i++)
	{
		grafo[i].aberto = aberto[i];
		grafo[i].inicio = NULL;
	}
	for(j = 0; j < a * 3; j = j + 3)
	{
		int v1 = ijpeso[j];
		int v2 = ijpeso[j + 1];
		int pesoAresta = ijpeso[j + 2]; 
		NO *aux;

		aux->adj = v2;
		aux->peso = pesoAresta;
		aux->prox = grafo[v1 - 1].inicio;
		grafo[v1 - 1].inicio = aux;

		aux->adj = v1;
		aux->prox = grafo[v2 - 1].inicio;
		grafo[v2 - 1].inicio = aux;
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


void zeraFlags(VERTICE *g, int v)
{
	int i;
	for(i = 0; i<v; i++)
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
	g[origem - 1].dist = 0; 
	g[origem - 1].flag = 1;
}

// Realiza a busca usando Dijkstra porém com vértice objetivo sendo o vertice que contém a chave.
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

// Realiza a busca usando Dijkstra da forma convêncional (computando a menor distância em todos os vértices alcançáveis a partir do vértice origem).
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
	NO *percursoComChave, *percursoSemChave, *aux = { NULL };
	int distanciaSemChave, distanciaComChave;

	if(buscaChave(g, N, inicio, chave)){
		abrirSalas(g, N);
		buscaDijkstra(g, N, chave, fim, -1);
		
		percursoComChave->adj = fim;
		percursoComChave->peso, distanciaComChave = g[fim - 1].dist;
		int vAnterior = g[fim - 1].via;

		while(vAnterior != -1)
		{	
			aux->peso = g[vAnterior - 1].dist;
			aux->adj = vAnterior;
			aux->prox = percursoComChave;
			percursoComChave = aux;
			vAnterior = g[vAnterior - 1].via;
		}

	} 
	// Aqui temos que realizar uma nova inicialização no grafo para deixa ele "zerado"
	// e assim realizar novamente a busca porém sem usar a chave.

	buscaDijkstra(g, N, inicio, fim, chave);
	
	percursoSemChave->adj = fim;
	percursoSemChave->peso, distanciaSemChave = g[fim - 1].dist;
	int vAnterior = g[fim - 1].via;

	while(vAnterior != -1)
	{	
		aux->peso = g[vAnterior - 1].dist;
		aux->adj = vAnterior;
		aux->prox = percursoSemChave;
		percursoSemChave = aux;

		vAnterior = g[vAnterior - 1].via;
	}

	free(g);
	if(distanciaComChave > distanciaSemChave) 
		return percursoSemChave;
	else
		return percursoComChave;
}
	// Aqui finalizaria o EP.

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
