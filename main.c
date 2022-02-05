#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    fgets(line, sizeof(line) + 1, word_list);
    fclose(word_list);

    strcpy((*word), line);
}

int valid_word (char * guess){
    printf("%s%ld\n", guess, strlen(guess));

    size_t len = strlen(guess);
    if (len != 6) {
        len > 6 ? printf("Word too long.\n") : printf("Word too short.\n");
        return 0;
    }

    return 1;
}

void user_input (char * word){
    char * guess;
    size_t len;
    int count = 0;

    printf("Your guess:\n");
    while(getline(&guess, &len, stdin) > -1 && count < 6) {
        if (!valid_word(guess)){continue;}

        response(word, guess);

        count ++;
    }
}

int main (void){
    char * word;
    printf("\033[1;1H\033[2J");  //clear terminal window

    printf("list len: %d\n", list_length());

    srand(time(0));
    choose_word(&word);

    printf("Chosen word: %s", word);
    user_input(word);
    return 0;
}