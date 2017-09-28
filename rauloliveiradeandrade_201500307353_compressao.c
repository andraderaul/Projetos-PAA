#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

FILE* input; // objeto da entrada 'input'
FILE* output; // objeto da saida 'output'

#define LIM 256 //MAXIMO HISTOGRAMA
#define MAXSIZE 10001
#define MAXSTRING 100000
#define TRUE 1
#define FALSE 0
int MAX_ATUAL = 0,SIZE = 0, setar = 0,sizeR = 0;
int acumulador = 0;
typedef unsigned int ui;
//typedef unsigned long long int ll;
//estrutura de nó
typedef struct tNo{

	ui freq; //frequencia
	int simb; //codigo do simbolo
	struct tNo* dir; //filho direito
	struct tNo* esq; //filho esquerdo

}tNo;

typedef struct fila_p_min{

	ui tam; //tamanho atual da fila
	ui capacidade; //capacidade maxima da fila
	tNo **heap; //vetor do tipo estrutura tNo

}fila_p_min;


typedef struct tTabela{ 
	int simb; //simbolo em hexa
	char *bits; //o valor desse simbolo em bits
	int hexa;
	int freq;
	int tam;
}tTabela;
typedef struct tRle{ //pra imprimir o Rle
	int len;
	int simb;
}tRle;
typedef struct tHuff{ //pra imprimir o huff
	int hexa;	
}tHuff;

