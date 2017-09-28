#include <iostream>
#include <fstream> //classe do arquivo
#include <cmath>
#include <stdlib.h>
#define SUB_CAD seq.tsubCadeia
#define CEM 100.0
using namespace std;

ifstream input; // objeto da entrada 'input'
ofstream output; // objeto da saida 'output'

typedef struct tDoenca{
	string cod; // codigo
	int numGen; // numero de genes
	string *genes; //genes
	float proba;
	
}tDoenca;

typedef struct tSeq{
	int tsubCadeia; // tamanho da subcadeia
	string dna; // cadeia completa
	int numDoen; //numero de doenças
}tSeq;

tSeq seq;
tDoenca* doencas; 

void copiar(tDoenca S[],tDoenca E[], int n){
	int i=0;	
	for(i=0;i<n;i++){
		S[i] = E[i];
	}
}
//intercalar do merge
void intercalar(tDoenca S[], tDoenca E[], int ini, int meio, int fim) {
	int i = ini, j = meio + 1, k = ini;
	
	while(i <= meio && j <= fim) {
		if(E[i].proba >= E[j].proba)
			S[k++] = E[i++];
		else 
			S[k++] = E[j++];
	}
	if(i > meio) 
		copiar(&S[k], &E[j], fim - j + 1);
	else
		copiar(&S[k], &E[i], meio - i + 1);
	
	copiar(&E[ini], &S[ini], fim - ini + 1);
}
//começa o merge
void mergesort(tDoenca S[], tDoenca E[], int ini, int fim) {
	int meio = ini + (fim - ini) / 2;
	if(ini < fim) {
		mergesort(S, E, ini, meio);
		mergesort(S, E, meio + 1, fim);
	}
	intercalar(S, E, ini, meio, fim);
}
void leituraArquivo(){
	
	input >> seq.tsubCadeia;
	input >> seq.dna;
	input >> seq.numDoen;
	
	doencas = new tDoenca[seq.numDoen];
	for(int i = 0; i < seq.numDoen; i++){
		input >> doencas[i].cod >> doencas[i].numGen;
			
		doencas[i].genes = new string[doencas[i].numGen];
		for(int j = 0; j < doencas[i].numGen; j++){
			input >> doencas[i].genes[j];			
		}
	}	
}
void escritaArquivo(){
	int n = seq.numDoen;
	for(int i = 0; i < n; i++){	
		output << doencas[i].cod<< ": "<< doencas[i].proba<<"%" << endl; 	
	}
}
void inicializarTab(int tab[], int m){
	for(int i = 0; i < m; i++)
		tab[i] = -1;
}
void kmpPreprocess(int tab[],string P){
	unsigned int i, m = P.size();
	int j = -1;
	inicializarTab(tab,m);
	for(i = 1; i < m; i++){
		while(j>=0 && P[j+1]!=P[i]) j = tab[j];
		if(P[j+1] == P[i]) j++;
		tab[i] = j;
	}
}
float combinacoes(float x, float y){
	/*	combinações que possuam o tamanho mínimo de
	*	subcadeia, com pelo menos 90% de compatibilidade
	*	total para manifestação da doença*/
	float hit;
	hit = ((float)(x-y)/(float)x)*CEM;	
return hit;
}
int probabilidade(float x, float y){
	/*	probabilidade de
	 *	manifestação da doença, de acordo com a
	 *	quantidade de genes detectados*/
	float hit;
	hit = ((float)y/(float)x)*CEM;	
return round(hit);
}
int compatibilidade(float x){
	/*	pelo menos 90% de compatibilidade
	 *	total para manifestação da doença*/
	if(x >= 90) return 1;
	return 0; 
}
float kmpSearch(int tab[], string T, string P, tDoenca V ){
	int temp = 0, tamAtual = 0;
	float hit = 0, acertos = 0;
	unsigned int i, n = T.size();
	unsigned int m = P.size();
	int j = -1;
	kmpPreprocess(tab,P);
		
	for(i = 0; i < n; i++){
		while(j>=0 && P[j+1] != T[i]){
			 j = tab[j];
		}
		if(P[j+1] == T[i]){
			j++;
			temp++; //variavel auxiliar 
		}
		else{
			if(temp >= SUB_CAD){ //auxiliar maior que o minimo
				string aux = P.substr(0,j+1); 
				P.erase(0,j+1); //remove as letras que deram hit
				kmpPreprocess(tab,P); //calcula tabela novamente
				j = -1; // j é reiniciado
			}			
		}
		temp = j+1;	//auxiliar sempre segue jota
		if(j == (int)m-1){ //MELHOR CASO E DA O BREAK
			string aux = P.substr(0,m);
			P.erase(0,m);
			j=tab[j];
			break;
		}
	}
	tamAtual = P.size(); // o resto da string
	hit = combinacoes(m,tamAtual); //combinacoes de gene
	acertos = compatibilidade(hit); // se > 90%
	return acertos; // retorna 1 ou 0
}
void processamento(){
	int *tab;
	float acertos;
	tDoenca *aux;
	string T = seq.dna;
	int n = seq.numDoen;
	for (int i = 0; i < n; i++){
		int m = doencas[i].numGen;
		for (int j = 0; j < m; j++){
			string P = doencas[i].genes[j];
			tab = new int[P.size()];
			acertos+= kmpSearch(tab,T,P,doencas[i]);
			doencas[i].proba = probabilidade((float)m,acertos);
		}
		acertos = 0;
	}
	
	aux = new tDoenca[n];	
	mergesort(aux,doencas,0,n-1);
	escritaArquivo();
	delete[] aux;
	delete[] tab;
}
int main(int argc, char* argv[]){
	
	//Ilustrando uso de argumentos de programa
	printf("#ARGS = %i\n", argc);
	printf("PROGRAMA = %s\n", argv[0]);
	printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);
	// Abrindo arquivos
	input.open(argv[1]);
	output.open(argv[2]);	
	
	leituraArquivo();
	processamento();	
	delete[] doencas; //desalocando memoria

return 0;
}
