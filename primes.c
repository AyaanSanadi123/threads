// #include<stdio.h>
// #include<stdlib.h>
// #include<unistd.h>
// #include<pthread.h>

// int primes[10] = {2,3,5,7,11,13,17,19,23,29};
// void* routine(void * arg){
//     //sleep(1);
//     arg = *(int *)arg;
//     printf("%d,",arg);
//     free(arg);
// }

// int main(){
//     pthread_t t[10];

    
//     for (int i = 0; i < 10; i++)
//     {
//         int *a = malloc(sizeof(int)); 
//         *a = i;
//         if(pthread_create(t+i,NULL,&routine,a) !=0) return 1;
        
//     }
//     for (int i = 0; i < 10; i++)
//     {
//         if (pthread_join(t[i],NULL))
//         {
//             return 2;
//         }
        
//     }
    

//     return 0;
// }


// code - 1
// #include<stdio.h>
// #include<stdlib.h>
// #include<pthread.h>
// #include<time.h>
// #include<unistd.h>

// int primes[10] = {2,3,5,7,11,13,17,19,23,29};

// void* routine(void* arg){
//     // first update
//     sleep(1);
//     int index = *(int*)arg;

//     printf("%d,",primes[index]);
    
// }

// int main(){
//     pthread_t t[10];

//     for (int i = 0; i < 10; i++)
//     {
//       if ( pthread_create(t+i,NULL,&routine,&i) != 0) perror("Failed to create thread\n");
//     }
//     for (int i = 0; i < 10; i++)
//     {
//         if(pthread_join(t[i],NULL) != 0) return 1;
//     }
    
    
//     return 0;
// }

// code-2

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<time.h>
#include<unistd.h>

int primes[10] = {2,3,5,7,11,13,17,19,23,29};

void* routine(void* arg){
    int index = *(int*)arg;

    printf("%d,",primes[index]);
    free(arg);
}

int main(){
    pthread_t t[10];

    for (int i = 0; i < 10; i++)
    {
        int *a = malloc(sizeof(int));
        *a = i;
      if ( pthread_create(t+i,NULL,&routine,a) != 0) perror("Failed to create thread\n");
    }
    for (int i = 0; i < 10; i++)
    {
        if(pthread_join(t[i],NULL) != 0) return 1;
    }
    
    
    return 0;
}