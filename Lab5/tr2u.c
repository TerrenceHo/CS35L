#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Not enough operads\n");
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

    char buffer[1];
    ssize_t read_val = read(STDIN_FILENO, buffer, 1);
    if (read_val < 0) {
        fprintf(stderr, "Error occured while reading in byte\n");
        exit(1);
    }
    while(read_val > 0) {
        for(size_t i = 0; i < from_len; i++) {
            if (buffer[0] == from[i]) {
                buffer[0] = to[i];
                break;
            }
        }
        ssize_t write_val = write(STDOUT_FILENO, buffer, 1);
        if (write_val < 0) {
            fprintf(stderr, "Error occured while writing char to screen\n");
            exit(1);
        }
        read_val = read(STDIN_FILENO, buffer, 1);
        if (read_val < 0) {
            fprintf(stderr, "Error occured while reading in byte\n");
            exit(1);
        }
    }
    return 0;
}
