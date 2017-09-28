import sys

M = 1103515245
D = 12345
BITS24 = 3

#saida = open('output.txt','w')
saida = open(sys.argv[2],'w')

def G(key,s,A,B,y,j):
	tam = len(s)-1
	saida.write("%s->%s: "%(A,B))
	for i in range(tam):
		
		if y > BITS24:
			aux = key
			key = M * aux + D
			y = 0
			j = 0
		
		z = (key >> j) & 0xFF
		x = ord(s[i])
		saida.write("%d "%(z^x)) 
		y = y +1
		j = j + 8
	
	saida.write("\n")
	
	return key,y,j
	
def meuPOW(x,k,p):
	r = 1
	x = x % p
	while(k> 0):
		r = (r * x) % p
		k = k - 1
	return r	
		
def main(args):
	
	print("#ARGS = %i"%len((args)))
	print("PROGRAMA = %s"%(args[0]))
	print("ARG1 = %s, ARG2 = %s" %(args[1], args[2]));
	
	
	#arquivo = open('input.txt','r')
	arquivo = open(sys.argv[1],'r')
	primeiaLinha = arquivo.readline().split(' ')
	segundaLinha = arquivo.readline().split(' ')
	terceiraLinha = arquivo.readline().split(' ')
	quartaLinha = arquivo.readline().split(' ')
	
	a = str(primeiaLinha[0])
	numA = int(primeiaLinha[1])
	b = str(segundaLinha[0])
	numB = int(segundaLinha[1])
	dh = str(terceiraLinha[0])
	p = int(terceiraLinha[1])
	g = int(terceiraLinha[2])
	n = int(quartaLinha[0])	
	
#	A = meuPOW(g,numA,p) #(g**numA) % p #calculando a chave privada de A
#	B = meuPOW(g,numB,p) #(g**numB) % p #calculando a chave privada de B	
#	S = meuPOW(A,numB,p) #(A**numB) % p #calculando a chave publica de A e B 
	
	A = (g**numA) % p
	B = (g**numB) % p
	S = (A**numB) % p
	
	saida.write("%s->%s: %d\n"%(a,b,A))
	saida.write("%s->%s: %d\n"%(b,a,B))
	
	key = S
	key = M * key + D
	y = 0
	j = 0
	for i in range(n):
		m = arquivo.readline()
		s = str(m)
		key,y,j = G(key,s,a,b,y,j)
		a,b = b,a	
	
	
	arquivo.close()
	saida.close()

	return 0

main(sys.argv)


