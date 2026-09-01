#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

// create global variables 
int fuel = 0;
pthread_mutex_t mutex;
pthread_cond_t condFuel;


void* fuel_filling(void* arg){
    pthread_mutex_lock(&mutex);
    fuel += 40;
     printf("Filling Fuel:%d\n",fuel);
    pthread_mutex_unlock(&mutex);
}

void* car(void* arg){
    pthread_mutex_lock(&mutex);
    while (fuel < 40)
    {
        sleep(1);
    }
   
    fuel-=15;
    printf("Fueling the car:%d\n",fuel);
    pthread_mutex_unlock(&mutex);
}


int main(){
    pthread_t t[2];
    pthread_mutex_init(&mutex,NULL);
    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
        {
           if (pthread_create(&t[i],NULL,&fuel_filling,NULL) != 0) return 1;
        }else{
            if(pthread_create(&t[i],NULL,&car,NULL) != 0) return 2;
        }
    }
    for (int i = 0; i < 2; i++)
    {
        if(pthread_join(t[i],NULL) != 0) return 3;
    }
    
    


    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condFuel);
    return 0;
}