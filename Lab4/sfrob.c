#include<stdio.h>
#include<stdlib.h>

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

// Input expects sorted array of char*, and how large words is.  Prints it out
// to standard output
void print_sorted_frob(char** words, int words_index) {
    for(size_t i = 0; i < words_index; i++) {
        size_t j = 0;
        while (1) {
            if(putchar(words[i][j]) == EOF) {
                for(size_t k = 0; k < words_index; k++) {
                    free(words[k]);
                }
                free(words);
                fprintf(stderr, "Error occured when writing character");
                exit(1);
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
    // Read in first two characters, while checking for errors.
    char c_char = getchar();
    if(ferror(stdin)) {
        fprintf(stderr, "Error occured while reading file");
        exit(1);
    }

    char next_char = getchar();
    if(ferror(stdin)) {
        fprintf(stderr, "Error occured while reading file");
        exit(1);
    }

    // word takes in each char until it hits a space.
    // Then store each word in words
    // words_index/word_index keep track of array location
    int word_index = 0;
    int words_index = 0;
    char* word = (char*)malloc(sizeof(char));
    char** words = (char**)malloc(sizeof(char*));

    // Begin big while loop to read in chars and place into arrays.
    while(c_char != EOF && !ferror(stdin)) {
        // Put char into word, and reallocate memory for next char.
        // If error, free everything, print out error, exit
        word[word_index] = c_char;
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
            fprintf(stderr, "Error occured while allocating memory");
            exit(1);
        }

        // When we have allocated a space, then that is the end of that line.
        // Place word into words, and reallocate words for next word. If error,
        // free everything, and print out error
        if(c_char == ' ') {
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
                fprintf(stderr, "Error occured while allocating memory");
                exit(1);
            }
        }

        // Edge case condition checking
        if(next_char == EOF && c_char == ' ') {
            // If end of file, end.  May not be necessary, but for explicit
            break;
        } else if (c_char == ' ' && next_char == ' ') {
            // If there are extra spaces, iterate through them until it is the
            // next char isn't a space.
            while(c_char == ' ') {
                c_char = getchar();
                if(ferror(stdin)) {
                    for(size_t i = 0; i < words_index; i++){
                        free(words[i]);
                    }
                    free(words);
                    fprintf(stderr, "Error occured while reading file");
                    exit(1);
                }
            }
            next_char = getchar();
            if(ferror(stdin)) {
                for(size_t i = 0; i < words_index; i++) {
                    free(words[i]);
                }
                free(words);
                fprintf(stderr, "Error occured while reading file");
                exit(1);
            }
            continue;
        } else if(next_char == EOF) {
            // At end of file, but last char was not a space, so we add one
            // space to the end.
            c_char = ' ';
            continue;
        }

        // Prep next loop
        c_char = next_char;
        next_char = getchar();
        if(ferror(stdin)) {
            free(word);
            for(size_t i = 0; i < words_index; i++) {
                free(words[i]);
            }
            free(words);
            fprintf(stderr, "Error occured while reading file");
            exit(1);
        }
    }

    free(word); // free last word
    // Return everything
    struct words_wrapper ww;
    ww.words = words;
    ww.words_index= words_index;
    return ww;
}

int main(void) {
    // retrieve input from stdin, retrieve data from struct
    struct words_wrapper ww = read_input();
    char** words = ww.words;
    int words_index = ww.words_index;

    // sort frobnicated words
    qsort(words, words_index, sizeof(char*), frobcmp_wrapper);

    // print words through putchar
    print_sorted_frob(words, words_index);

    // Deallocate memory
    for(size_t i = 0; i < words_index; i++) {
        free(words[i]);
    }
    free(words);

    return 0;
}
