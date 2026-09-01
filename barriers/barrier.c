#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

// create a barrier 
pthread_barrier_t barrier;

void* routine(void* arg){
    while(1){
    printf("Waiting for threads...\n");
    //sleep(1);
    pthread_barrier_wait(&barrier);
    printf("We passed the barrier...\n");
    sleep(1);
    }
}

int main(){
    pthread_barrier_init(&barrier,NULL,5);

    // create 10 threads and join them 
    pthread_t th[10];
    for (int i = 0; i < 10; i++)
    {
        if(pthread_create(&th[i],NULL,&routine,NULL) != 0) perror("Failed to create thread\n");
    }
    
    for(int i = 0; i<10;i++){
        if(pthread_join(th[i],NULL) != 0) perror("Failed to join!\n");
    }


    pthread_barrier_destroy(&barrier);
    return 0;
}