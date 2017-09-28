#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <stdint.h>
using namespace std;

#define M 1103515245
#define D 12345
#define BITS24 3
ifstream input; // objeto da entrada 'input'
ofstream output; // objeto da saida 'output'

///BAILE FUNK RITO CULTURAL
uint32_t G(uint32_t key, string s,string A, string B) {
	uint32_t tam = s.size(),z;
	static unsigned int j = 0, y = 0;
	
	output << A<<"->" << B << ": ";
	for(uint32_t i = 0; i < tam; i++){	
		if(y > BITS24){
			uint32_t aux = key;
			key = M * aux + D;
			y = 0;j = 0;
		}		
		z = (key >> j) & 0xFF;
		output << (z^s[i]) << " ";
	//	cout << j <<" "<< ((int)s[i]^z) << endl;
		y++; // y++  é a mesma coisa de y+8 bits; se y > 4 é sinal q y > 24bits		
		j+=8;
	}
	output << endl;
	
	return key;
} 
///PROBLEMATIZADO LAZER DE GERAL
uint32_t meuPOW(uint32_t x, uint32_t k, uint32_t p){
	uint64_t r = 1;
	x %= p;
	while(k--){
		r = (r * x) % p;
	}		
	return r;
}
///QUEREM PROIBIR, TEM QUE ORGANIZAR 
void criptografia(){
	
	string strA, strB, strDH, m;
	uint32_t a = 0, b = 0, A = 0, B = 0, S = 0, n = 0;
	uint32_t p = 0, g = 0;
		
	input >> strA >> a; //strA pessoa As
	input >> strB >> b; //strB pessoa B
	input >> strDH >> p >> g; //strDH Diffie-Hellman
	input >> n; //numero de strings a ser criptografadas
	
	A = meuPOW(g,a,p); //calculando a chave privada de A
	B = meuPOW(g,b,p); //calculando a chave privada de B
	///calculando chave publica
	S = meuPOW(A,b,p); // tanto faz S = meuPOW(B,a,p); 
	output << strA<<"->" << strB << ": "<<A<< endl;
	output << strB<<"->" << strA << ": "<<B<< endl;
//	cout << S << endl;
	uint32_t key = S; //chave 
//	cout << key << endl;
	key = M * key + D; // M e D padrao do projetinho
	for(uint i = 0; i < n; i++){
		input >> m;		
		key = G(key,m,strA,strB);
		swap(strA,strB); //pra ficar alternando as mensagens
	}
	
}
///TODO MUNDO PERDE SE O BAILE NAO ROLAR
int main(int argc, char* argv[]){
	
	//Ilustrando uso de argumentos de programa
	printf("#ARGS = %i\n", argc);
	printf("PROGRAMA = %s\n", argv[0]);
	printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);
	
	///abrindo arquivo	
	input.open(argv[1]); 
	output.open(argv[2]);	
	
	criptografia();
	
	///fechando arquivo
	input.close();
	output.close();
	
	return 0;
}
