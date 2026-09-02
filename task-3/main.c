#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>


#define BUFFER_SIZE 5
// create the mutex 
pthread_mutex_t mutex;
// create the condition variables 
pthread_cond_t cond_not_full,cond_not_empty;

// create the array 
int arr[BUFFER_SIZE];
// global count 
int count = 0;
void* producer(void* arg){
    int index = *(int*) arg;
    free(arg);
    while(1){
        int random = rand()%100;
        pthread_mutex_lock(&mutex);
        while(count == BUFFER_SIZE){
            printf("Producer: Buffer full. Sleeping...\n");
            pthread_cond_wait(&cond_not_full,&mutex);
        }
        arr[count++] = random;
        printf("Producer(%d): Added %d (Count: %d)\n",index,random, count);

        pthread_cond_signal(&cond_not_empty);
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }
    return NULL;
}
void* consumer(void* arg){
    int index = *(int*) arg;
    free(arg);
    while(1){
        pthread_mutex_lock(&mutex);
        while(count == 0){
            printf("Consumer : Buffer empty. Sleeping...\n");
            pthread_cond_wait(&cond_not_empty,&mutex);
        }
        int item = arr[--count] ;
        printf("Consumer (%d): Removed %d (Count: %d)\n",index,item, count);
        pthread_cond_signal(&cond_not_full);
        pthread_mutex_unlock(&mutex);
        
        sleep(2);
    }
    return NULL;
}

int main(){
    pthread_t producer_threads[5],consumer_threads[5];
    srand(time(NULL));
    
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond_not_empty,NULL);
    pthread_cond_init(&cond_not_full,NULL);



    //pthread_create(&producer_thread,NULL,producer,NULL);
    //pthread_create(&consumer_thread,NULL,consumer,NULL);
    for (int i = 0; i < 5; i++)
    {
        int* a = malloc(sizeof(int));
        int* a1 = malloc(sizeof(int));

        *a= i; *a1 = i;
        if(pthread_create(&consumer_threads[i],NULL,consumer,a) != 0){
            perror("Consumer threads failed to create\n");
        }
        if(pthread_create(&producer_threads[i],NULL,producer,a1) != 0){
            perror("Producer threads failed to create\n");
        }
    }
    

  // pthread_join(producer_thread,NULL);
  // pthread_join(consumer_thread,NULL);
  for (int i = 0; i < 5; i++)
  {
    if (pthread_join(consumer_threads[i],NULL) != 0)
    {
       perror("Failed to join consumer threads\n");
    }
    if (pthread_join(producer_threads[i],NULL) != 0)
    {
       perror("Failed to join producer threads\n");
    }
    
  }
  
   
   pthread_mutex_destroy(&mutex);
   pthread_cond_destroy(&cond_not_empty);
   pthread_cond_destroy(&cond_not_full);
    return 0;
}