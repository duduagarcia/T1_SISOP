#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>
#include <semaphore.h>

// Fila com seu tamanho máximo
#define MAX_ITEMS 10 
int fila[MAX_ITEMS];

int in = 0; // Posição de inserção
int out = 0; // Posição de remoção
int count = 0; // Contador de itens na fila

int num_threads = 2; 

sem_t mutex, availableItens, isEmpty;


void *producer(void *arg) {
    int item, tid;

    tid = (int)(long int)arg;

    while (1) {
        // Gera um aleatório
        item = rand() % 100; 

        sem_wait(&isEmpty); 
        sem_wait(&mutex); 

        // Critial session
        fila[in] = item;
        printf("Produtor %d adicionou o item %d na posição %d\n", tid, item, in);
        in = (in + 1) % MAX_ITEMS;
        count++;
        // End critical session

        sem_post(&mutex); // Sair da região crítica
        sem_post(&availableItens); // Sinalizar que há um item disponível na fila

        // Aguarda um tempo
        usleep(10000); 
    }
}

void *consumer(void *arg) {
    int item, tid;

    tid = (int)(long int)arg;

    while (1) {
        sem_wait(&availableItens); // Esperar até que haja itens na fila
        sem_wait(&mutex); // Entrar na região crítica

        // Critical Session
        item = fila[out];
        printf("Consumidor %d removeu o item %d da posição %d\n", tid, item, out);
        out = (out + 1) % MAX_ITEMS;
        count--;
        // End critical session

        sem_post(&mutex); // Sair da região crítica
        sem_post(&isEmpty); // Sinalizar que há espaço disponível na fila

        // Aguardar um tempo aleatório
        usleep(10000); 
    }
}

int main() {
    long int i;
    pthread_t *prod_threads, *cons_threads;

    prod_threads = malloc(sizeof(pthread_t) * num_threads);
    cons_threads = malloc(sizeof(pthread_t) * num_threads);

    // Iniciando os semáforos
    sem_init(&mutex, 0, 1);
    sem_init(&availableItens, 0, 0);
    sem_init(&isEmpty, 0, MAX_ITEMS);

    // Iniciando as threads
    for(i = 0; i < num_threads; i++) {
        pthread_create(&prod_threads[i], NULL, producer, (void *)i);
        pthread_create(&cons_threads[i], NULL, consumer, (void *)i);
    }

    pthread_exit(NULL);

    // Liberando a memória do malloc
    free(prod_threads);
    free(cons_threads);

    return 0;
}
