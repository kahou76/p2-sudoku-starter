#include <pthread.h> // pthread*
#include <semaphore.h> // sem*
#include <stdio.h> // printf
#include <unistd.h>
#include <time.h> // clock

#define NUMTHREADS 9

pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void Report(char* msg) {
    pthread_mutex_lock(&printMutex);
    printf(msg);
    pthread_mutex_unlock(&printMutex);
}

void* child(void* arg) {
    Report("Child: starting \n");
    sleep(3); // simulate working
    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    Report("Child: finishing \n");
    return NULL;
}
int main() {
    pthread_t tid;
    Report("Parent: launching child \n");
    pthread_create(&tid, NULL, child, NULL);
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);
    Report("Parent: child done \n");
    return 0;
}