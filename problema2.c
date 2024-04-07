#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_PORCOES 5

sem_t mutex, vazio, cheio;
int porcoes_na_travessa = MAX_PORCOES;

void *canibal(void *arg) {
    int tid = (int)(long int)arg;

    while (1) {
        sem_wait(&mutex);
        if (porcoes_na_travessa > 0) {
            porcoes_na_travessa--;
            printf("Canibal %d se serviu e está comendo. Porções restantes: %d\n", tid, porcoes_na_travessa);
            sem_post(&mutex);
            usleep(10000); 

        } else {
            printf("Canibal %d encontrou a travessa vazia. Acordando o cozinheiro...\n", tid);
            sem_post(&vazio);
            sem_wait(&cheio);
        }
    }
    return NULL;
}

void *cozinheiro(void *arg) {
    while (1) {
        sem_wait(&vazio);
        printf("Cozinheiro preparando %d porções...\n", MAX_PORCOES);
        porcoes_na_travessa = 5;
        sem_post(&cheio);
        sem_post(&mutex);
    }
    return NULL;
}

int main() {
    int num_canibais = 5; // Número de canibais

    pthread_t *threads_canibais = malloc(sizeof(pthread_t) * num_canibais);
    pthread_t thread_cozinheiro;

    sem_init(&mutex, 0, 1);
    sem_init(&vazio, 0, 0);
    sem_init(&cheio, 0, 0);

    for (int i = 0; i < num_canibais; i++) {
        pthread_create(&threads_canibais[i], NULL, canibal, (void *)(long int)i);
    }
    pthread_create(&thread_cozinheiro, NULL, cozinheiro, NULL);

    for (int i = 0; i < num_canibais; i++) {
        pthread_join(threads_canibais[i], NULL);
    }
    pthread_join(thread_cozinheiro, NULL);

    free(threads_canibais);


    return 0;
}
