#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define FILENAME_CZ "cz_words.txt"
#define FILENAME_EN "en_words.txt"

void yellow () {
    printf("\033[1;33m");
}

void green () {
    printf("\033[0;32m");
}

void reset () {
    printf("\033[0m");
}

int list_length (int lang){
    FILE * file;
    lang == 1 ? file = fopen(FILENAME_CZ, "r") : file = fopen(FILENAME_EN, "r");
    if (!file) return 0;
    fseek(file, 0, SEEK_END);
    int len = (ftell(file)/6);
    fclose(file);
    return len;
}

void choose_word (char ** word, int lang){
    int max = list_length(lang);
    int random = (rand() % (max + 1));
    //printf("random number: %d\n", random);

    FILE * word_list;
    lang == 1 ? word_list = fopen(FILENAME_CZ, "r") : word_list = fopen(FILENAME_EN, "r");
    fseek(word_list, (random - 1) * 6, SEEK_SET);
    char line[6];
    fgets(line, sizeof(line), word_list);
    fclose(word_list);

    strcpy((*word), line);
    (*word)[5] = '\0';
}

int valid_word (char * guess, int count){
    size_t len = strlen(guess);

    if (len != 6) {
        len > 6 ? printf("Word too long.\n") : printf("Word too short.\n");
        printf("Your %d. guess: ", count + 1);
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
        green();
        printf("%s\n", word);
        reset();
        printf("Splendid. The word was %5s.\n", guess);
        return 1;
    }

    int * state = (int *) calloc (5, sizeof(int));
    check_letters(&state, word, guess);    

    for (int i = 0; i < 5; i ++) {
        switch (state[i]){
            case 1:
                yellow();
                break;
            case 2:
                green();
                break;
            case 0:
                reset();
        }

        printf("%c", toupper(guess[i]));
        reset();
    }
    printf("\n");

    free(state);
    return 0;
}

void user_input (char * word){
    char guess[100];
    int count = 0;

    printf("Your 1. guess: ");
    while(count < 6 && fgets(guess, sizeof(guess), stdin)) {
        if (!valid_word(guess, count)){continue;}
        guess[5] = '\0';

        if (response(word, guess) == 1){
            break;
        }

        count ++;
        if (count < 6) {
            printf("Your %d. guess: ", count + 1);
        }
    }

    if (count == 6) {
        printf("The word was %s. Better luck next time.\n", word);
    }
}

int keep_playing () {
    char check;
    int play = -1;

    printf("New game? (y/n)\n");
    while (play == -1) {
        scanf("%c", &check);
        switch (check) {
            case 'y':
                scanf("%c", &check);
                play = 1;
                break;
            case 'n':
                play = 0;
                break;
            case '\n':
                break;
            default:
                printf("Type 'y' for new game or 'n' to stop.\n");
                play = -1;
        }
    }

    return play;
}

int select_language (int argc, char ** argv){
    if (argc < 2){
        printf("Argument missing. Select cz for czech, en for englisth language.\n");
        return 0;
    }
    else if (argc > 2){
        printf("That's too many arguments. Select cz for czech, en for englisth language.\n");
        return 0;
    }

    if (!strcasecmp(argv[1], "cz")){
        return 1;
    }
    else if (!strcasecmp(argv[1], "en")){
        return 2;
    }

    printf("Invalid argument. Select cz for czech, en for englisth language.\n");
    return 0;
}

int main (int argc, char * argv[]){
    int lang = select_language(argc, argv);
    if (!lang) {
        return 0;
    }

    char * word = (char *) calloc (6, sizeof(char));
    int play = 1;
    printf("\033[1;1H\033[2J");  //clear terminal window
    printf("Welcome to Wordle!\n");

    while(play){
        srand(time(0));
        choose_word(&word, lang);
        user_input(word);

        if (keep_playing()){play = 1;}
        else {play = 0;}
        printf("\033[1;1H\033[2J");  //clear terminal window
    }

    free(word);
    return 0;
}