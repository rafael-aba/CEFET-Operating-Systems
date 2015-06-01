/*
CENTRO FEDERAL DE EDUCAÇÃO TECNOLÓGICA DE MINAS GERAIS
Redes de Computadores 1


Aluno: Rafael Antunes Batista
Professor: Sandro Renato Dias


*/
#include <stdio.h>

int potencia (int a, int b){
	int i, sol = 1;
	for (i = 0; i < b; i++)
		sol *= a;
	return sol;
}

int conv (int a[]){
	int i;
	double sol = 0;
	for(i = 0; i < 8; i++){
		if (a[i] == 1)
			sol += potencia(2.0,7-i);
	}
	return (int) sol;
}

int calcFaixa(int numMask){
	if(potencia(2,numMask) == 256) return 0;
	else return (potencia(2,8-numMask) - 2);

}

void printData(int numMask){
	char s[6] = "x.x.x.";
	int mascara[8];
	int i, rede, broadcast, inicial, final, tamFaixa, subRede;
	numMask = numMask-24;

	for (i = 0; i < 8; i++){
		if (i < numMask ) mascara[i] = 1;
		else mascara[i] = 0;
	}

	printf("Para mascara 255.255.255.%d:\n",conv(mascara));

	tamFaixa = calcFaixa(numMask);
	subRede = 0;
	for (i = 0; i < 255; i++){
		if(tamFaixa == 0){
			printf("\tEsta mascara nao possui subredes nem IP validos.\n");
			break;
		}
		else{
			rede = i;
			inicial = rede + 1;
			final = rede + tamFaixa ;
			broadcast = final + 1;

			printf("\tSubrede\t%d\n",++subRede);
			printf("\t\tRede:\t\t%s%d\n",s,rede);
			printf("\t\tInicial:\t%s%d\n",s,inicial);
			printf("\t\tFinal:\t\t%s%d\n",s,final);
			printf("\t\tBroadcast:\t%s%d\n",s,broadcast);

			i = broadcast;
		}
	}
	printf("\tTotal de subredes: %d\n",subRede);
	printf("\tTotal de IPs Validos: %d\n" ,((final - inicial) + 1)*subRede);

	printf("\n");
}

void main(){
	int numMask;

	printf("Entre com mascara a ser testada: x.x.x.x/");
	while (scanf("%d",&numMask) != 0){
		if (numMask == 0) break;
		else if (numMask > 31) printf("Valor de mascara invalido.\n");
		else if (numMask < 24) printf("Valor de mascara nao afeta somente o primeiro octeto.\n");
		else printData(numMask);
		printf("Para sair, entre zero\nEntre com mascara a ser testada: x.x.x.x/");
	}
	printf("\n");
}
