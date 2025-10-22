//= laz r
//= 10-16-2025 13:16
//= Operating Systems Fall 2025
//= producer.c

//= Dependencies =//
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <time.h>
#include "sharedTable.h"

//= Seeds for random number generation
static Table *table;
static int shm_fd;
static sem_t *empty, *full, *mutex;
static volatile __sig_atomic_t running = 1;

//= pre-define helper functions

int genItem();

void handleQuit(int);

void cleanup();

int main() {
    signal(SIGINT, handleQuit);
    srand(time(NULL)); //= Init randomization with testing seed
    
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(Table));
    table = mmap(NULL, sizeof(Table), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    table->in = 0;
    table->out = 0;

    sem_t *empty = sem_open(SEM_EMPTY, O_CREAT, 0666, TABLE_SIZE);
    sem_t *full  = sem_open(SEM_FULL, O_CREAT, 0666, 0);
    sem_t *mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);

    printf("[producer]: Production started.\n");

    int iterations = 0;
    while(running && (iterations < ITERATIONS)) {
        //= generate an item
        int item = genItem();

        sem_wait(empty);
        sem_wait(mutex);

        table->buffer[table->in] = item;
        printf("[producer]: Produced %d at position %d\n", item, table->in);
        table->in = (table->in + 1) % TABLE_SIZE;

        sem_post(mutex);
        sem_post(full);
        
        iterations++;
        sleep(rand() % 2 + 1); //= simulate work time
    }

    cleanup();
    return 0;
}

//= ENSURES: returns a random integer between 0 and 999 (inclusive)
int genItem() {
    return (rand() % 1000);
}

//= Handler for CTRL+C
void handleQuit(int sig) {
    running = 0;
}

//= closes semaphores and unlinks shared resources
void cleanup() {
    printf("\n[producer]: cleaning up...\n");
    //= unmap and close shared table
    munmap(table, sizeof(Table));
    close(shm_fd);

    //= close semaphores
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);

    //= unlink semaphores
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_MUTEX);
    sem_unlink(SHM_NAME);

    printf("[producer]: exiting cleanly.\n");
}