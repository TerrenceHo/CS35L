#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    void *lib = dlopen("libmymath.so", RTLD_NOW);
    if (lib == NULL) {
        fprintf(stderr, "Unable to open library file");
        exit(1);
    }

    int (*func_ptr)(int) = dlsym(lib, "factorial");
    if (func_ptr == NULL) {
        fprintf(stderr, "Unable to find function.");
        exit(1);
    }

    int n = 5;
    int result = (*func_ptr)(n);
    printf("Result = %d\n", result);
    dlclose(lib);
    return 0;
}
