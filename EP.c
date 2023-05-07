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


// ICARO: A PARTIR DAQUI IMPLEMENTAMOS NOSSAS FUNÇÕES E CÓDIGOS AUXILIARES ANTES DA FUNÇÃO PRINCIPAL CAMINHO

typedef struct noFila{ // Estrutura nó da Fila em lista ligada que será implementada no algoritmo de busca.
	int vertice;
	struct noFila *prox;
} NO_FILA;

typedef struct fila{ // Estrutura da respectiva Fila em lista ligada. 
	NO_FILA *inicio;
	NO_FILA *fim;
} FILA;


// ICARO: Acima está a estrutura ideal (no meu entender) para a FILA que será necessária no algoritmo de Dijkstra.
// A partir daqui realizei a implementação de algumas funções para manipular a FILA para utilizarmos dentro do algoritmo de busca (Dijkstra).


FILA *criaFilaVazia(){ // Cria a estrutura FILA vazia com seu inicio e fim apontando para NULL.
	FILA *fila = (FILA*) malloc(sizeof(FILA*));
	fila->fim, fila->inicio = NULL;
	return fila;
}


int filaVazia(FILA *fila){ // Verifica se a estrutura FILA está vazia
	if(fila->inicio) return 0;
	else return 1;
}


void insereFila(FILA *fila, int vertice){ // Insere o elemento, no nosso caso o vertice, na estrutura FILA.
	NO_FILA *no = (NO_FILA*) malloc(sizeof(NO_FILA*));
	no->vertice = vertice;
	no->prox = NULL;
	if(filaVazia(fila)) fila->inicio, fila->fim = no;
	else {
		fila->fim->prox = no;
		fila->fim = no;
	}
}


int pegaFila(FILA *fila){ // Pega o primeiro elemento (vertice que está no inicio da fila) da estrutura FILA.
	int vertice = fila->inicio->vertice;
	NO_FILA *aux = fila->inicio;
	fila->inicio = fila->inicio->prox;
	free(aux);
	return vertice;
}


void destroiFila(FILA *fila){ // Destroi a estrutura FILA liberando seu espaço em memória.
	free(fila);
}

// ICARO: A cima estão definidas (se eu não me esqueci de alguma) todas as funções necessárias para operar a estrutura FILA dentro do nosso algoritmo de busca (Dijkstra).

void abrirSalas(VERTICE *g, int N){ // Função para abertura de todas as salas (somente caso encontre a chave)
	int i;
	for(i = 0; i<N ; ++i){
		g[i].aberto = 1;
	}
}

VERTICE *criaGrafoAdj(int v, int a, int *ijpeso, int *aberto){ // Cria um grafo em lista de adjacencias e já o inicializa com os parâmetros fornecidos.
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


void buscaDijkstra(VERTICE *g, int v, int origem, int objetivo, int chave){ // ICARO: Algoritmo de busca que acredito ser o mais indicado para situação/problema.
	// inicializa as dist (peso das arestas) e vias (por qual vertice estou acessando o vertice atual) dos vertices, respectivamente, em infinito e 0.
	int i,j;
	for(i = 0; i<v; ++i){
		g[i].dist = 2147483647/2; // Para não termos problemas de estourar o limite de bytes de um int pegamos o maior inteiro possível e dividimos por 2.
		g[i].via = 0;
	}
	g[origem].dist = 0; // A dist do vertice inicial inicializa em 0 pelo fato de não termos percorrido ainda nenhuma distância.
	g[origem].flag  = 1;
	FILA *fila = criaFilaVazia();
	insereFila(fila, origem);

	while(!filaVazia(fila)){
	/* ICARO: Está e a parte principal do algoritmo, acho que consegui realizar verificações básicas como: inserir elementos na FILA, alterar sua Flag,
	após isto pegar o vertice da fila, pegar todos os seus adjacentese ai entrar em outro laço este sendo enquanto há vertices adjacentes, após isso
	verificar se este adjacente atual está aberto, caso aberto verificar se do vertice atual até o vertice adjacente atual a distancia é menor da que
	o vertice adjacente guarda, se sim alteramos o melhor (neste caso, o menor) caminho aquele que vem do vertice atual e a nova distancia do vertice
	inicial até este vertice adjacente. Após isso mudamos a Flag deste vertice adjacente para 1 e inserimos na FILA. Depois de realizar esse processo 
	para todos os vertices adjacentes do vertice atual marcamos a Flag do vertice atual como 2 (oque significa que já verificamos todos os seus adjacentes)
	e repetimos o processo para um novo vertice pegando ele da fila, sendo sempre o que está no inicio dela.
	
	OQUE FALTA: Acho que verificar se está fazendo tudo isso que citei acima corretamente e se sim implementar a parte que vamos até o vertice Objetivo e 
	pegamos a sua distância e as vias (o caminho do inicial até ele) e devolvemos para assim podermos inciar os teste até conseguir compilar e executar o
	primeito teste no MAIN deixado pelo professor.
	*/
		int vertice = pegaFila(fila);
		if(g[vertice].flag == 1){
			NO *adj = g[vertice].inicio;
			while(adj){
				if(g[adj->adj].aberto){

					if((g[vertice].dist + adj->peso) < g[adj->adj].dist){
							g[adj->adj].via = vertice;
							g[adj->adj].dist = g[vertice].dist + adj->peso;
						}
						
						if(adj->adj == chave) abrirSalas(g, v);
						g[adj->adj].flag = 1;
						insereFila(fila, adj->adj);
				}
	
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

