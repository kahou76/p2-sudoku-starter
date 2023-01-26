#include <pthread.h> // pthread*
#include <semaphore.h> // sem*
#include <stdio.h> // printf
#include <unistd.h>
#include <stdlib.h> // clock

void* fun(void* arg) {
    char* str = (char*)arg;
    double* pd = malloc(sizeof(double));
    *pd = atof(str);
    return pd;
}
int main() {
    pthread_t tid;
    pthread_create(&tid, NULL, fun, "1.234");
    double* pdm;
    pthread_join(tid, (void **) &pdm);
    printf("d = %f \n", *pdm);
    return 0;
}