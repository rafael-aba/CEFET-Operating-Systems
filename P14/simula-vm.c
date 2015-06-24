#include <stdlib.h>
#include <stdio.h>
#include <limits.h>  // for INT_MAX

#define  MAXIMO 10000000
int numQua;
int n;

int exists(int valor, int *quadros);
int FIFO(int *vetor);
int LRU(int *vetor);
int OPT(int *vetor);
int achaPivoOPT(int i, int *quadros, int *vetor);
int existsVetor(int valor, int inicio, int *vetor);


int main(int argc,char *argv[]){
	int *vetor;
	int fFIFO, fOPT, fLRU, ref = 0;

	numQua = atoi(argv[1]);	

	vetor = malloc(sizeof(int)*MAXIMO);

	n = 0;
	while ( (! feof(stdin)) && (n < MAXIMO) ){
 		scanf ("%d\n", &vetor[n++]) ;
 		ref++;
	}

 	fFIFO = FIFO(vetor);
 	//printf("Terminou FIFO\n");
 	fLRU = LRU(vetor);
 	//printf("Terminou LRU\n");
 	fOPT = OPT(vetor);
 	//printf("Terminou OPT\n");

	printf ("%5d quadros, %7d refs: FIFO: %5d  PFs, LRU: %5d PFs, OPT: %5d PFs\n",numQua,ref,fFIFO,fLRU,fOPT);
	return 0;	
}

int exists(int valor, int *quadros){
	int i;
	for(i = 0; i < numQua; i++){
		if (quadros[i] == valor)
			return i;
	}
	return -1;
}

int FIFO(int *vetor){
	int *quadros;
	quadros = malloc(sizeof(int)*numQua);
	int falhas = 0;
	int pivo = 0;

	int i;
	for (i = 0; i < numQua; i++){
		quadros[i] = -1;
	}

	for(i = 0; i < n; i++){
		if(exists(vetor[i], quadros) == -1){
			quadros[pivo] = vetor[i];
			if (++pivo == numQua) pivo = 0;
			falhas++;
		}
	}
	return falhas;
}

int LRU(int *vetor){
	int *quadros;
	quadros = malloc(sizeof(int)*numQua);
	int falhas = 0;
	int pivo = 0;
	int idade[numQua];
	int pos;

	// Condição Inicial
	int i;
	for (i = 0; i < numQua; i++){
		quadros[i] = -1;
		idade[i] = 0;
	}

	// Para cada valor
	for(i = 0; i < n; i++){
		// Se valor já existir obter posição dele
		pos = exists(vetor[i], quadros);

		// Se valor não existir na lista
		if(pos == -1){
			// Procura primeiro quadro vazio
			pos = exists(-1, quadros);
			if (pos != -1){ // Se quadro vazio
				pivo = pos;
				quadros[pivo] = vetor[i];
				falhas++;

				// Atualiza Idade
				int j;
				for(j = 0; j < numQua; j++)
					if (j == pivo) 
						idade[j] = 0;
					else
						idade[j]++;

				//Atualiza Pivo
				if (++pivo == numQua){
					pivo = 0;
					for(j = 0; j < numQua; j++){
						if (idade[j] > idade[pivo])
							pivo = j;
					}
				}
			}else{ // Se não há quadro vazio
				quadros[pivo] = vetor[i];
				falhas++;

				// Atualiza Idade
				int j;
				for(j = 0; j < numQua; j++)
					if (j == pivo) 
						idade[j] = 0;
					else
						idade[j]++;

				// Proximo Pivo
				pivo = 0;
				for(j = 0; j < numQua; j++){
					if (idade[j] > idade[pivo])
						pivo = j;
				}
			}
		}else{ // Já existe na lista
			int j;
			for(j = 0; j < numQua; j++){
				if (quadros[j] == vetor[i]){
					pivo = j;
					idade[j] = 0;
				}else
					idade[j]++;
			}

			//Proximo Pivo
			pivo = 0;
			for(j = 0; j < numQua; j++){
				if (idade[j] > idade[pivo])
					pivo = j;
			}
		}
	}
	return falhas;
}

int OPT(int *vetor){
	int *quadros;
	quadros = malloc(sizeof(int)*numQua);
	int falhas = 0;
	int pivo = 0;
	int pos;

	// Condição Inicial
	int i;
	for (i = 0; i < numQua; i++){
		quadros[i] = -1;
	}

	// Para cada valor
	for(i = 0; i < n; i++){
		// Se valor já existir obter posição dele
		pos = exists(vetor[i], quadros);

		// Se valor não existir na lista
		if(pos == -1){
			// Procura primeiro quadro vazio
			pos = exists(-1, quadros);
			if (pos != -1){ // Se quadro vazio
				pivo = pos;
				quadros[pivo] = vetor[i];
				falhas++;

				//Atualiza Pivo
				if (++pivo == numQua){
					pivo = 0;
					pivo = achaPivoOPT(i, quadros, vetor);
				}
			}else{ // Se não há quadro vazio
				quadros[pivo] = vetor[i];
				falhas++;

				// Proximo Pivo
				pivo = achaPivoOPT(i, quadros, vetor);
			}
		}else{ // Já existe na lista
			//Proximo Pivo
			pivo = achaPivoOPT(i, quadros, vetor);
		}		
	}
	return falhas;
}

int achaPivoOPT(int i, int *quadros, int *vetor){
	int distMax = 0, retorno = -1;
	// Para cada quadro
	int k;
	for(k = 0; k < numQua; k++){
		// Pesquisa no vetor 
		int j;
		j = existsVetor(quadros[k],i+1,vetor);
		if (j == -1)
			return k;
		else{
			int dist = j - i;
			if (dist > distMax){
				distMax = dist;
				retorno = k;
			}
		}
	}
	return retorno;
}

int existsVetor(int valor, int inicio, int *vetor){
	int i;
	for(i = inicio; i < n; i++){
		if (vetor[i] == valor)
			return i;
	}
	return -1;
}
/*
CÓDIGO PARA TESTES
int k;
printf("%d -> ",vetor[i]);
for(k = 0; k < numQua; k++) printf("%d(%d) ",quadros[k],idade[k]);
printf("\n");
*/