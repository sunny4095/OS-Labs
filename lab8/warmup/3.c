#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int curr_thread = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

void *slp(void* arg) {
    int n_thread = *(int *)arg;
    int seed = time(NULL) ^ n_thread;
    int slp_time = (rand_r(&seed) % 10) + 1;
    sleep(slp_time);

    pthread_mutex_lock(&m);
    while (curr_thread != n_thread) {
        pthread_cond_wait(&cv, &m);
    }
    printf("I am thread %d\n", n_thread);
    curr_thread = n_thread + 1;
    pthread_cond_broadcast(&cv);
    pthread_mutex_unlock(&m);
}

int main(int argc, char *argv[]) {

    int n = atoi(argv[1]);
    pthread_t p[n];
    int tn[n];

    for (int i=0; i<n; i++) tn[i]=i;
    for (int i=0; i<n; i++) pthread_create(&p[i],NULL,slp,(void *)&tn[i]);
    for (int i=0; i<n; i++) pthread_join(p[i],NULL);
}



