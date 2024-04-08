#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
//#include "ws2812.pio.h"
#include "hardware/watchdog.h"

const char* LEVEL_ONE = "-----";
const char* LEVEL_TWO = ".----";
const char* LEVEL_THREE = "..---";
const char* LEVEL_FOUR = "...--";

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

void reset_watchdog() {
    watchdog_update();
}



struct player
{
    int lives;
    int currentLevel;
    int completeLevels;
    int currentWins;
    bool gameComplete;
};
void playerReset(struct player * player){
    player->lives = 3;
    player->currentLevel = 0;
    player->completeLevels = 0;
    player->currentWins = 0;
    player->gameComplete = false;
}

struct player newPlayer() {
    struct player * newPlayer = malloc(sizeof(struct player));
    player->lives = malloc(sizeof(int));
    player->currentLevel = malloc(sizeof(int));
    player->completeLevels = malloc(sizeof(int));
    player->currentWins = malloc(sizeof(int));
    player->gameComplete = malloc(sizeof(bool));


}

struct letter{
    char letter;
    char *morse_Code;
};

struct letter newLetter(char letter, char *morse_Code){
    struct letter new;
    new.letter = letter;
    new.morse_Code = morse_Code;
    return new;
};

struct letter letterArr[26];

void new_Letter_Array(){
    // A
    letterArr[0] = newLetter('A', ".-");
    // B
    letterArr[1] = newLetter('B', "-...");
    // C
    letterArr[2] = newLetter('C', "-.-.");
    // D
    letterArr[3] = newLetter('D', "-..");
    // E
    letterArr[4] = newLetter('E', ".");
    // F
    letterArr[5] = newLetter('F', "..-.");
    // G
    letterArr[6] = newLetter('G', "--.");
    // H
    letterArr[7] = newLetter('H', "....");
    // I
    letterArr[8] = newLetter('I', "..");
    // J
    letterArr[9] = newLetter('J', ".---");
    // K
    letterArr[10] = newLetter('K', "-.-");
    // L
    letterArr[11] = newLetter('L', ".-..");
    // M
    letterArr[12] = newLetter('M', "--");
    // N
    letterArr[13] = newLetter('N', "-.");
    // O
    letterArr[14] = newLetter('O', "---");
    // P
    letterArr[15] = newLetter('P', ".--.");
    // Q
    letterArr[16] = newLetter('Q', "--.-");
    // R
    letterArr[17] = newLetter('R', ".-.");
    // S
    letterArr[18] = newLetter('S', "...");
    // T
    letterArr[19] = newLetter('T', "-");
    // U
    letterArr[20] = newLetter('U', "..-");
    // V
    letterArr[21] = newLetter('V', "...-");
    // W
    letterArr[22] = newLetter('W', ".--");
    // X
    letterArr[23] = newLetter('X', "-..-");
    // Y
    letterArr[24] = newLetter('Y', "-.--");
    // Z
    letterArr[25] = newLetter('Z', "--..");
};

struct letter letterGetter (char letter) {
    int letterIndex = (int) letter - 65;
    return letterArr[letterIndex];
}

void displayInfo(struct player p, struct letter l) {
    printf("Level -  %d\n", p.currentLevel);
    printf("Lives: %d\n", p.lives);
    printf("Wins: %d\n" , p.currentWins);
    printf("\n LETTER[%c] - MORSE CODE[%s]\n" , l.letter, l.morse_Code);
}

void displayWelcome() {
    printf("            WELCOME TO GROUP 18's MORSE CODE TEACHER\\n");
    printf("+--------------------------------------------------------------+\\n");
    printf("You will be shown a sequence of characters and\\n");
    printf("it's your job to input the correct corresponding morse code!\\n");
    printf("Dots are inputed when you press the button for under a second.\\n");
    printf("Spaces are inputed when you press the button for over a second.\\n");
    printf("                 USE GP21 TO SELECT LEVEL\\n");
    printf("                  \\\"-----\\\"  - LEVEL 01\\n");
    printf("                  \\\".----\\\"  - LEVEL 02\\n");
    printf("                  \\\"..---\\\"  - LEVEL 03\\n");
    printf("                  \\\"...--\\\"  - LEVEL 04\\n");
}


