#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <semaphore.h>

#define RSIZE 4
#define WSIZE 2

void *read(void *vargp);
void *write(void *vargp);

sem_t w_mutex;
sem_t r_mutex;


int shared_information=0;

int main() {    
    pthread_t readers[RSIZE], writers[WSIZE];

    sem_init(&r_mutex, 0, RSIZE);
    sem_init(&w_mutex, 0, 1);

    for (int i=0; i<WSIZE; i++)
        pthread_create(&writers[i], NULL, write, i);
    for (int i=0; i<RSIZE; i++)
        pthread_create(&readers[i], NULL, read, i);

    for (int i=0; i<WSIZE; i++)
        pthread_join(writers[i], NULL); 
    for (int i=0; i<RSIZE; i++)
        pthread_join(readers[i], NULL);    

    sem_destroy(&r_mutex);
    sem_destroy(&w_mutex);
}


void *read(void *vargp){
    sem_wait(&r_mutex);

    int id = vargp;
    printf("reader0%d reads %d\n", id, shared_information); 
    
    sem_post(&r_mutex);
} 

void *write(void *vargp){
    sem_wait(&w_mutex);

    int current_readers_size;
    sem_getvalue(&r_mutex, &current_readers_size);
    for (int i=0; i<current_readers_size; i++)
        sem_wait(&r_mutex); // setting r_mutex to 0       

    int id = vargp;
    shared_information += 1;
    printf("writer0%d writes %d\n", id, shared_information);

    sem_getvalue(&r_mutex, &current_readers_size);
    for (int i=0; i<RSIZE - current_readers_size; i++)
        sem_post(&r_mutex); //r_mutex back to normal    

    sem_post(&w_mutex);
}
