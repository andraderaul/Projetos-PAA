#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//DEFINES
#define PP 0
#define PM 1
#define PA 2
#define HP 3
#define HM 4
#define HA 5
#define TECNICAS_LENGTH 6
#define INICIO 0
#define FIM v.size-1

//estrutura do vetor original
typedef struct vetores{ 
	
	int size;	
	int *vector;
	
}vetores;
//estrutura do vetor de tecnicas e suas variantes
typedef struct tecnicas{
	
	int variante;
	int *vector;
	int cont;
	char name[3];
		
}tecnicas;

void contador(int variante, tecnicas* K){//contadores
	switch(variante){
		case PP:
			K[PP].cont++;
		break;
		case PM:
			K[PM].cont++;
		break;
		case PA:
			K[PA].cont++;
		break;
		case HP:
			K[HP].cont++;
		break;
		case HM:
			K[HM].cont++;
		break;
		case HA:
			K[HA].cont++;
		break;
	}	

}
void swap(int *x, int *y, int variante, tecnicas* K){ //troca
	
	contador(variante,K); //cont
	
	int aux;	
	aux = *x;
	*x = *y;
	*y = aux;
} 
void ordenacao_por_insercao(int V[], int n) { //ordenacao por insercao
	
	
}
void ordenacaoStruct(tecnicas* V, int n) { //ordenacao por insercao
	
	int i, j;
	int aux;
	char str[3];
//	tecnicas aux;
	
	for(i = 1; i < n; i = i+1){
		for(j = i; j > 0 && V[j-1].cont > V[j].cont; j--){
			
			aux = V[j].cont;
			V[j].cont = V[j-1].cont;
			V[j-1].cont = aux;	
			
			strcpy(str,V[j].name);
			strcpy(V[j].name,V[j-1].name);
			strcpy(V[j-1].name,str);
			
		}
	}

}
void randomico(int V[], int inicio, int fim, int variante,tecnicas* K) { //randomico

	int a = V[inicio];	
	int aux;

	aux = abs(a);
	
	int i = inicio + (aux % (fim-inicio+1));
		switch(variante){
			case PA:			
				swap(&V[fim],&V[i],variante,K);
			break;
			case HA:
				swap(&V[inicio],&V[i],variante,K);
			break;	
		}
}
void mediana (int V[],int inicio,int fim,int variante, tecnicas* K){ //achando a mediana
	
	int n = (fim-inicio)+1;
	int a = (n/4)+inicio;
	int b = (n/2)+inicio;
	int c = ((3*n)/4)+inicio;
	
	int Y[] = {V[a],V[b],V[c]};
	
	
	//InsertionSort
	int i, j,aux;
	for(i = 1; i < 3; i = i+1){
		for(j = i; j > 0 && Y[j-1] > Y[j]; j--){
			aux = Y[j];
			Y[j] = Y[j-1];
			Y[j-1] = aux;
		}
	}
	switch(variante){
		case PM:
			
			if(Y[1]==V[a])
				swap(&V[a],&V[fim],variante,K);
			else if (Y[1]==V[b])
				swap(&V[b],&V[fim],variante,K);
			else 
				swap(&V[c],&V[fim],variante,K);		
		
		break;
		
		case HM:
		
			if(Y[1]==V[a])
				swap(&V[a],&V[inicio],variante,K);
			else if (Y[1]==V[b])
				swap(&V[b],&V[inicio],variante,K);
			else
				swap(&V[c],&V[inicio],variante,K);
		
		break;
	
	}
	
		
}
int hoare(int V[], int inicio, int fim,int variante, tecnicas* K) { //hoare
	
	int pivo = V[inicio];
	
	int i = inicio;
	int j = fim;
	
	while(i < j) {
		while(j > i && V[j] >= pivo) j--;
		while(i < j && V[i] < pivo) i++;
			if(i < j) swap(&V[i], &V[j],variante,K);
	}
return j;
}
int particionar(int V[], int inicio, int fim,int variante,tecnicas* K) { //particionar
	
	int pivo = V[fim];
		
	int i = inicio - 1, j;
	
	for(j = inicio; j < fim; j++) {
		if(V[j] <= pivo) {
			i = i + 1;
			swap(&V[i], &V[j],variante,K);
		}
	}
		swap(&V[i + 1], &V[fim],variante,K);
	return i + 1;
}
void quicksort(int V[], int inicio, int fim, int variante, tecnicas* K){ //quicksort
	
	int pivo;
	
	contador(variante,K); //cont
	
	if(inicio < fim) {
		switch(variante){
		
		case PP:
			pivo = particionar(V, inicio, fim, variante,K);
		break;
		
		case PM:
			mediana(V,inicio,fim,variante,K);
			pivo = particionar(V, inicio, fim, variante,K);
		break;
		
		case PA:
			randomico(V,inicio,fim,variante,K);
			pivo = particionar(V, inicio, fim, variante,K);
		break;
		case HP:
			pivo = hoare(V, inicio, fim,variante,K);
		break;
		
		case HM:
			mediana(V,inicio,fim,variante,K);
			pivo = hoare(V, inicio, fim,variante,K);
		break;
		
		case HA:
			randomico(V,inicio,fim,variante,K);
			pivo = hoare(V, inicio, fim,variante,K);
		break;	
	}//fim do switch
	
		if(variante == PP || variante == PM || variante == PA){
			quicksort(V, inicio, pivo - 1,variante,K);
			quicksort(V, pivo + 1, fim,variante,K);
		}
		else if (variante == HP || variante == HM || variante == HA){
			quicksort(V, inicio, pivo,variante,K);
			quicksort(V, pivo + 1, fim,variante,K);
		}
	}
}
void imprime(tecnicas* K, int index,int size,FILE* output){//imprime
	
	//-------------------//
	// ESCREVE NO ARQUIVO
	//-------------------//
	
	int i;
	
	ordenacaoStruct(K, TECNICAS_LENGTH);
	fprintf(output,"%d: ", index);
	fprintf(output,"N(%d) ",size);
	for(i = 0; i<TECNICAS_LENGTH; i++ ){
		fprintf(output,"%s(%d)",K[i].name,K[i].cont);
		if(i!=TECNICAS_LENGTH-1){
			fprintf(output," ");
		}
	}
	fprintf(output,"\n");
	
	/*
	//APAGAR DEPOIS
	printf("%d: ", index);
	printf("N(%d) ",size);
	for(i = 0; i<TECNICAS_LENGTH; i++ ){
		printf("%s(%d)",K[i].name,K[i].cont);
		if(i!=5){
			printf(" ");
		}
	}
	printf("\n");
	*/
	
}
void copiaVector(tecnicas* K, vetores v){ //copia v pra k	
	
	int i=0;		
	for(i=0;i<v.size;i++){
		//quicksort particionado
		K[PP].vector[i] = v.vector[i];
		K[PM].vector[i] = v.vector[i];
		K[PA].vector[i] = v.vector[i];
		//quicksort hoare
		K[HP].vector[i] = v.vector[i];
		K[HM].vector[i] = v.vector[i];
		K[HA].vector[i] = v.vector[i];		
	}

}
void chamaOrdenacao(tecnicas* K, vetores v ){//chama quicksort e quicksorthoare
	
	quicksort(K[PP].vector,INICIO,FIM,K[PP].variante,K);
	quicksort(K[HP].vector,INICIO,FIM,K[HP].variante,K);	
				
	quicksort(K[PM].vector,INICIO,FIM,K[PM].variante,K);
	quicksort(K[HM].vector,INICIO,FIM,K[HM].variante,K);
				
	quicksort(K[PA].vector,INICIO,FIM,K[PA].variante,K);
	quicksort(K[HA].vector,INICIO,FIM,K[HA].variante,K);

}
void addVariante(tecnicas* K){ //define as seis variantes
	
	//-------------------------------------//
	//	PP = PARTICIONAR PADRAO			   //
	//	PM = PARTICIONAR POR MEDIA DE 3    //
	//	PA = PATICIONAR POR PIVO ALEATORIO //
	//	HP = HOARE PADRAO				   //	
	//	HM = HOARE POR MEDIA DE 3		   //
	//	HA = HOARE POR PIVO ALEATÓRIO	   //
	//-------------------------------------//
	
		
		//quicksort particionado
		K[PP].variante = PP; 
		K[PM].variante = PM;
		K[PA].variante = PA;
		
		//quicksort hoare
		K[HP].variante = HP;
		K[HM].variante = HM;
		K[HA].variante = HA;
		
		//quicksort particionado
		strcpy(K[PP].name,"PP"); 
		strcpy(K[PM].name,"PM");
		strcpy(K[PA].name,"PA");
		
		//quicksort hoare
		strcpy(K[HP].name,"HP"); 
		strcpy(K[HM].name,"HM");
		strcpy(K[HA].name,"HA");
		
	
}
void zerandoContador(tecnicas* K){ //zerando os contadores antes de recomeçar o loop
	int i=0;
	for(i=0;i<TECNICAS_LENGTH;i++){
			K[i].cont = 0;
	}
}

