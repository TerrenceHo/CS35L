#include<stdlib.h>
#include<string.h>
#include<stdio.h>

/* Simple implementation of tr without regex possibilities. */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Not enough operands\n");
        exit(1);
    }
    char* from = argv[1];
    char* to = argv[2];

    int from_len = strlen(from);
    int to_len = strlen(to);

    if (from_len != to_len) {
        fprintf(stderr, "Operands from and to are not the same length\n");
        exit(1);
    }

    for(size_t i = 0; i < from_len; i++) {
        for(size_t j = i; j < from_len; j++){
            if ( from[i] == to[j] ) {
                fprintf(stderr, "Bytes in FROM cannot have duplicates\n");
                exit(1);
            }
        }
    }

    char curr = getchar();
    if(ferror(stdin)) {
        fprintf(stderr, "Error while reading in byte\n");
        exit(1);
    }
    while(!feof(stdin)) {
        for(size_t i = 0; i < from_len; i++) {
            if(curr == from[i]) {
                curr = to[i];
                break;
            }
        }
        if(putchar(curr) == EOF) {
            fprintf(stderr, "Error occurred while printing char to screen\n");
            exit(1);
        }
        curr = getchar();
        if(ferror(stdin)) {
            fprintf(stderr, "Error while reading in byte\n");
            exit(1);
        }
    }
    return 0;
}
