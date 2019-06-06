/* 
 * Grupo Unicorn Squad
 * Projeto 1 "Divide Matriz" - Sistemas Operacionais
 *
 * O programa separa uma matriz NxN em duas matrizes a partir da diagonal principal: 
 * Uma matriz possui os valores da diagonal principal e acima dela;
 * Uma matriz com os valores abaixo da diagonal principal. 
 *
 * Autoras: 
 * Ana Luisa Fogarin 	- 193948
 * Isabela Mendes		- 218123
 * Larissa Correia		- 219765
 *
 */

/* ------------------------- BIBLIOTECAS --------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

/* -------------------------- ESTRUTURA --------------------------------- */ 
/* Estrutura para passagem dos dados necessarios para as funcoes */ 
typedef struct Argumentos {
	int N;
	int T;
	int posicao; 
	double **matrizGerada;
	double **matrizCima;
	double **matrizBaixo;
} argumentos;

/* ---------------------------- FUNÇOES --------------------------------- */ 
/* Funçao que identifica linha e coluna da matriz */ 
int linha(int ordem, int local){
	return local/ordem;
}

int coluna(int ordem, int local){
	return local%ordem;
}

/* Funcao que realiza a divisao da matriz */
void *separaMatriz(void *arg){
	argumentos *x = arg; 				//cria um novo objeto da struct para receber os argumentos passados
	int i = linha(x->N, x->posicao); 	//descobre em qual linha o elemento que a thread ira analisar esta
	int j = coluna(x->N, x->posicao); 	//descobre em qual coluna o elemento que a thread ira analisar esta
	
	/* Laço while permite proporciona a repeticao ate que atinja o limite de elementos da matriz*/ 
	while(x->posicao <= (x-> N*x->N)-1) {
		if (i <= j) //Define matriz superior
		{
			x->matrizCima[i][j] = x->matrizGerada[i][j];
			x->matrizBaixo[i][j] = 0; //Se nao eh o elemento desejado, iguala a zero 
		} 
		else { //Define matriz inferior 
			x->matrizBaixo[i][j] = x->matrizGerada[i][j];
			x->matrizCima[i][j] = 0; //Se nao eh o elemento desejado, iguala a zero 
		}
		
		/* Muda a posicao para realizar a divisao no proximo elemento desejado */
		x->posicao += x->T;
		i = linha(x->N, x->posicao);
		j = coluna(x->N, x->posicao);
	}
	return(NULL);
}
/* ---------------------------------------------------------------------- *
 *
 *
 * ------------------------- PROGRAMA PRINCIPAL ------------------------- */