void lerArquivo(FILE* input, FILE* output){
	
	//---------------//
	// LER O ARQUIVO
	//---------------//
	
	int vectorSize, i = 0, j = 0, index = 0;
	
	struct vetores v;
	struct tecnicas *K;	

	fscanf(input,"%d",&vectorSize); //numero total de vetores
		
	for(i = 0; i<vectorSize;i++){
		
		K = (tecnicas*)malloc(sizeof(tecnicas)*TECNICAS_LENGTH); // malloc das tecnicas		
		fscanf(input,"%d",&v.size); //tamanho do vetor		
		v.vector = (int*)malloc(sizeof(int)*v.size); 	//malloc do tamanho do vetor 		
		for(j=0;j<TECNICAS_LENGTH;j++){
			K[j].vector = (int*)malloc(sizeof(int)*v.size); //malloc nos vetores de tecnicas
		}		
		addVariante(K); //funcao q define as variantes e coloca os nomes
		for(j = 0; j<v.size;j++){
			fscanf(input,"%d",&v.vector[j]);
		}		
		copiaVector(K,v);	//copia do v para o k e suas seis variantes		
		chamaOrdenacao(K,v); //ordena as seis variantes existentes em K
		imprime(K, index,v.size,output);//imprime os contadores das seis variantes já ordenados
		zerandoContador(K); //zerando todos os contadores antes de recomeçar o laço
		index++;			
		K = NULL;
		free(K);			
	}
	
}
int main(int argc, char* argv[]) {
		
	// Ilustrando uso de argumentos de programa
	printf("Quantidade de argumentos: %i\n", argc);
	printf("Nome do programa: %s\n", argv[0]);
	printf("Arquivo de entrada: %s\n", argv[1]);
	printf("Arquivo de saida: %s\n", argv[2]);
	// Abrindo arquivos
	FILE* input = fopen(argv[1], "r");
	FILE* output = fopen(argv[2], "w");	
	
//	FILE* input = fopen("input.txt","r");
//	FILE* output = fopen("output.txt","w");
	lerArquivo(input,output);
	
	// Fechando arquivos
	fclose(input);
	fclose(output);
	// Finalizando programa
	
return 0;
}
