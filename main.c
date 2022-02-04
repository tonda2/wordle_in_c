#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILENAME "word_list.txt"

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

    FILE * file = fopen(FILENAME, "r");
    fseek(file, (random - 1) * 6, SEEK_SET);
    char line[6];
    fgets(line, sizeof(line) + 1, file);
    fclose(file);

    strcpy((*word), line);
}

int main (void){
    char * word;

    printf("list len: %d\n", list_length());

    srand(time(0));
    choose_word(&word);

    printf("Choosen word: %s", word);

    return 0;
}