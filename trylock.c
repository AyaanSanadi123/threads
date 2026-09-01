#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<unistd.h>
pthread_mutex_t mutexs[4];
int fuel[100] = {100,100,100,100};
void* routine(void * arg){
    for (int i = 0; i < 4; i++)
    {
        // try and get lock on any availabe mutex 
        if(pthread_mutex_trylock(&mutexs[i]) == 0){
            // got the lock
            int fuelNeeded = (rand()%30);
            if(fuel[i] - fuelNeeded < 0){
                printf("NOT ENOUGH FUEL...\n");
            }else{
                fuel[i] -= fuelNeeded;
                printf("Fuel left : %d\n",fuel[i]);
                //sleep(3); is this a bad practise, to sleep with the lock on?
            }
            pthread_mutex_unlock(&mutexs[i]);
            sleep(3); // is ths a better place to sleep?? 
        }else{
           if(i == 3) {// did not get the lock
            printf("No stoves availabe... waiting\n");
            i = -1;
            printf("The value of i : %d\n",i);
            sleep(3);}

        }
    }
    
}

int main(){
    srand(time(NULL));
    pthread_t t[10];
    for (int i = 0; i < 4; i++)
    {
        pthread_mutex_init(&mutexs[i],NULL);
        
    }
    for (int i = 0; i < 10; i++)
    { 
        if(pthread_create(&t[i],NULL,&routine,NULL) != 0) return 1;
    }
    
    
    for (int i = 0; i < 10; i++)
    {
       if( pthread_join(t[i],NULL) != 0) return 2;
    }
    
    for (int i = 0; i < 4; i++)
    {
        pthread_mutex_destroy(&mutexs[i]);
    }
    
    return 0;
}