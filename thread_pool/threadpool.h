#ifndef THREADPOOL_H
#define THREADPOOL_H

#include<stdlib.h>
#include<pthread.h>
#include<stdbool.h>

typedef struct {
    void(*execute)(void* arg); // pointer to the function 
    void* arg; // pointer to the argument of that function
} Task;

typedef struct {
    pthread_t* threads;
    int num_threads;

    Task* task_queue;
    int queue_capacity;
    int head;
    int tail;
    int count;

    pthread_mutex_t lock;
    pthread_cond_t notify;
    bool shutdown;
} ThreadPool;

#endif