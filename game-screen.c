#include <stdio.h>

void displayInfo(struct player p, struct letter l) {
    printf("Level -  %d\n", p.currentLevel);
    printf("Lives: %d\n", p.lives);
    printf("Wins: %d\n" , p.currentWins);
    printf("\n LETTER[%c] - MORSE CODE[%s]\n" , l.letter, l.morse_Code);
}

