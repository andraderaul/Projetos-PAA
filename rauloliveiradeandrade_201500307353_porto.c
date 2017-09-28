#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define O_CODIGO 1
#define O_PRIORIDADE 0
#define P_MENOR 0
#define P_MAIOR 1
#define PERCENTUAL 0.105

//ponteiro de arquivos
FILE *input = NULL;
FILE *output = NULL;
int pos = 0;

//estrutura dos contêineres
typedef struct tCarga{
	
	char codigo[12];
	char cnpj[19];
	float peso;
	int indice;
	
	//caso a carga seja de procedencia duvidosa 
	char cnpjErrado[19];
	char cnpjCerto[19];
	int prioridade;
	float porcentagem;
	float diferencaPeso;


}tCarga;
//binary search
int busca_bin(tCarga* vetor, int tamanho, char* valor) { //funcionando 10/10

	int inf = 0, sup = tamanho - 1;
	int i = (sup + inf) / 2;
	
	// Busca iterativa
	while(sup >= inf && strcmp(vetor[i].codigo,valor)!=0) {
		if(strcmp(vetor[i].codigo,valor)>0)
			sup = i - 1;
		else
			inf = i + 1;
		
		i = (sup + inf) / 2;
	}
	return (strcmp(vetor[i].codigo,valor) == 0) ? (i) : (-1);
}
void copiar(tCarga S[],tCarga E[], int n){ //funcionando 10/10
	int i=0;
	
	for(i=0;i<n;i++){
		S[i] = E[i];
	}

}
//intercalar do merge
void intercalar(tCarga S[], tCarga E[], int ini, int meio, int fim, int tipo) { //funcionando 10/10
	int i = ini, j = meio + 1, k = ini;
	
	while(i <= meio && j <= fim) {
		
		//ORDENANDO PELO CODIGO
		if(tipo == O_CODIGO){ // é usado no começo pra facilitar a binary search
			if(strcmp(E[i].codigo,E[j].codigo) < 0)
				S[k++] = E[i++];
			else 
				S[k++] = E[j++];	
		}
		//ORDENANDO POR PRIORIDADE
		if(tipo == O_PRIORIDADE){ //esse é usado no final
			if(E[i].prioridade > E[j].prioridade){// PRIORIDADES DIFERENTES
				S[k++] = E[i++];
			}
			else if(E[i].prioridade == E[j].prioridade){ //PRIORIDADES IGUAIS
				if(E[i].prioridade == P_MAIOR){ //P_MAIOR == CNPJ
					if(E[i].indice < E[j].indice){ // Checar quem veio antes
						S[k++] = E[i++];
					}
					else{ //veio depois
						S[k++] = E[j++];
					}
				}
				else if(E[i].prioridade == P_MENOR){ // P_MENOS == PESO
					if(E[i].porcentagem > E[j].porcentagem){ //checar quem é mais pesado
						S[k++] = E[i++];
					}
					else if(E[i].porcentagem == E[j].porcentagem){ // peso iguais
						if(E[i].indice < E[j].indice){ // Checar quem veio antes
							S[k++] = E[i++];
						}
						else{ //veio depois
							S[k++] = E[j++];
						}
					}
					else{ //pesos mais leves
						S[k++] = E[j++];
					}
				}
			}
			else{ //PRIORIDADES DIFERENTES 
				S[k++] = E[j++];
			}		
		}	
	}
	if(i > meio) 
		copiar(&S[k], &E[j], fim - j + 1);
	else
		copiar(&S[k], &E[i], meio - i + 1);
	
	copiar(&E[ini], &S[ini], fim - ini + 1);
}
//começa o merge
void mergesort(tCarga S[], tCarga E[], int ini, int fim, int tipo) { //funcionando 10/10
	int meio = ini + (fim - ini) / 2;
	if(ini < fim) {
		mergesort(S, E, ini, meio, tipo);
		mergesort(S, E, meio + 1, fim, tipo);
	}
	intercalar(S, E, ini, meio, fim, tipo);
}
//funcao generica
void lendoArquivo(tCarga* V, int length){ //my kawaii desu

	//---------------//
	// LER O ARQUIVO
	// --------------//
	
	int i = 0;
	for(i = 0; i<length; i++){
		V[i].indice = i;
		fscanf(input,"%s %s %f", V[i].codigo,V[i].cnpj,&V[i].peso);
	}	
}
void escrevendoArquivo(tCarga* S){ //me nota senpai
	
	//--------------------//
	// ESCREVER O ARQUIVO
	// -------------------//
	
	int i = 0;
	for(i =0;i<pos;i++){
		
		if(S[i].prioridade == P_MAIOR){
		//	printf("%s: %s<->%s\n", S[i].codigo, S[i].cnpjCerto, S[i].cnpjErrado);
			fprintf(output,"%s: %s<->%s\n", S[i].codigo, S[i].cnpjCerto, S[i].cnpjErrado);
			
		}
		else{
		//	printf("%s: %.f (%.f%%)\n", S[i].codigo,S[i].diferencaPeso, S[i].porcentagem);
			fprintf(output,"%s: %.fkg (%.f%%)\n", S[i].codigo,S[i].diferencaPeso, S[i].porcentagem);
		}
		
	}
	

}
//funcao para saber os que divergem por CNPJ e por PESO
void divergencias(tCarga T, tCarga* V, tCarga* S, int length){
	
	// ---------------------------------------------// 
	//	T = A CARGA SALVA ATUALMENTE 
	//	V = SAO AS CARGAS CADASTRADAS
	//	S = SALVAREI AS CARGAS QUE FOREM DIVERGENTES 
	// ---------------------------------------------// 

	int indice = 0;	 
	indice = busca_bin(V,length,T.codigo); //vai pegar o indice do cara
	
	if(strcmp(V[indice].codigo,T.codigo) == 0){ //comparando o codigo
		strcpy(S[pos].codigo, T.codigo);
		S[pos].indice = V[indice].indice;
		
		//calculo do peso
		float difPeso = fabs(V[indice].peso - T.peso)/V[indice].peso ; //salva a diferença do peso pra checar no if dps
	
		if(strcmp(V[indice].cnpj,T.cnpj)!=0){ //comparando os cnpj
			
			strcpy(S[pos].cnpjCerto,V[indice].cnpj);
			strcpy(S[pos].cnpjErrado,T.cnpj);
			S[pos].prioridade = P_MAIOR;
			pos++;
		}
		else if(difPeso > PERCENTUAL){ //vê se o peso é maior
			
			S[pos].diferencaPeso = fabs(V[indice].peso - T.peso); //a diferença do peso por KILO
			S[pos].porcentagem = round(difPeso * 100); // a porcentagem da diferença de peso
			S[pos].prioridade = P_MENOR;
			pos++;
		}
	}		
}
//funcao principal
int main(int argc, char* argv[]){
	
	// Ilustrando uso de argumentos de programa
	printf("#ARGS = %i\n", argc);
	printf("PROGRAMA = %s\n", argv[0]);
	printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);
	// Abrindo arquivos
	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w");	
	
	int n = 0, m = 0;
	
	tCarga *cadastrados;
	tCarga selecionados;
	tCarga *divergentes;
	tCarga *aux;
	
	
