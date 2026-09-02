#include <stdlib.h>
#include <stdio.h>
#include<stdbool.h>
#include "threadpool.h"


static void* worker_loop(void* arg);


ThreadPool* thread_pool_init(int num_threads,int queue_capacity){
    if(num_threads <= 0 || queue_capacity <= 0) {
        printf("Please enter a valid number of threads or queue size\n");
        return NULL;
    }

    ThreadPool* pool = (ThreadPool*) malloc(sizeof(ThreadPool));

    if (pool == NULL)
    {
        printf("Could not allocate Pool\n");
        return NULL;
    }

    pool -> num_threads = num_threads;
    pool -> queue_capacity = queue_capacity;
    pool -> head = 0;
    pool -> tail = 0;
    pool -> count = 0;
    pool -> shutdown = false;


    pool -> threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
    pool -> task_queue = (Task*) malloc(queue_capacity * sizeof(Task));


    if (pool -> threads == NULL || pool -> task_queue == NULL)
    {
       printf("Cound not allocate memory for threads or task queue\n");
       free(pool->threads);
       free(pool -> task_queue);
       free(pool);

       return NULL;
    }

    if ( pthread_mutex_init(&(pool->lock),NULL) != 0 ||
         pthread_cond_init(&(pool->notify),NULL) != 0
        )
    {
       printf("Cound not allocate memory to mutex or condition variables\n");
       free(pool->threads);
       free(pool -> task_queue);
       free(pool);
       return NULL;
    }
    
    for (int i = 0; i < num_threads; i++)
    {
        if(pthread_create(&(pool->threads[i]),NULL,worker_loop,(void*)pool) != 0){

            // if a thread fails to create, we need to make sure all the created threads are 
            // deleted carefully and then destroy the pool

            pthread_mutex_lock(&(pool->lock));
            pool -> shutdown = true;
            pthread_cond_broadcast(&(pool -> notify)); // wakeup all the threads 
            pthread_mutex_unlock(&(pool -> lock));

            // wait for all the created threads to finish their execuation
            for (int j = 0; j < i; j++) {
                pthread_join(pool->threads[j], NULL);
            }

            // now destroy the pool 
            pthread_mutex_destroy(&(pool->lock));
            pthread_cond_destroy(&(pool->notify));
            free(pool->threads);
            free(pool->task_queue);
            free(pool);
            
            return NULL;
        }
    }
    
    return pool;
}


static void* worker_loop(void* arg){
    // caste the argument into a pool obj 
    ThreadPool * pool = (ThreadPool*)arg;

    while(1){
        pthread_mutex_lock(&(pool->lock));
        // if the queue is empty and does not need to shutdown
        while(pool -> count == 0 && !pool->shutdown){
            pthread_cond_wait(&(pool->notify),&(pool->lock));
        }

        // if shutdown is true and the queue is empty, break out of the while(1)
        if(pool -> shutdown && pool -> count == 0){
            pthread_mutex_unlock(&(pool -> lock));
            break;
        }

        // pop the task from the ring buffer 
        Task task = pool -> task_queue[pool -> head];

        // advance the head 
        pool -> head = (pool -> head + 1) % pool -> queue_capacity;
        pool -> count --;
        // unlock, so other threads can pick up tasks as the current one is being executed 
        pthread_mutex_unlock(&(pool -> lock));

        // execute the task 
        if(task.execute != NULL) task.execute(task.arg);

    }
    return NULL;
}

bool thread_pool_submit(ThreadPool* pool, void (*execute)(void*), void* arg){
    if (pool == NULL || execute == NULL) return false;

    pthread_mutex_lock(&(pool -> lock));

    // check for overflow,
    // if we have an overflow reject the task
    // the rest can be managed in the main loop 
     if (pool -> count == pool -> queue_capacity){
        pthread_mutex_unlock(&(pool -> lock));
        return false;
     }
     
     // do not accept new tasks if shutdown has been initiated 
     if (pool -> shutdown)
     {
        pthread_mutex_unlock(&(pool -> lock));
        return false;
     }
     
}