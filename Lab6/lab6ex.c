#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void* thread_work() {
    printf("--- ((( Hello, I am running inside the thread! ))) \n");
}

int main(void) {
    printf("Hello, I am the main thread!\n");
    int n_threads = 5;
    pthread_t tids[n_threads];
    for(int i = 0; i < n_threads; i++) {
        int ret = pthread_create(&tids[i], NULL, thread_work, NULL);
        if (ret < 0) {
            fprintf(stderr, "Failed tocreate thread!\n");
            exit(1);
        }
    }
    for(int i = 0; i < n_threads; i++) {
        pthread_join(tids[i], NULL);
    }
    return 0;
}
