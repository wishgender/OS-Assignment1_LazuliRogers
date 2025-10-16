# OS-Assignment1_LazuliRogers
Assignment 1 for Operating Systems class at Kent State University

## Build Instructions

### GCC
```
gcc producer.cpp -pthread -lrt -o producer
gcc consumer.cpp -pthread -lrt -o consumer
```

## Execution Instructions

### Linux
```
./producer & ./consumer &
```