char* getMorse() {
    const int DOT_DURATION = 500;
    const int DASH_DURATION = 1500;
    const int SPACE_DURATION = 1000;
    const int COMPLETE_DURATION = 2000;
    static char morse_sequence[100];  // Initialize an empty string to hold the Morse code sequence
    int button_press_duration;
    clock_t start_t, end_t;

    start_t = clock();
    while (1)
    {
        button_press_duration = 0;  // Get the duration of the button press
        int space = 0;

        if (button_press_duration <= DOT_DURATION && button_press_duration >= 0) {
            if(space == 1){
                strcat(morse_sequence, " ");
                space = 0;
            }
            strcat(morse_sequence, ".");
            start_t = clock(); //reset the timer
        } else if (button_press_duration <= DASH_DURATION) {
            if(space == 1){
                strcat(morse_sequence, " ");
                space = 0;
            }
            strcat(morse_sequence, "-");
            start_t = clock();//reset the timer
        } else {
            end_t = clock();
            if ((end_t - start_t) >= SPACE_DURATION) {
                space = 1;
            }
            if ((end_t - start_t) >= COMPLETE_DURATION) {
                printf("Morse sequence complete: %s\n", morse_sequence);
                return morse_sequence;
            }  
        }         
    }
}

char characterFromMorse(char* userInput) {
    int i;
    for(i = 0; i < 26; i++) {
        if(strcmp(userInput, letterArr[i].morse_Code) == 0) {
            return letterArr[i].letter;
        }
    }
    // Return space character if no match found
    return ' ';
}


void playLevel(int levelNo) {
    struct player currentPlayer = newPlayer();
    char currentChar = (rand() % 26) + 'A';
    struct letter currentLetter = letterGetter(currentChar);
    if(levelNo <= 2) {
        printf("Letter: %c\nMorse Code: %s\n", currentLetter.letter, (levelNo==1)?currentLetter.morse_Code:"HIDDEN");
        char* userInput = getMorse();
        printf("You enterred: %c\n", characterFromMorse(userInput));
        if(strcmp(userInput, currentLetter.morse_Code) == 0) {
            if(currentPlayer.lives < 3)
                currentPlayer.lives++;
            currentPlayer.currentWins++;
            if(currentPlayer.currentWins >= 5) {
                //player wins.
            }
        } else {
            currentPlayer.lives--;
            //update RGB
            if(currentPlayer.lives <= 0) {
                //player loses
            }
        }
            
    } else { 
        printf(""); //levels 3 and four
    }
}

bool selectLevel() {
    struct player currentPlayer = newPlayer();
    char* levelInput = getMorse();
    if(strcmp(levelInput, LEVEL_ONE) == 0)
        playLevel(1);
    else if(strcmp(levelInput, LEVEL_TWO) == 0)
        playLevel(2);
    else if(strcmp(levelInput, LEVEL_THREE) == 0)
        playLevel(3);
    else if(strcmp(levelInput, LEVEL_FOUR) == 0)
        playLevel(4);
    else
        return false;
    return true;

}





void rgbLights(struct player p){
    if(p.lives==3)
    {
        put_pixel(urgb_u32(0x00, 0xFF, 0x00)); //green
    }
    else if(p.lives==2)
    {
        put_pixel(urgb_u32(0xFF, 0xFF, 0x00)); //yellow
    }
    else if(p.lives==1)
    {
        put_pixel(urgb_u32(0xFF, 0x80, 0x00)); //orange
    }
    else if(p.lives==0)
    {
        put_pixel(urgb_u32(0xFF, 0x00, 0x00));//red
    }
}

void rgbOff()
{
    put_pixel(urgb_u32(0x00, 0x00, 0x00));
}

/*
 * Main entry point for the code - simply calls the main assembly function.
 */
int main() {
    timer_hw->dbgpause = 0; //this is just here for the timer to work properly when debugging with openocd. It can be removed when not in debug mode
    stdio_init_all();              // Initialise all basic IO
    watchdog_reboot(0, 0, 0x7fffff);
    watchdog_enable(0x7fffff, false);
    watchdog_start_tick(12);
    main_asm();

    displayWelcome();
    while(!selectLevel());


    return(0);
}
