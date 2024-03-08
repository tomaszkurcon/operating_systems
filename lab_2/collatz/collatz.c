#include "collatz.h"

int collatz_conjecture(int input) {
    return input % 2 == 0 ? input/2 : 3*input+1;
};
int test_collatz_conjecture(int input, int max_iter) {
    int counter = 0;
    while(counter<=max_iter) {
        if(input == 1) {
            return counter;
        } else {
            input = collatz_conjecture(input);
        }
        counter+=1;
    }
    return -1;
};