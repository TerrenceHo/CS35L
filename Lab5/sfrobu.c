#include<stdlib.h>
#include<ctype.h>
#include<stdio.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>


void print_error(const char* msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void io_error(ssize_t io_val) {
    if (io_val < 0) {
        print_error("Error occured while reading or writing data");
    }
}

/* Iterate until one is greater or less, or both have a space.  If both are
 * equal return 0. If a is greater, return 1, if a is less return -1
 */
int frobcmp(char const* a, char const* b) {
    for(;; a++, b++) {
        if(*a == ' ' && *b == ' ') { 
            return 0; 
        } else if (*b == ' ' || ((*a ^ 42) > (*b ^ 42))) { 
            return 1; 
        } else if (*a == ' ' || ((*a ^ 42) < (*b ^ 42))) { 
            return -1; 
        }
    }
    return 0;
}

// Wrapper for fromcmp, use with qsort
int frobcmp_wrapper(const void* a, const void* b) {
    return frobcmp(*(const char**)a, *(const char**)b);
}

int frobcmp_upper(char const* a, char const* b) {
    for(;; a++, b++) {
        if(*a == ' ' && *b == ' ') { 
            return 0; 
        } else if (*b == ' '){
            return 1;
        } else if (*a > 0 && *b > 0 && (toupper(*a ^ 42) > toupper(*b ^ 42))) { 
            return 1; 
        } else if (*a == ' '){
            return -1;
        } else if (*a > 0 && *b > 0 && (toupper(*a ^ 42) < toupper(*b ^ 42))) { 
            return -1; 
        }
    }
    return 0;
}

int frobcmp_upper_wrapper(const void* a, const void* b) {
    return frobcmp_upper(*(const char**)a, *(const char**)b);
}

// Input expects sorted array of char*, and how large words is.  Prints it out
// to standard output
void print_sorted_frob(char** words, int words_index) {
    for(size_t i = 0; i < words_index; i++) {
        long j = 0;
        while (1) {
            if(write(1, (void*)&words[i][j], 1) <= 0) {
                for(size_t k = 0; k < words_index; k++) {
                    free(words[k]);
                }
                free(words);
                print_error("Error occured when writing character");
            }
            if (words[i][j] == ' ') {
                break;
            }
            j++;
        }
    }
}


// Wrapper struct to return both char** words and words_index
struct words_wrapper {
    char** words;
    int words_index;
};

// Returns words and words_index.  Reads input from stdin, stores each line in a
// char* and stores each char* in a char**.  Reallocates memory each time to
// account for expanding documents, and unknown size requirements.
struct words_wrapper read_input() {
    char* file_array;
    int word_index = 0;
    int words_index = 0;
    char* word;
    char** words = malloc(sizeof(char*));
    char c_char[1];
    char next_char[1];

    // Get file information from fstat about stdin
    struct stat file_stat;
    ssize_t fstat_val = fstat(0, &file_stat);
    if(fstat_val) {
        print_error("Error occured while parsing fstat");
    }

    if(S_ISREG(file_stat.st_mode)) {
        file_array = (char*)malloc((file_stat.st_size+1)* sizeof(char));
        ssize_t read_val = read(0, file_array, file_stat.st_size);
        file_array[file_stat.st_size] = ' '; // set last char to a space
        int start_flag = 0;
        int space_search = 0;
        size_t file_array_len = file_stat.st_size + 1;
        for(size_t i = 0; i < file_array_len; i++) {
            if(space_search == 0 && file_array[i] != ' ') {
                start_flag = i;
                space_search = 1;
            }
            if(file_array[i] == ' ') {
                word = malloc( (i - start_flag + 1) * sizeof(char));
                memcpy(word, file_array + start_flag, i - start_flag + 1);
                words[words_index] = word;
                words_index++;
                char** temp_words = realloc(words, (words_index+1)*sizeof(char*));
                if(temp_words != NULL) {
                    words = temp_words;
                } else {
                    print_error("Error occured while allocating memory");
                }
                while(i < file_array_len && file_array[i+1] == ' ') {
                    i++;
                }
                space_search = 0;
            }
        }
        free(file_array);
    }


    word = malloc(sizeof(char));

    // If file is growing, read in more characters one by one
    // Read in first two characters, while checking for errors.
    ssize_t c_char_val = read(0, c_char, 1);
    io_error(c_char_val);

    ssize_t next_char_val = read(0, next_char, 1);
    io_error(next_char_val);

    // Begin big while loop to read in chars and place into arrays.
    while(c_char_val > 0) {
        // Put char into word, and reallocate memory for next char.
        // If error, free everything, print out error, exit
        word[word_index] = c_char[0];
        word_index++;
        char* temp_word = realloc(word, (word_index+1)*sizeof(char));
        if(temp_word != NULL) {
            word = temp_word;
        } else {
            free(word);
            for(size_t i = 0; i < words_index; i++) {
                free(words[i]);
            }
            free(words);
            print_error("Error occured while allocating memory");
        }

        // When we have allocated a space, then that is the end of that line.
        // Place word into words, and reallocate words for next word. If error,
        // free everything, and print out error
        if(c_char[0] == ' ') {
            words[words_index] = word;
            words_index++;
            char** temp_words = realloc(words, (words_index+1)*sizeof(char*));
            if(temp_words != NULL) {
                words = temp_words;

                word = NULL;
                word = (char*)malloc(sizeof(char));
                word_index = 0;
            } else {
                for(size_t i = 0; i < words_index; i++) {
                    free(words[i]);
                }
                free(words);
                print_error("Error occured while allocating memory");
            }
        }

        // Edge case condition checking
        if(next_char_val == 0 && c_char[0] == ' ') {
            // If end of file, end.  May not be necessary, but for explicit
            break;
        } else if (c_char[0] == ' ' && next_char[0] == ' ') {
            // If there are extra spaces, iterate through them until it is the
            // next char isn't a space.
            while(c_char[0] == ' ') {
                c_char_val = read(0, c_char, 1);
                if(c_char_val < 0) {
                    for(size_t i = 0; i < words_index; i++){
                        free(words[i]);
                    }
                    free(words);
                    print_error("Error occured while reading file");
                }
            }
            next_char_val = read(0, next_char, 1);
            if(next_char_val < 0) {
                for(size_t i = 0; i < words_index; i++) {
                    free(words[i]);
                }
                free(words);
                print_error("Error occured while reading file");
            }
            continue;
        } else if(next_char_val == 0) {
            // At end of file, but last char was not a space, so we add one
            // space to the end.
            c_char[0] = ' ';
            continue;
        }

        // Prep next loop
        c_char[0] = next_char[0];
        next_char_val = read(0, next_char, 1);
        if(next_char_val < 0) {
            free(word);
            for(size_t i = 0; i < words_index; i++) {
                free(words[i]);
            }
            free(words);
            print_error("Error occured while reading file");
        }
    }

    free(word); // free last word
    // Return everything
    struct words_wrapper ww;
    ww.words = words;
    ww.words_index= words_index;
    return ww;
}

int main(int argc, char* argv[]) {
    int is_upper = 0;
    if(argc == 2 && argv[1] == "-f") {
        is_upper = 1;
    }

    // retrieve input from stdin, retrieve data from struct
    struct words_wrapper ww = read_input();
    char** words = ww.words;
    int words_index = ww.words_index;

    if(is_upper == 1) {
        qsort(words, words_index, sizeof(char*), frobcmp_upper_wrapper);
    } else {
        // sort frobnicated words
        qsort(words, words_index, sizeof(char*), frobcmp_wrapper);
    }

    // print words through putchar
    print_sorted_frob(words, words_index);

    // Deallocate memory
    for(size_t i = 0; i < words_index; i++) {
        if(words[i]) {
            free(words[i]);
        }
    }
    free(words);

    return 0;
}
