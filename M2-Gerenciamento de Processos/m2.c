// Nome: Júlia Faccio Zanette
// RA: 1916851


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ROWS 10
#define COLS 10

// Função para imprimir a matriz
void print_matriz(int matriz[][COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

// Função para transpor a matriz
void transposta(int matriz[][COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = i + 1; j < cols; j++) {
            int temp = matriz[i][j];
            matriz[i][j] = matriz[j][i];
            matriz[j][i] = temp;
        }
    }
}

// Função main ou principal
int main() {
    pid_t pid;
    int status;
    int matriz[ROWS][COLS] = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15, 16, 17, 18, 19, 20},
        {21, 22, 23, 24, 25, 26, 27, 28, 29, 30},
        {31, 32, 33, 34, 35, 36, 37, 38, 39, 40},
        {41, 42, 43, 44, 45, 46, 47, 48, 49, 50},
        {51, 52, 53, 54, 55, 56, 57, 58, 59, 60},
        {61, 62, 63, 64, 65, 66, 67, 68, 69, 70},
        {71, 72, 73, 74, 75, 76, 77, 78, 79, 80},
        {81, 82, 83, 84, 85, 86, 87, 88, 89, 90},
        {91, 92, 93, 94, 95, 96, 97, 98, 99, 100}
    };

    // Cria um novo processo com fork
    pid = fork();
    
    // Verifica se ocorreu um erro ao criar o processo filho
    if (pid == -1) {
        perror("Erro em executar a primitiva fork!");
        exit(1);
        
    } else if (pid == 0) {
        // Processo filho
        // Executa o comando curl em busca da palavra "brasil" no site do Uol
        char *args[] = {"curl", "-s", "http://www.uol.com.br", "|", "egrep", "-o", "brasil", "|", "wc", "-l", NULL};
        execvp(args[0], args);
        
        // Caso ocorra um erro ao executar o comando, imprime a mensagem de erro e finaliza o processo
        perror("Erro em executar a primitiva exec!");
        exit(1);
        
    } else {
        // Processo pai
        // Transpõe a matriz e imprime seu conteúdo na tela
        transposta(matriz, ROWS, COLS);
        print_matriz(matriz, ROWS, COLS);
        
        // Espera o processo filho terminar e imprime a saída do comando executado
        wait(&status);
        printf("Número de ocorrências da palavra \"brasil\" no site uol.com.br: %d\n", WEXITSTATUS(status));
    }

    return 0;
}

