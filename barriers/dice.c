#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<pthread.h>

#define THREAD_NUM 8

int diceValues[8];
int status[8] = {0};

void* rollDice(void* args){
    int index = *(int*)args;
    diceValues[index] = rand() % 6 + 1;

    
    free(args);
    
}

int main(){
    pthread_t th[THREAD_NUM];
    srand(time(NULL));
    for (int i = 0; i < THREAD_NUM; i++)
    {
        int* a = malloc(sizeof(int));
        *a = i;
       if(pthread_create(&th[i],NULL,&rollDice,a) != 0) return 1;
    }

    for (int i = 0; i < THREAD_NUM; i++)
    {
        if(pthread_join(th[i],NULL) != 0) return 2;
    }
    int max = 0;
    for (int i = 0; i < THREAD_NUM; i++)
    {
       if(diceValues[i] > max) max = diceValues[i];
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if (diceValues[i] == max)
        {
            status[i] = 1;
        }else{
            status[i] = 0;
        }
        
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        if(status[i] == 1 ){
        printf("%d rolled %d and i won\n",i,diceValues[i]);
    }else{
         printf("%d rolled %d and i lost\n",i,diceValues[i]);
    }
    }
    
    
    
    
    return 0;
}