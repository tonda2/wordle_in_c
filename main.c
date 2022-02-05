#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define FILENAME "answers.txt"

int list_length (){
    FILE * file = fopen(FILENAME, "r");
    if (!file) return 0;
    fseek(file, 0, SEEK_END);
    int len = (ftell(file)/6);
    fclose(file);
    return len;
}

void choose_word (char * word[6]){
    int max = list_length();
    int random = (rand() % (max + 1));
    printf("random number: %d\n", random);

    FILE * word_list = fopen(FILENAME, "r");
    fseek(word_list, (random - 1) * 6, SEEK_SET);
    char line[6];
    fgets(line, sizeof(line), word_list);
    fclose(word_list);

    strcpy((*word), line);
    (*word)[5] = '\0';
}

int valid_word (char * guess){
    size_t len = strlen(guess);

    if (len != 6) {
        len > 6 ? printf("Word too long.\n") : printf("Word too short.\n");
        return 0;
    }

    for (int i = 0; i < 5; i ++) {
        if (!isalpha(guess[i])){
            printf("Invalid word.\n");
            return 0;
        }
    }

    return 1;
}

void check_letters (int ** state, char * word, char * guess) {
    int * taken = (int *) calloc (5, sizeof(int));

    for (int i = 0; i < 5; i ++) {
        if (tolower(guess[i]) == tolower(word[i])) {
            //printf("ano\n");
            taken[i] = 1;
            (*state)[i] = 2;
        }
        else {
            //printf("ne\n");
        }
    }

    for (int i = 0; i < 5; i ++) {
        for (int j = 0; j < 5; j ++) {
            if ((*state)[i] != 2 && taken[j] == 0 && tolower(guess[i]) == tolower(word[j])){
                (*state)[i] = 1;
                taken[j] = 1;
                break;
            }
        }
    }

    free(taken);
}

int response (char * word, char * guess){
    if (!strcasecmp(word, guess)){
        printf("Splendid. It was %5s.\n", guess);
        return 1;
    }

    int * state = (int *) calloc (5, sizeof(int));
    check_letters(&state, word, guess);    

    for (int i = 0; i < 5; i ++) {
        printf("%d\n", state[i]);
    }

    free(state);
    return 0;
}

void user_input (char * word){
    char guess[10];
    int count = 0;

    printf("Your guess:\n");
    while(count < 6 && fgets(guess, sizeof(guess), stdin)) {
        if (!valid_word(guess)){continue;}
        guess[5] = '\0';

        if (response(word, guess) == 1){
            break;
        }

        count ++;
    }

    if (count == 6) {
        printf("Better luck next time.\n");
    }
}

int main (void){
    char * word;
    printf("\033[1;1H\033[2J");  //clear terminal window

    printf("list len: %d\n", list_length());

    srand(time(0));
    choose_word(&word);

    printf("Chosen word: %s\n", word);
    user_input(word);
    return 0;
}