//	input = fopen("inputPort.txt","r");
//	output = fopen("output.txt","w");
	
	fscanf(input,"%d",&n);
	cadastrados = (tCarga*)malloc(sizeof(tCarga)*n+1); //alocando espaço do tamanho da entrada
	aux = (tCarga*)malloc(sizeof(tCarga)*n+1); //vetor aux pro merg
	lendoArquivo(cadastrados,n); //pegando os navios cadastrados //FUNCAO KAWAII DESU
	
	mergesort(aux,cadastrados,0,n-1, O_CODIGO); //ordenacao pra ajudar na binary search
	
	//printando os conteiners ja cadastrados
/*	printf("O QUE VEIO DO ARQUIVO!!\n");
	for(i=0;i<n;i++){
		printf("%s %s %.f\n", cadastrados[i].codigo,cadastrados[i].cnpj, cadastrados[i].peso);
	};
	printf("\n");*/	
	fscanf(input,"%d",&m);
	divergentes = (tCarga*)malloc(sizeof(tCarga)*m);
	
	//pegando os navios selecionados
	int j = 0;	
	for(j=0;j<m;j++){
		fscanf(input,"%s %s %f",selecionados.codigo, selecionados.cnpj,&selecionados.peso);
		divergencias(selecionados,cadastrados,divergentes,n);
	//	printf("%s %s %f\n", selecionados[j].codigo, selecionados[j].cnpj, selecionados[j].peso);
	
	}
	
	mergesort(aux,divergentes,0,pos-1,O_PRIORIDADE);
	printf("\n");	
	
	//printando a saida
//	printf("MINHA SAIDA!!\n");
	escrevendoArquivo(divergentes);
	
	
	//liberando espaço alocado	
	cadastrados = NULL;
	free(cadastrados);
//	selecionados = NULL;
//	free(selecionados);
	//fechando arquivos
	fclose(input);
	fclose(output);
return 0;
}

