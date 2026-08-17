#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int i=0;
pthread_mutex_t m;

void* counter(void* args) {
    for (int j=0; j<1000; j++) {
        pthread_mutex_lock(&m);
        i++;
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main() {
    pthread_t p1, p2, p3, p4, p5;

    pthread_create(&p1, NULL, counter, NULL);
    pthread_create(&p2, NULL, counter, NULL);
    pthread_create(&p3, NULL, counter, NULL);
    pthread_create(&p4, NULL, counter, NULL);
    pthread_create(&p5, NULL, counter, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_join(p3, NULL);
    pthread_join(p4, NULL);
    pthread_join(p5, NULL);

    printf("%d\n",i);
}