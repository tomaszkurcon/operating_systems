#include <stdio.h>
#include <stdlib.h>
#ifdef DYNAMIC
#include <dlfcn.h>
#else
#include "collatz/collatz.h"
#endif

int main(int argc, char *argv[]) {
#ifdef DYNAMIC
    void *handle = dlopen("libcollatz_shared.so", RTLD_LAZY);
    if(!handle){
        printf("%s", "error1");
        return 1;
    }
    int (*test_collatz_conjecture)(int input, int max_iter);
    test_collatz_conjecture = dlsym(handle,"test_collatz_conjecture");
    if(dlerror() != NULL){
        printf("%s", "error2");
        return 1;
    }
#endif
    for (int i = 2; i < argc; i++) {
        int arg = atoi(argv[i]);
        if(arg > 0) {
            printf("Arg: %d, Result:  %d\n", arg, test_collatz_conjecture(arg, atoi(argv[1])));
        }
    }
#ifdef DYNAMIC
    dlclose(handle);
#endif
    return 0;
}
