#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

// create global variables 
int fuel = 0;
pthread_mutex_t mutex;
pthread_cond_t condFuel;


void* fuel_filling(void* arg){
   while(1)
    { 
    pthread_mutex_lock(&mutex);
    fuel += 10;
    printf("Filling Fuel:%d\n",fuel);
    pthread_cond_broadcast(&condFuel);
    pthread_mutex_unlock(&mutex);
    sleep(5);
    }
    return  NULL;
}

void* car(void* arg){
    
    while(1){
        pthread_mutex_lock(&mutex);
        while(fuel < 20){
            printf("Not enough fuel,Waiting for Fuel:%d\n",fuel);
            pthread_cond_wait(&condFuel,&mutex);
        }
        fuel -=15;
        printf("Fueled the car, remainig fuel :%d\n",fuel);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    return NULL;
}


int main(){
    pthread_t t[4];
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&condFuel,NULL);
    for (int i = 0; i < 4; i++)
    {
        if (i == 0)
        {
           if (pthread_create(&t[i],NULL,&fuel_filling,NULL) != 0) return 1;
        }else{
            if(pthread_create(&t[i],NULL,&car,NULL) != 0) return 2;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if(pthread_join(t[i],NULL) != 0) return 3;
    }
    
    


    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condFuel);
    return 0;
}
