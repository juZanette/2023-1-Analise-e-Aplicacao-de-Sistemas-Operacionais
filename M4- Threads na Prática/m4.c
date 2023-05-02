
//Nome: Júlia Faccio Zanette
//RA: 1916851

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_THREADS 4
#define N 5000000

int proximo_num = 2;
int contagem_primos = 0;
int contagem_primos_por_thread[N_THREADS];
pthread_mutex_t mutex;

// Função que verifica se um número é primo ou não
void *eh_primo(void *arg) {
    int eh_primo, num;
    int thread_id = *(int *) arg;
    contagem_primos_por_thread[thread_id] = 0;

    while (1) {
        // Bloqueia o acesso a próxima variável global "next_num" usando um mutex
        pthread_mutex_lock(&mutex);
        
        if (proximo_num > N) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        num = proximo_num;
        proximo_num++;
        pthread_mutex_unlock(&mutex);

        eh_primo = 1;
        for (int i = 2; i <= num / 2; i++) {
            if (num % i == 0) {
                eh_primo = 0;
                break;
            }
        }

        if (eh_primo) {
            // Bloqueia o acesso a contagem de primos usando um mutex.
            pthread_mutex_lock(&mutex);
            contagem_primos++;
            contagem_primos_por_thread[thread_id]++;
            pthread_mutex_unlock(&mutex);
            printf("Thread %d encontrou um número primo: %d (total: %d)\n", thread_id, num, contagem_primos);
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[N_THREADS];
    int thread_args[N_THREADS];

    // Inicializa o mutex
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < N_THREADS; i++) {
        thread_args[i] = i;
        if (pthread_create(&threads[i], NULL, eh_primo, &thread_args[i])) {
            perror("Falha ao criar thread");
            return 1;
        }
    }

    for (int i = 0; i < N_THREADS; i++) {
        if (pthread_join(threads[i], NULL)) {
            perror("Falha ao juntar thread");
            return 1;
        }
    }

    // Finaliza o mutex
    pthread_mutex_destroy(&mutex);

    printf("Encontrados %d números primos\n", contagem_primos);
    for (int i = 0; i < N_THREADS; i++) {
        printf("Thread %d encontrou %d números primos\n", i, contagem_primos_por_thread[i]);
    }

    pthread_exit(NULL);
}

