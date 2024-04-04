#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "hardware/watchdog.h"

// Must declare the main assembly entry point before use.
void main_asm();

int gpio_get_next_input();

// Initialise a GPIO pin – see SDK for detail on gpio_init()
void asm_gpio_init(uint pin) {
    gpio_init(pin);
}

// Set direction of a GPIO pin – see SDK for detail on gpio_set_dir()
void asm_gpio_set_dir(uint pin, bool out) {
    gpio_set_dir(pin, out);
}

// Get the value of a GPIO pin – see SDK for detail on gpio_get()
bool asm_gpio_get(uint pin) {
    return gpio_get(pin);
}

// Set the value of a GPIO pin – see SDK for detail on gpio_put()
void asm_gpio_put(uint pin, bool value) {
    gpio_put(pin, value);
}

// Enable interrupts for both falling and rising edges
void asm_gpio_set_irq(uint pin) {
    gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(pin, GPIO_IRQ_EDGE_RISE, true);
}

/*
 * Main entry point for the code - simply calls the main assembly function.
 */
int main() {
    timer_hw->dbgpause = 0; //this is just here for the timer to work properly when debugging with openocd. It can be removed when not in debug mode
    stdio_init_all();              // Initialise all basic IO
    main_asm();

    printf("%d", gpio_get_next_input());

    while(true) {}

    return(0);
}

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

char letter_index_return(struct letter *ltr, int i){
    return ltr->morse_code[i];

}

void newCharAdd(int out){
    char dash = '-';
    char dot = '.';
    char space = ' ';
    char * temp = malloc ((strlen(input) +1)*sizeof(char));
    strcpy(temp,input);
    strncat(temp,((x == 0)? &dot (x == 1)?&dash:&space),1)
}
