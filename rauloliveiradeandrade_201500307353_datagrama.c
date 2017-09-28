#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_SIZE 513
#define MIN_SIZE 5
#define INICIO 0

FILE* input = NULL;
FILE* output = NULL; 

typedef struct tPacote{
	
	int numpac;
	int tampac;
	char dados[MAX_SIZE][MIN_SIZE];	//vetor de struct
	
}tPacote;

typedef struct tAuxiliares{
	
	int indice;
	int parametro;
	int linha;
	
}tAuxiliares;

struct tAuxiliares A; //salvador da patria

//troca
void trocar(tPacote* x, tPacote* y){

	tPacote aux;
	aux = *x;
	*x = *y;
	*y = aux;

}
//pai
unsigned int pai(int i){
	return ((i-1)/2);
}
//filho esquerdo
unsigned int fesq(int i){
	return ((2*i)+1);
}
//filho direito
unsigned int fdir(int i){
	return ((2*i)+2);
}
//heapfeels
void heapify(tPacote* V, unsigned int T, unsigned int i) {
	
	unsigned int P = i;
	unsigned int E = fesq(i);
	unsigned int D = fdir(i);
	
	if(E < T && V[E].numpac < V[P].numpac){
		P = E;
	}	
	if(D < T && V[D].numpac < V[P].numpac){
		P = D;
	}
	if(P != i) {
		trocar(&V[P],&V[i]);
		heapify(V,T,P);				
	}
}
void heapsort (tPacote* V){
	
	int cont = 0,c=0;
	
	while(V[INICIO].numpac == A.parametro){	
		if(cont==0){
			fprintf(output,"%d: ",A.linha);
			A.linha++;			
			cont++;
		}
		
	for(c=0;c<V[INICIO].tampac;c++){
		fprintf(output,"%s", V[INICIO].dados[c]);
		if((V[INICIO].tampac-1)!=c){ // para imprimir o " "
			fprintf(output," ");
		}			
	}		
	V[INICIO]= V[A.indice-1];
	heapify(V,A.indice,INICIO);
			
	A.parametro++;
	A.indice--;
	
		if(V[INICIO].numpac == A.parametro){ //para imprimir o " "
			fprintf(output," ");	
		} 
	}
	if(cont!=0){
		fprintf(output," \n");	
	}	
}
void abrindoArquivo(tPacote* V, int n, int lim){
	
	//---------------//
	// LER O ARQUIVO
	//---------------//
	int i = 0, j = 0,y = 0,k = 0, p = 0;
	
	for(i=0; i<n/lim;i++){
		for(y=0;y<lim;y++){
			
			fscanf(input,"%d %d",&V[A.indice].numpac, &V[A.indice].tampac);
			
			if(V[A.indice].numpac == A.parametro){ //flag para reduzir o heapify
				p++;
			}
		
			for(j=0; j<V[A.indice].tampac;j++){
				fscanf(input,"%s ",V[A.indice].dados[j]);
			}
			A.indice++;
		}
		
		if(p!=0){ // caso p != 0, encontrou o numero e da heapify
			for(k=(A.indice-1)/2; k>=0;k--){
				heapify(V,A.indice,k);		
			}
			p = 0;
		}
		
		heapsort(V);	
				
	}
	
	for(i=0; i<(n%lim);i++){
			
			fscanf(input,"%d %d",&V[A.indice].numpac, &V[A.indice].tampac);
			
			
			for(j=0; j<V[A.indice].tampac;j++){
				fscanf(input,"%s ",V[A.indice].dados[j]);
			}
			A.indice++;

	}	
	for(k=(A.indice-1)/2; k>=0;k--){
			heapify(V,A.indice,k);		
	}	
		heapsort(V);
}
int main(int argc, char* argv[]) {
	// Ilustrando uso de argumentos de programa
	printf("Quantidade de argumentos: %i\n", argc);
	printf("Nome do programa: %s\n", argv[0]);
	printf("Arquivo de entrada: %s\n", argv[1]);
	printf("Arquivo de saida: %s\n", argv[2]);
	// Abrindo arquivos
	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w");
	
//	input = fopen("cabulosoHeap.txt","r");
//	output = fopen("output.txt","w");
	
	int ntotalpac, qtdpac;
	struct tPacote *pacotes;
	
	fscanf(input,"%d %d",&ntotalpac,&qtdpac);
	
	pacotes = (tPacote*)malloc(sizeof(tPacote)*ntotalpac);
	abrindoArquivo(pacotes,ntotalpac,qtdpac);
	
	pacotes = NULL;
	free(pacotes);	
	// Fechando arquivos
	fclose(input);
	fclose(output);
	// Finalizando programa	
		
return 0;
}

