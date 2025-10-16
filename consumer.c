//= Lazuli Rogers
//= 10-16-2025 13:05
//= Operating Systems Fall 2025
//= consumer.cpp

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

static Table *table;
static int shm_fd;
static sem_t *empty, *full, *mutex;
static volatile sig_atomic_t running = 1;

void handleQuit(int);

void cleanup();

int main() {
    signal(SIGINT, handleQuit);
    srand(time(NULL));

    //= open shared memory
    shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    table = mmap(NULL, sizeof(Table), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    //= open the semaphores
    empty = sem_open(SEM_EMPTY, 0);
    full = sem_open(SEM_FULL, 0);
    mutex = sem_open(SEM_MUTEX, 0);

    printf("[consumer]: Consumption started.\n");
    
    while(running) {
        sem_wait(full);
        sem_wait(mutex);

        int item = table->buffer[table->out];
        printf("[consumer]: Consumed %d from position %d\n", item, table->out);
        table->out = (table->out + 1) % TABLE_SIZE;

        sem_post(mutex);
        sem_post(empty);

        sleep(rand() % 3 + 1);
    }

    cleanup();
    return 0;
}

void handleQuit(int sig) {
    running = 0;
}

//= closes semaphores
void cleanup() {
    printf("\n[consumer]: cleaning up...\n");

    //= unmap shared table
    munmap(table, sizeof(Table));
    close(shm_fd);

    //= close semaphores
    sem_close(empty);
    sem_close(full);
    sem_close(mutex);

    printf("[consumer]: exiting cleanly.\n");
}