tTabela* tab;
char* compHUF;
tRle* compRLE;
tHuff* codHuff;  
///ONDE COMEÇA O HEAP
void trocar(tNo** x, tNo** y){ //troca do ordenacao por insercao
	
	tNo* aux;	
	aux = *x;
	*x = *y;
	*y = aux;
} 
//pai
ui pai(int i){
	return ((i-1)/2);
} 
//filho esquerdo
ui fEsq(int i){
	return ((2*i)+1);
}
//filho direito
ui fDir(int i){
	return ((2*i)+2);
}
//heapifiy
void heapify(tNo**V, ui T, ui i ){
	tNo* aux;
	ui P = i;
	ui E = fEsq(i);
	ui D = fDir(i);
	
	if(E < T && V[E]->freq < V[P]->freq) P = E;
	if(D < T && V[D]->freq < V[P]->freq) P = D;
	
	if(P != i ){
		aux = V[i];
		V[i] = V[P];
		V[P] = aux;
		heapify(V,T,P);
	} 
}
void construir_heap(tNo** V, int n){
	for(int i = n/2; i>=0;i--){
		heapify(V,n,i);
	}
}
///ONDETERMINA O HEAP
///INICIO HUFFMAN
//criando fila
fila_p_min* criar_fila_p_min(){
	fila_p_min* fp_min = (fila_p_min*)malloc(sizeof(struct fila_p_min));  
	fp_min->tam = 0; //quando a fila começa tamanho é zero
	fp_min->capacidade = SIZE; //capacidade maxima da fila
	fp_min->heap = (tNo**) malloc (fp_min->capacidade*sizeof(tNo*)); //o heap é criado com a capacidade maxima
	return fp_min; //retorna fila criada
}
// alocando novo no
void inserir_NO(fila_p_min* fp_min, ui freq, ui simb, tNo* esq, tNo* dir){
		
	tNo* novo = (tNo*) malloc(sizeof(tNo));	 //cria um novo tNo para ser adicionado na fila
	novo->esq = esq; // novo no é conectado com os dois menos frequente ou NULL
	novo->dir = dir; // novo no é conectado com os dois menos frequente ou NULL
	novo->simb = simb; //novo no recebe o simbolo atual
	novo->freq = freq; //novo no recebe a frequencia total soma do dir e esq

	fp_min->heap[fp_min->tam] = novo; // no é adicionado no heap na posicao q ele entra na fila
	fp_min->tam++; // a fila cresceu		
	construir_heap(fp_min->heap,fp_min->tam); //inserir e chamar heapify	

}
// pra saber o tamanho da fila
int tamanho(fila_p_min* fp_min){
	return fp_min->tam;
}
//remover um tNo da fila
tNo* extrairNO(fila_p_min* fp_min){
	
	tNo* aux = fp_min->heap[0];	// auxiliar recebe o primeiro		
	fp_min->heap[0] = fp_min->heap[fp_min->tam - 1]; // troca o primeiro pelo ultimo
	fp_min->tam--; // decrementa a o tamanho da fila
	heapify(fp_min->heap,fp_min->tam,0);
	return aux;	//retorna o aux
}
//psicodelia 
tNo* construir_arvore(ui* V, ui* H, ui n){
	fila_p_min* fp_min = criar_fila_p_min(); //cria a fila
	for(ui i = 0; i < n; i++){
		inserir_NO(fp_min,H[i],V[i],NULL,NULL); // Adicione pela primeira vez na fila
	}	
	while(tamanho(fp_min) > 1){ // tamanho da fila é atualizado constantemente
		tNo* x = extrairNO(fp_min);	//remove e atualiza tamanho da fila
		tNo* y = extrairNO(fp_min);	// remove e atualiza tamanho da fila
		inserir_NO(fp_min,x->freq + y->freq,'\0',x,y); //adiciona os dois nos antigos em um novo no na fila
	}
	return extrairNO(fp_min); //retorna a 'arvore' gerada
}
// funcao checar se o no é folha
int ehFolha(tNo* root){
	return !(root->esq) && !(root->dir);
}
//winter is here
void compactar(ui* V){//COMPACTAR E IMPRIMIR
	if(setar == 0){//pro moves 2017
		char bits_8[9] = {0};
		int n = 0, simb = 0, k = 0,hexa;
		for(int i = 0; i < MAX_ATUAL; i++){
			simb = V[i];
			n = tab[simb].tam;	
			for(int j = 0; j < n; j++){
				bits_8[k] = tab[simb].bits[j];
				k++; //auxiliar pra saber se bateu os 8bits
				if(k==8){
					hexa = (int)strtol(bits_8,NULL,2); //FUNCAO QUE TRANSFORMA STR EM BIN
					if(i!= MAX_ATUAL-1)fprintf(output,"0x%02X ",hexa);
					else fprintf(output,"0x%02X ",hexa); 
					k = 0;
				}
			}
		}
		if(k){//se K né zero é pq ainda falta preencher os 8bits
			while(k<8){
				bits_8[k] = '0';
				k++;
			}
			hexa = (int)strtol(bits_8,NULL,2);//FUNCAO QUE TRANSFORMA STR EM BIN
			fprintf(output,"0x%02X",hexa);
			k = 0;
		}
		fprintf(output,"\n");
	}
	else{//pro moves 2017
		for(int i = 0; i < acumulador/8; i++){
			fprintf(output,"0x%02X ",0X00);
		}
		fprintf(output,"\n");	
	}
}
void salvarTAB(char* C, int simb, int n, int freq){ // salvar o percurso feito na arvore huffman
	//criando tabela
	tab[simb].freq = freq; //salvando o simbolo e sua frequencia na tab
	tab[simb].tam = n; // e o tamanho dos bits gastados
	for(int i = 0; i < n; i++){
		tab[simb].bits[i] = C[i]; //salvando os bits na tabela
	}
	tab[simb].bits[n] = '\0';
	acumulador+=n; //
}
//printar o percurso na arvore HUFFMAN
void caminharNaArvore(tNo* root, char* C, int top){
		
	// atribui zero a esquerda e continua a rec
	if(root->esq){
		C[top] = '0';
		caminharNaArvore(root->esq,C,top+1);
	}
	// atribui um a direita e continua a rec
	if(root->dir){
		C[top] = '1';
		caminharNaArvore(root->dir,C,top+1);	
	}
	// se for um no folha imprime os caracter contido em V
	if(ehFolha(root)){	
		salvarTAB(C,root->simb,top,root->freq);
	}
}
void huffman(ui* V, ui* H, ui* original, ui max){
	char* s = (char*)calloc(LIM,sizeof(char));
	memset(s,0,LIM);
	if(max>1){//sinal que não sao todos iguais
		
		tNo* root = construir_arvore(V,H,max);
		int index = 0;
		caminharNaArvore(root,s,index);
		setar = 0;		
		free(root);
	}
	else{//dibre pra nao calcular quando o cara é sempre igual
		acumulador = MAX_ATUAL;
		while(acumulador%8>0) acumulador++;
		setar = 1;
	}
	
	free(s);	
}
float porcentagemHuffman(ui* V){
	
	if(setar == 0){//continuacao do dibre
		int compressao = 0;
		for(int i = 0; i < MAX_ATUAL;i++){
			for(int j = 0; j < tab[V[i]].tam; j++){
				compressao++;
			}	
		}while(compressao%8>0)compressao++;
		float bits = (float)compressao;
		float n_bits = MAX_ATUAL * 8.0;
		float por = (bits/n_bits)*100.0; 
		return por;
	}
	else{
		float bits = (float)acumulador;
		float n_bits = MAX_ATUAL * 8.0;
		float por = (bits/n_bits)*100.0; 
		return por;
	
	}			
}
///FIM HUFFMAN
///INICIO RLE
float porcentgemRle(){
	float bits = (2*8)*sizeR; // 2 * 8bits * tamanho total do rle
	float n_bits = MAX_ATUAL*8;
	float por = (bits/n_bits)*100.0;
	return por;
}
void rle(ui* V){
	
	compRLE = (tRle*)calloc(MAX_ATUAL,sizeof(tRle));
	
	int len;
	int i=0,j=0;
	
	for(i=0, j=0; i < (int)MAX_ATUAL; i++,j++){
		len = 1;
		while(i+1 < (int)MAX_ATUAL && V[i] == V[i+1]){
			len++;
			i++;
		}
		compRLE[j].len = len; //freq
		compRLE[j].simb = V[i]; //simb
		sizeR = j+1;
	}
}
void imprimirRle(){	
	for(int i = 0; i < (int)sizeR; i++ ){
		if(i != sizeR-1)
			fprintf(output,"0x%02X 0x%02X ",compRLE[i].len,compRLE[i].simb);
		else
			fprintf(output,"0x%02X 0x%02X",compRLE[i].len,compRLE[i].simb);
		
	}
	fprintf(output,"\n");
}
///FIM RLE
void escolherCompactacao(int i,float x, float y, ui* V){
	
	if(x < y){ //HUFF
		fprintf(output,"%d: [HUF %.2f%%] ",i,x);
		compactar(V);
	}
	else if (y < x){ //RLE
		fprintf(output,"%d: [RLE %.2f%%] ",i,y);
		imprimirRle();
	}
	else{ // OS DOIS
		fprintf(output,"%d: [HUF %.2f%%] ",i,x);
		compactar(V);
		fprintf(output,"%d: [RLE %.2f%%] ",i,y);
		imprimirRle();
	}
}

