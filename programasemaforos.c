#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Definimos las macros P y V basadas en tu apunte teórico
#define P(s) sem_wait(s)
#define V(s) sem_post(s)

sem_t mutex_critico;

void* tarea_hilo(void* arg) {
    int id = *((int*) arg);

    for (int i = 1; i <= 10000; i++) {
        // Cuando llega a 4000, aplicamos P(s) para entrar a la sección crítica
        if (i == 4000) {
            printf("[HILO %d] Llegó a 4000. Aplicando P(s) (Bloqueando exclusión mutua)...\n", id);
            P(&mutex_critico);
            printf("[HILO %d] Entró a la sección crítica (sin interrupciones hasta 6000).\n", id);
        }

        printf("[HILO %d] Cuenta: %d\n", id, i);

        // Cuando llega a 6000, aplicamos V(s) para salir de la sección crítica
        if (i == 6000) {
            printf("[HILO %d] Llegó a 6000. Aplicando V(s) (Liberando semáforo)...\n", id);
            V(&mutex_critico);
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hilo1, hilo2;
    int id1 = 1, id2 = 2;

    // Inicialización del semáforo en 1 (como se indica en el apunte)
    sem_init(&mutex_critico, 0, 1);

    pthread_create(&hilo1, NULL, tarea_hilo, &id1);
    pthread_create(&hilo2, NULL, tarea_hilo, &id2);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    sem_destroy(&mutex_critico);

    printf("¡Proceso finalizado correctamente!\n");
    return EXIT_SUCCESS;
}