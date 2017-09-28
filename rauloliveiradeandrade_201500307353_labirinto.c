#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 110
#define TRUE 1
#define FALSE 0
#define CAMINHO '0'
#define SAIDA '$'
#define VISITADO '1'
#define DIREITA coluna + 1
#define ESQUERDA coluna -1
#define FRENTE linha - 1
#define TRAS linha + 1
#define MAPA lab.mapa

FILE* input; // objeto da entrada 'input'
FILE* output; // objeto da saida 'output'


typedef struct tCoordenada{
	
	int x;
	int y;

}tCoordenada;
typedef struct tLab{
	
	tCoordenada jogador;
	tCoordenada vitoria;
	char **mapa;
	int m;
	int n;

}tLab;

tLab lab;

//SEGUE O BAILE
int posicaoValida(int linha, int coluna){
	if(linha > lab.n-1 || coluna > lab.m-1 || linha < 0 || coluna < 0) //pra nao passar pra fora do mapa
		return FALSE;
	return TRUE;
}
int andar(int linha, int coluna){
	
	if((MAPA[linha][coluna] == CAMINHO) || (MAPA[linha][coluna] == SAIDA)) //para o boneco andar
		return TRUE;
	return FALSE;	
	
}
//O BAILE NAO VAI MORRER
int encontrarSaida(int linha, int coluna){
	int achou = FALSE;
	if(MAPA[linha][coluna] == SAIDA){ 
		return TRUE; //CASO BASE DA RECURSAO
	}
	else{
		MAPA[linha][coluna] = VISITADO; //marque o vertice como visitado
		achou = FALSE; 
		
		if(!achou && posicaoValida(linha,DIREITA) && andar(linha,DIREITA)){
			fprintf(output,"D [%d,%d]->[%d,%d]\n",linha,coluna,linha,DIREITA);
			achou = encontrarSaida(linha,DIREITA); //PRIMEIRO VAMOS PELA DIREITA
			//ainda nao encontrou, a direita é saida ou caminho
			if(!achou)fprintf(output,"BT [%d,%d]<-[%d,%d]\n",linha,coluna,linha,DIREITA); //na volta da recursao tem o BT
		}
		if(!achou && posicaoValida(FRENTE,coluna) && andar(FRENTE,coluna)){
			fprintf(output,"F [%d,%d]->[%d,%d]\n",linha,coluna,FRENTE,coluna);
			achou = encontrarSaida(FRENTE,coluna); //SEGUNDO VAMOS PELA FRENTE
			// ainda nao encontrou, a frente é saida ou caminho 
			if(!achou)fprintf(output,"BT [%d,%d]<-[%d,%d]\n",linha,coluna,FRENTE,coluna); //na volta da recursao tem o BT
		}
		if(!achou && posicaoValida(linha,ESQUERDA) && andar(linha,ESQUERDA)){
			fprintf(output,"E [%d,%d]->[%d,%d]\n",linha,coluna,linha,ESQUERDA);
			achou = encontrarSaida(linha,ESQUERDA); // TERCEIRO VAMOS PELA ESQUERDA
			//ainda nao encontrou, a esquerda é saida ou caminho
			if(!achou)fprintf(output,"BT [%d,%d]<-[%d,%d]\n",linha,coluna,linha,ESQUERDA); //na volta da recursao tem o BT
		}
		if(!achou && posicaoValida(TRAS,coluna) && andar(TRAS,coluna)) {
			fprintf(output,"T [%d,%d]->[%d,%d]\n",linha,coluna,TRAS,coluna);
			achou = encontrarSaida(TRAS,coluna); // QUARTO VAMOS PELA TRAS
			//ainda nao encontrou, a tras é saida ou caminho
			if(!achou)fprintf(output,"BT [%d,%d]<-[%d,%d]\n",linha,coluna,TRAS,coluna); //na volta da recursao tem o BT
		}
		
		return achou;
	}
}
void iniciarLab(int n, int m){
	
	lab.vitoria.x = 0;
	lab.vitoria.y = 0;
	lab.jogador.x = 0;
	lab.jogador.y = 0;
	lab.m = m;
	lab.n = n;
	
	MAPA = (char**)calloc(n,sizeof(char*));
	for(int i = 0; i < n; i++)
			MAPA[i] = (char*)calloc(m,sizeof(char));
		
	

}

void lerMapa(){
		
	int k = 0, n = 0, m = 0,resultado = 0;
	
	fscanf(input,"%d",&k);
	for(int a = 0; a < k; a++){
		fscanf(input,"%d %d",&m,&n);
		
		lab.m = m;
		lab.n = n;
		iniciarLab(n,m);		
				
		for(int i = 0; i < n; i++ ){
			for(int j = 0; j < m; j++){		
				
				fscanf(input," %c", &MAPA[i][j]);
				if(MAPA[i][j] == 'X'){ //descobrindo onde inicia 
					lab.jogador.x = i;
					lab.jogador.y = j;				
				}
				if((MAPA[i][0] == '0') || (MAPA[0][j] == '0')
				|| (MAPA[n-1][j] == '0') || (MAPA[i][m-1] == '0')){ //descobrindo onde termina
					lab.vitoria.x = i;
					lab.vitoria.y = j;
					MAPA[i][j] = SAIDA;
				}				
			}
		}
		fprintf(output,"L%d:\nINICIO [%d,%d]\n",a,lab.jogador.x,lab.jogador.y);
		resultado = encontrarSaida(lab.jogador.x,lab.jogador.y); 
		if(resultado) fprintf(output,"SAIDA [%d,%d]\n",lab.vitoria.x,lab.vitoria.y);
		else fprintf(output,"SEM SAIDA\n");	
	}
	
}
int main(int argc, char* argv[]){
	
	//Ilustrando uso de argumentos de programa
	printf("#ARGS = %i\n", argc);
	printf("PROGRAMA = %s\n", argv[0]);
	printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);
	
	// Abrindo arquivos
	input = fopen(argv[1], "r");
	output = fopen(argv[2], "w");
	
	lerMapa();
	
	fclose(input);
	fclose(output);
return 0;
}
