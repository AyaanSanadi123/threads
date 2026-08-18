#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
pthread_mutex_t mutex;
int mails = 0;
void* routine(void* arg){
    for (int i = 0; i < 10000000; i++)
    {
        pthread_mutex_lock(&mutex);
        mails++;
        pthread_mutex_unlock(&mutex);
    }
    
}

int main(){
    pthread_t th[4];
    pthread_mutex_init(&mutex, NULL);
    for (int i = 0; i < 4; i++)
    {
        // create the thread 
        if(pthread_create(th + i,NULL,&routine,NULL) != 0){
            // the thread failed 
            perror("Failed to create thread\n");
            return 1;
        }
        if(pthread_join(th[i],NULL) != 0){
            return 2;
        }
    }
    
    pthread_mutex_destroy(&mutex);
    printf("Number of mails : %d\n",mails);
    return 0;
}