void inicializar(ui* H){ // inicia todas as estruturas
	
	memset(compHUF,0,MAXSTRING);
	
	for(int i = 0; i < LIM; i++){
		
		H[i] = 0;
		tab[i].simb = 0;
		tab[i].bits = (char*)calloc(LIM,sizeof(char));
		memset(tab[i].bits,0,LIM);	
		
	}
}

int calculaHistograma(ui* simb, ui* freq, ui* H){ //preciso saber o tamanho do histo pra poder fazer alocacao da fila
	
	int j = 0;
	for(int i = 0; i < LIM; i++){
		if(H[i]>0){
			simb[j] = i; // alinhamento dos simbolos com o histograma
			freq[j] = H[i];
			j++;
		}		
	}	
	return j;	
}

void leitura(){
	int qtdseq = 0, t = 0, hex = 0, n = 0 ; 
	float x,y;
	compHUF = (char*)calloc(MAXSTRING, sizeof(char)); //onde irei salvar os bits do HUFF
	tab = (tTabela*)malloc(sizeof(tTabela)*LIM);
	
	ui* V = (ui*)calloc(MAXSIZE,sizeof(ui)); //vetor de sequencias
	ui* H = (ui*)calloc(LIM,sizeof(ui)); //histograma
	ui* freq = (ui*)calloc(LIM,sizeof(ui)); //pra poder deixar o H[i] sendo o histograma do V[i]
	ui* simb = (ui*)calloc(LIM,sizeof(ui)); // analogo
	inicializar(H);//valeu valgrind		
		
	fscanf(input,"%d", &qtdseq); //quantidade de sequencia
	for(int i = 0; i < qtdseq; i++){
		
		fscanf(input,"%d",&t);
		MAX_ATUAL = t;
		for(int j = 0; j < t; j++ ){
		
			fscanf(input,"%X ",&V[j]);
			hex = V[j];
			H[hex]++; 		
		}
		n = calculaHistograma(simb,freq,H);		
		SIZE = n;
		huffman(simb,freq,V,n); //chama compressao huffman	
		rle(V); //chama compressao rle
		
		x = porcentagemHuffman(V); //retorna a procentagem de compressao huff
		y = porcentgemRle(); // retorna porcentagem de compressao rle
		escolherCompactacao(i,x,y,V);
		
		for(int r = 0; r < LIM; r++){ //zerando histograma
			freq[r] = simb[r] = H[r] = 0;
			
		}
		memset(compHUF,0,MAXSTRING);
		acumulador = 0;
	}
	
	free(V);
	free(H);
	free(freq);
	free(simb);
	
}
int main(int argc, char* argv[]){
	
	//Ilustrando uso de argumentos de programa
	printf("#ARGS = %i\n", argc);
	printf("PROGRAMA = %s\n", argv[0]);
	printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);
	
	// Abrindo arquivos
	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w");
	leitura();
	free(compHUF);
	
	fclose(input);
	fclose(output);
	
return 0;	
}
