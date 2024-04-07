#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_PORCOES 5

// Declaração dos semáforos e variáveis compartilhadas
sem_t mutex, vazio, cheio; // Semáforos
int porcoes_na_travessa = MAX_PORCOES; // Variável compartilhada que indica o número de porções na travessa

// Função executada pelas threads canibais
void *canibal(void *arg) {
    int tid = (int)(long int)arg;

    while (1) {
        sem_wait(&mutex); // Entrar na seção crítica
        if (porcoes_na_travessa > 0) {
            porcoes_na_travessa--; // Servir uma porção
            printf("Canibal %d se serviu e está comendo. Porções restantes: %d\n", tid, porcoes_na_travessa);
            sem_post(&mutex); // Sair da seção crítica
            usleep(10000); // Aguardar um tempo antes de continuar
        } else {
            printf("Canibal %d encontrou a travessa vazia. Acordando o cozinheiro...\n", tid);
            sem_post(&vazio); // Sinalizar que a travessa está vazia e acordar o cozinheiro
            sem_wait(&cheio); // Aguardar até que a travessa seja reabastecida
        }
    }
    return NULL;
}

// Função executada pela thread cozinheiro
void *cozinheiro(void *arg) {
    while (1) {
        sem_wait(&vazio); // Aguardar até que a travessa esteja vazia e precise ser reabastecida
        printf("Cozinheiro preparando %d porções...\n", MAX_PORCOES);
        porcoes_na_travessa = 5; // Reabastecer a travessa com o número máximo de porções
        sem_post(&cheio); // Sinalizar que a travessa está cheia
        sem_post(&mutex); // Sair da região crítica
    }
    return NULL;
}

// Função principal
int main() {
    int num_canibais = 5; // Número de canibais

    pthread_t *threads_canibais = malloc(sizeof(pthread_t) * num_canibais);
    pthread_t thread_cozinheiro;

    // Inicialização dos semáforos
    sem_init(&mutex, 0, 1);
    sem_init(&vazio, 0, 0);
    sem_init(&cheio, 0, 0);

    // Criação das threads canibais
    for (int i = 0; i < num_canibais; i++) {
        pthread_create(&threads_canibais[i], NULL, canibal, (void *)(long int)i);
    }

    // Criação da thread cozinheiro
    pthread_create(&thread_cozinheiro, NULL, cozinheiro, NULL);

    // Espera pelas threads terminarem
    for (int i = 0; i < num_canibais; i++) {
        pthread_join(threads_canibais[i], NULL);
    }
    pthread_join(thread_cozinheiro, NULL);

    // Liberação da memória alocada
    free(threads_canibais);

    return 0;
}