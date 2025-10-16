//= laz r
//= 10-16-2025 18:15
//= sharedTable.hpp
#ifndef SHARED_TABLE_H
#define SHARED_TABLE_H

//= Dependencies =//
#include <semaphore.h>

#define TABLE_SIZE 2 //= table can only hold two items at once
#define SHM_NAME "/shmTable"

//= define table
typedef struct {
    int buffer[TABLE_SIZE];
    int in;
    int out;
} Table;

#define SEM_EMPTY "/sem_empty"
#define SEM_FULL "/sem_full"
#define SEM_MUTEX "/sem_mutex"

#endif