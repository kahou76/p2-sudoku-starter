#include <stdio.h> // printf
#include <pthread.h> // pthread
#include <unistd.h> // usleep
// LostUpdates.c
// Build: gcc -std=c11 -l pthread LostUpdates.c
// Run: a.exe (Windows);
// ./a.out (Linux)
int gcount = 0;

void* bump(void* arg) {
    for (int i = 1; i <= 5000; ++i) {
        if (i % 100 == 0) usleep(5);
            ++gcount;
    }
return NULL;
}
int main() {
pthread_t bumper1, bumper2, bumper3;
pthread_create(&bumper1, NULL, bump, NULL);
pthread_create(&bumper2, NULL, bump, NULL);
pthread_create(&bumper3, NULL, bump, NULL);
printf("Final value of gcount = %d \n", gcount);
return 0;
}