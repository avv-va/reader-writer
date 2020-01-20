#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#define readers_size 4
#define writers_size 2
  

int shared_information=0; // we assume that writers write numbers

void *read(void *vargp){ 
    int id = vargp;
    printf("reader0%d reads %d\n", id, shared_information); 
} 

void *write(void *vargp){
    int id = vargp;
    shared_information += 1;
    printf("writer0%d writes %d\n", id, shared_information);
}

int main() {    
    pthread_t readers[readers_size], writers[writers_size];

    for (int i=0; i<writers_size; i++)
        pthread_create(&writers[i], NULL, write, i);
    for (int i=0; i<readers_size; i++)
        pthread_create(&readers[i], NULL, read, i);

    
    for (int i=0; i<writers_size; i++)
        pthread_join(writers[i], NULL); 
    for (int i=0; i<readers_size; i++)
        pthread_join(readers[i], NULL); 
    exit(0); 
}
