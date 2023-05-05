//--------------------------------------------------------------
// NOMES DOS RESPONSÁVEIS: Icaro Lima de Oliveira E Wesley Gabriel Rosa Fernandes
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int grupo() {
  return 10;
}

// ######### ESCREVA O NROUSP AQUI
int nroUSP1() {
    return 13718662;
}

int nroUSP2() {
    return 13716893;
}

// elemento das listas de adjacência e de resposta - NAO ALTERE ESTA DEFINICAO
typedef struct estr 
{
    int adj; // elemento
	int peso; // custo (não precisa ser usado na resposta)
    struct estr *prox;
} NO;

// vertices do grafo (salas) - use este tipo ao criar o grafo  - NAO ALTERE ESTA DEFINICAO
typedef struct 
{
    int flag; // para uso na busca em largura e profundidade, se necessario
    int aberto; // vale true se a sala em questao esta aberta
    int via; // use este campo se precisar computar algum caminho etc.
	int dist; // use este campo se precisar computar alguma distância etc.
    NO* inicio;
} VERTICE;



// COMENTÁRIO ICARO: A PARTIR DAQUI IMPLEMENTAMOS NOSSAS FUNÇÕES E CÓDIGOS AUXILIARES ANTES DA FUNÇÃO PRINCIPAL CAMINHO

typedef struct noFila{ // Estrutura nó da Fila em lista ligada que será implementada no algoritmo de busca.
	int vertice;
	struct noFila *prox;
} NO_FILA;

typedef struct fila{ // Estrutura da respectiva Fila em lista ligada. 
	NO_FILA *inicio;
	NO_FILA *fim;
} FILA;


void abrirSalas(VERTICE *g, int N){ // Função para abertura de todas as salas (somente caso encontre a chave)
	int i;
	for(i = 0; i<N ; ++i){
		g[i].aberto = 1;
	}
}

VERTICE *criaGrafoAdj(int v, int a, int *ijpeso, int *aberto){ // Cria um grafo em lista de adjacencias e já o inicializa.
	VERTICE * grafo = (VERTICE *) malloc(sizeof(VERTICE*)*v);
	int i,j;
	for(i = 0; i<v; ++i){
		grafo[i].aberto = aberto[i];
		grafo[i].flag = 0;
		grafo[i].inicio = NULL;
		for(j = 0; j<a*3; j = j+3){
			if(ijpeso[j] == i){
				NO *aux = (NO*) malloc(sizeof(NO*));
				aux->adj = ijpeso[j+1];
				aux->peso = ijpeso[j+2];
				aux->prox = grafo[i].inicio;
				grafo[i].inicio = aux;
			}
		}
	}
	return grafo;
}


void buscaDijkstra(VERTICE *g, int v, int origem, int objetivo){ // ICARO: Algoritmo de busca que acredito ser o mais indicado para situação/problema.
	// inicializa as dist (peso das arestas) e vias (por qual vertice estou acessando o vertice atual) dos vertices, respectivamente, em infinito e 0.
	int i,j;
	for(i = 0; i<v; ++i){
		g[i].dist = 2147483647/2; // Para não termos problemas de estourar o limite de bytes de um int pegamos o maior inteiro possível e dividimos por 2.
		g[i].via = 0;
	}
	g[origem].dist = 0; // A dist do vertice inicial inicializa em 0.

	/* ICARO: 
	A partir daqui estava iniciando a inicialização da estrutura FILA 
	*Observação: acho melhor criar funções para criação/inicializa/inserção/pegar elemento/destruição da fila
	realizar estas operações dentro da buscaDijkstra vai deixa o código/função/estrutura muito sujo e zuado
	por enquanto vou deixa o inicio dela abaixo porém assim que der vou transferir pra funções externas e só chama-las
	dentro da buscaDijkstra
	*/
	NO_FILA *aux = (NO_FILA*) malloc(sizeof(NO_FILA*));
	aux->vertice = origem;
	aux->prox = NULL;
	FILA *fila;
	fila->inicio->vertice, fila->fim->vertice = aux->vertice;
	fila->inicio, fila->fim = aux;
	
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
	
	// seu codigo AQUI

	//...

}



//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
//---------------------------------------------------------
int main() {


	// aqui vc pode incluir codigo de teste

	// exemplo de teste trivial

	int N=3; // grafo de 3 vértices numerados de 1..3
    int A=3;
	int aberto[] = {1,1,1}; // todos abertos
	int inicio=1;
	int fim=3;
	int chave=2;
	int ijpeso[]={1,2,10, 2,3,20, 3,1,10};

	// o EP sera testado com uma serie de chamadas como esta
	NO* teste = NULL;
	teste = caminho(N, A, ijpeso, aberto, inicio, fim, chave);
	return teste;

}

// por favor nao inclua nenhum código abaixo da função main()

