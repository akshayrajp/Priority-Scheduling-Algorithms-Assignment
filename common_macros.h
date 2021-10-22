#ifndef COMMON_MACROS
#define COMMON_MACROS

#define QUANTUM 3
#define AGING_PERIOD 1
#define FRONT 0
#define INPUT_FILE "input.csv"

#define MAX(x, y) (((x) >= (y)) ? (x) : (y))
#define MIN(x, y) (((x) <= (y)) ? (x) : (y))

#define TYPE_QUANTUM "quantum"
#define TYPE_AGING "aging"

#define OPERATION_ENQUEUE "enqueue"
#define OPERATION_DEQUEUE "dequeue"

#endif