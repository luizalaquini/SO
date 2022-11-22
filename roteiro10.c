#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>

int TAM 500

int main(){

    int buffer[TAM]; // inicialilza array
    for (int i=0; i<TAM; i++) buffer[i] = 0; // preenche com zeros

    // Cria Threads:
    int threadA = pthread_create();
    int threadB = pthread_create();
    int threadC = pthread_create();
    int threadD = pthread_create();
    int threadE = pthread_create();
}