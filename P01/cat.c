#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(){
	pid_t pid;
	// Cria um processo filho
	pid = fork();
	if (pid < 0) { //erro
		fprintf(stderr,"fork falhou");
		return 1;
	}else if(pid == 0) //processo  filho
		execlp("/bin/cat","cat",NULL);
	else { // Processo pai
		//Pai espera filho terminar
		wait(NULL);
		printf("filho terminou");
	}
	return 0;
}
