//= laz r
//= 10-16-2025 13:16
//= Operating Systems Fall 2025
//= producer.cpp

//= Dependencies =//
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <shared_mutex>

#define TABLE_SIZE 2 //= table can only hold two items at once

//= Seeds for random number generation
const int DEV_SEED = 0;
const time_t PROD_SEED = time_t(nullptr);

//= define table
int table[TABLE_SIZE]; 

//= pre-define helper functions

int genItem();

int main() {
    std::srand(DEV_SEED); //= Init randomization with testing seed
    // std::srand(PROD_SEED); //= init randomization with production seed
    while(true) {
        //= generate an item
        int item = genItem();
    }

    return;
}

//= REQUIRES: N/A
//= ENSURES: returns a random integer between 0 and 999 (inclusive)
int genItem() {
    return (rand() % 1000);
}