int main (int argc, char *argv[]) { 
	int N; //N = dimensao;
	int T; //T = numero de threads
	int i, j;
	char nomeMatriz[30]; 
	
	/* Atribui as variveis os valores passados juntos a inicializaçao do programa  */
	/* Afunçao "atoi" faz conversao de tipo char para int */
	N = atoi(argv[1]); 
	T = atoi(argv[2]);
	strcpy(nomeMatriz, argv[3]);
	
	/* Cria as threads*/
	pthread_t threads[T];
	/* Cria um objeto da struct */
	argumentos dados[T]; 
	
	/* Cria e aloca dinamicamente as matrizes matrizes 
	 *
	 * Cria um vetor em que cada posiçao tem um double* (linhas da matriz)
	 * Para cada posiçao double* do vetor, cria um novo vetor double (colunas da matriz) 
	 */ 
	
	double **matrizGerada = (double **) malloc(sizeof(double *) * N); 
	for (i=0; i < N; i++) {
		matrizGerada[i] = (double *) malloc(sizeof(double) * N);
	}
	
	double **matrizCima = (double **) malloc(sizeof(double) * N);
	for (i=0; i < N; i++) {
		matrizCima[i] = (double *) malloc(sizeof(double) * N); 
	}
	
	double **matrizBaixo = (double **) malloc(sizeof(double) * N);
	for (i=0; i < N; i++) {
		matrizBaixo[i] = (double *) malloc(sizeof(double) * N); 
	}
	
	/* Cria e abre arquivo para ler a matriz fornecida*/ 
	FILE *arq; 
	arq = fopen(nomeMatriz, "r"); 
	if (arq == NULL) {
		puts("Ocorreu um erro ao abrir o arquivo da matriz!\n");
		return 1;
	}
	
	/* Le a matriz do arquivo */ 
	for (i=0; i < N; i++) {
		for (j=0; j < N; j++) {
			fscanf(arq,"%lf", &matrizGerada[i][j]);
		}
	}
	
	/* Fecha arquivo da matriz */ 
	fclose(arq);
	
	/* Remove a extensao do nome da matrizGerada para poder criar arquivos das matrizes desejadas da forma desejada */ 
	const char c[2] = ".";
	char *nomeSemExt;
	nomeSemExt = strtok(nomeMatriz, c); 
	/* A funçao "strtok(string1, string2) aponta o ponteiro atribuido a mesma para a string1 delimitada pela string2,
	 * nesse caso, passa para nomeSemExt a string nomeMatriz ate encontrar o caractere "." (atribuido na variavel c),
	 * permitindo assim, criar os nomes dos arquivos para gravas as matrizes resultantes
	 */ 

	/* Define variaveis para contar o tempo */ 
	clock_t tempIni;
	clock_t tempFim;
	
	/* ------------------------------ Implementa thread ------------------------------ */
	tempIni = clock();
	for(i=0; i < T; i++) {
		/* Passa os valores necessarios para a struct*/
		dados[i].N = N; 
		dados[i].T = T;
		dados[i].matrizGerada = matrizGerada;
		dados[i].matrizCima = matrizCima;
		dados[i].matrizBaixo = matrizBaixo;
		dados[i].posicao = i;
		
		/* Cria as threads */ 
		pthread_create(&threads[i], NULL, separaMatriz, (void *)&dados[i]);
												
	}
	 
	for (i=0; i < T; i++) {
		pthread_join(threads[i], NULL);
	}
	/* Termina a contagem de tempo e atribui a tempoTotal o tempo decorrido */ 
	tempFim = clock(); 
	double tempoTotal = (tempFim - tempIni) * 1000 / CLOCKS_PER_SEC; 
	
	/* -------------------------------------------------------------------------------- */
	
	/* Cria a string com o nome desejado para gravaçao da matriz superior*/
	char primeiraMatriz[30];
	strcpy(primeiraMatriz, nomeSemExt);
	strcat(primeiraMatriz, ".diag1");
	
	/* Cria e abre arquivo para a matriz superior */ 
	arq = fopen(primeiraMatriz, "w+"); 
	if (arq == NULL) {
		puts("Ocorreu um erro ao abrir o arquivo da matriz diag1!\n");
		return 1;
	}
	
	/* Grava matrizCima no arquivo */ 
	for (i=0; i < N; i++) {
		for (j=0; j < N; j++) {
			fprintf(arq," %lf ", matrizCima[i][j]);
		} 
		fprintf(arq,"\n");
	}
	
	/* Fecha o arquivo */ 
	fclose(arq);
	
	/* Cria a string com o nome desejado para gravaçao da matriz inferior */
	char segundaMatriz[30];
	strcpy(segundaMatriz, nomeSemExt);
	strcat(segundaMatriz, ".diag2");
	
	/* Cria e abre arquivo para a matriz inferior */  
	arq = fopen(segundaMatriz, "w+"); 
	if (arq == NULL) {
		puts("Ocorreu um erro ao abrir o arquivo da matriz diag2!\n");
		return 1;
	}
	
	/* Grava matrizBaixo no arquivo */ 
	for (i=0; i < N; i++) {
		for (j=0; j < N; j++) {
			fprintf(arq, " %lf ", matrizBaixo[i][j]);
		}
		fprintf(arq,"\n");
	}
	
	/* Fecha o arquivo */ 
	fclose(arq);
	
	/* Libera a memoria alocada pelas matrizes */
	free(matrizGerada);
	free(matrizCima);
	free(matrizBaixo);
	
	/* Exibe na tela o tempo que o programa levou */
	printf("\nO programa levou %.3lf ms para separar a matriz.\n", tempoTotal);
	
return 0;
}
