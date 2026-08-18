#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>

pthread_mutex_t mutex;

void* roll_dice(){
   
    int * result = malloc(sizeof(int));
    int random = (rand() % 6) + 1;
    *result = random;
    return (void *) result;
   
}

int main(){
    pthread_t t[8];
     srand(time(NULL));
    for (int i = 0; i < 8; i++)
    {
      if (pthread_create(t+i,NULL,&roll_dice,NULL) != 0) return 1;
    }

    for (int i = 0; i < 8; i++)
    {
        int *res;
        if (pthread_join(t[i],(void *)&res) != 0) return 2;
       
       printf("The value from thread %d is %d\n",i,*res); 
       free(res);
    }
    
    return 0;
}