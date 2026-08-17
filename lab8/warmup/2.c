#include <stdio.h>
#include <pthread.h>

void *print_thread(void* arg) {
    int k = *(int *)arg;
    printf("I am thread %d\n",k);

    return NULL;
}

int main() {
    
    int n;
    scanf("%d",&n);
    pthread_t p[n];

    for (int i=0; i<n; i++) {
        int j = i+1;
        pthread_create(&p[i], NULL, print_thread, (void *)&j);
    }

    for (int i=0; i<n; i++) {
        pthread_join(p[i],NULL);
    }
}