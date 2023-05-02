
//Nome: Júlia Faccio Zanette
//RA: 1916851

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//Abre o navegador Firefox com a URL passada como parâmetro
void open_firefox(char* url){
    if (url == NULL) {
        printf("URL não informada!\n");
        exit(1);
    }
    char command[256];
    sprintf(command, "firefox %s &", url);
    system(command);
}

//Lida com o sinal SIGUSR2 e solicita ao usuário uma URL
void sigusr2_handler(int signum){
    char url[256];
    printf("Digite a URL do site que você deseja abrir no navegador Firefox: ");
    scanf("%s", url);
    open_firefox(url);
}

//É criada a estrutura que contém o handler para o sinal SIGUSR2
int main(){
    struct sigaction sa;
    sa.sa_handler = sigusr2_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("ERRO - Configuração do handler para o sinal SIGUSR2 não pôde ser feita");
        exit(1);
    }

//É criado um pipe, e criada uma função pai e filho usando fork
    pid_t pid;
    int fd[2];
    if (pipe(fd) == -1) {
        perror("ERRO - Processo pipe não pôde ser criado!");
        exit(1);
    }

    if ((pid = fork()) == -1) {
        perror("ERRO - Processo filho não pôde ser criado");
        exit(1);
    } 
    
    // Processo filho    
    else if (pid == 0) {
        close(fd[1]);
        sleep(1);
        char buffer[1];
        while (read(fd[0], buffer, 1) > 0);
        printf("Sinal SIGUSR2 recebido!\n");
        close(fd[0]);
	} 
	
	// Processo Pai
	else {
        close(fd[0]);
        printf("Seja bem-vindo(a)!\nPressione ENTER para enviar o sinal SIGUSR2!\n");
        getchar();
        if (kill(pid, SIGUSR2) == -1) {
            perror("ERRO - Sinal SIGUSR2 não foi enviado!");
            exit(1);
        }
        printf("Sinal SIGUSR2 enviado!\n");
        close(fd[1]);
        wait(NULL);
    }

    return 0;
}

