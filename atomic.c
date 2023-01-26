#include <stdio.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUMTHREADS 10
#define LOOP 100000

_Atomic int acount; // atomic count
int ncount; // non-atomic count

void* fun(void* arg) { // pthread worker
    for(int n = 0; n < LOOP; ++n) {
        ++ncount; // race!
        ++acount; // atomic
    }
    return NULL;
}
int main(void) {
    pthread_t tids[NUMTHREADS];
    for(int n = 0; n < NUMTHREADS; ++n) pthread_create(&tids[n], NULL, fun, NULL);
    for(int n = 0; n < NUMTHREADS; ++n) pthread_join(tids[n], NULL);
    printf("Atomic count = %d \n", acount);
    printf("Non-Atomic count = %d \n", ncount);
    return 0;
}