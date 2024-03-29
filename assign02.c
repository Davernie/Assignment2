#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "hardware/watchdog.h"
struct player
{
    int lives;
    int currentLevel;
    int completeLevels;
    int currentWins;
    bool gameComplete;
}
void playerReset(struct player * player){
    player->lives = 3;
    player->currentLevel = 0;
    player->completeLevels = 0;
    player->currentWins = 0;
    player->gameComplete = false;
}

struct letter{
    char letter;
    char *morse_Code;
}

struct letter newLetter(char letter, char *morse_Code){
    struct letter new;
    new.letter = letter;
    new.morse_Code = morse_Code;
    return new;
}

struct letter letterArr[26];

void new_Letter_Array;{
    // A
    letterArr[0] = newLetter('A', '.-');
    // B
    letterArr[1] = newLetter('B', '-...');
    // C
    letterArr[2] = newLetter('C', '-.-.');
    // D
    letterArr[3] = newLetter('D', '-..');
    // E
    letterArr[4] = newLetter('E', '.');
    // F
    letterArr[5] = newLetter('F', '..-.');
    // G
    letterArr[6] = newLetter('G', '--.');
    // H
    letterArr[7] = newLetter('H', '....');
    // I
    letterArr[8] = newLetter('I', '..');
    // J
    letterArr[9] = newLetter('J', '.---');
    // K
    letterArr[10] = newLetter('K', '-.-');
    // L
    letterArr[11] = newLetter('L', '.-..');
    // M
    letterArr[12] = newLetter('M', '--');
    // N
    letterArr[13] = newLetter('N', '-.');
    // O
    letterArr[14] = newLetter('O', '---');
    // P
    letterArr[15] = newLetter('P', '.--.');
    // Q
    letterArr[16] = newLetter('Q', '--.-');
    // R
    letterArr[17] = newLetter('R', '.-.');
    // S
    letterArr[18] = newLetter('S', '...');
    // T
    letterArr[19] = newLetter('T', '-');
    // U
    letterArr[20] = newLetter('U', '..-');
    // V
    letterArr[21] = newLetter('V', '...-');
    // W
    letterArr[22] = newLetter('W', '.--');
    // X
    letterArr[23] = newLetter('X', '-..-');
    // Y
    letterArr[24] = newLetter('Y', '-.--');
    // Z
    letterArr[25] = newLetter('Z', '--..');
}

struct letterGetter (char letter){
    int letterIndex = (int) letter - 65;
    return letterArr[letterIndex];
}


    
int main() {
    rand
    return 0;
}
