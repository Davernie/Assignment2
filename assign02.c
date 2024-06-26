#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/watchdog.h"
#include "ws2812.pio.h"

#define IS_RGBW true                    // Will use RGBW format
#define NUM_PIXELS 1                    // There is 1 WS2812 device in the chain
#define WS2812_PIN 28                   // The GPIO pin that the WS2812 connected to

const char* LEVEL_ONE = "-----";
const char* LEVEL_TWO = ".----";
const char* LEVEL_THREE = "..---";
const char* LEVEL_FOUR = "...--";
const char* WORD_LIST[] = {"COME", "GETS", "GIVE", "GONE", "KEEP", "LETS", "MAKE", "PUTS"};
const int WORD_LIST_LEN = 8;

// Must declare the main assembly entry point before use.
void main_asm();

int gpio_get_next_input();

//pushes 32bit color to PIO block
static inline void put_pixel(uint32_t pixel_grb) 
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

//converts to a 32 bit unsigned rgb color
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) 
{
    return  ((uint32_t) (r) << 8)  |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}


/**
 * @brief Function to generate an unsigned 32-bit composit GRB
 *        value by combining the individual 8-bit paramaters for
 *        red, green and blue together in the right order.
 * 
 * @param r     The 8-bit intensity value for the red component
 * @param g     The 8-bit intensity value for the green component
 * @param b     The 8-bit intensity value for the blue component
 * @return uint32_t Returns the resulting composit 32-bit RGB value
 */

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
    int totalWins;
    int totalLoses;
    bool gameComplete;
};
void playerReset(struct player * player){
    player->lives = 3;
    player->currentLevel = 0;
    player->completeLevels = 0;
    player->currentWins = 0;
    player->totalWins = 0;
    player->totalLoses = 0;
    player->gameComplete = false;
}

struct player * newPlayer() {
    struct player * newPlayer = malloc(sizeof(struct player));

    return newPlayer;

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



struct letter letterArr[36];

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
    // 0-9
    letterArr[26]=newLetter('0', "-----");
    letterArr[27]=newLetter('1', ".----");
    letterArr[28]=newLetter('2', "..---");
    letterArr[29]=newLetter('3', "...--");
    letterArr[30]=newLetter('4', "....-");
    letterArr[31]=newLetter('5', ".....");
    letterArr[32]=newLetter('6', "-....");
    letterArr[33]=newLetter('7', "--...");
    letterArr[34]=newLetter('8', "---..");
    letterArr[35]=newLetter('9', "----.");
};

struct letter letterGetter (char letter) {
    int letterIndex = (int) letter;
    if(letterIndex < 65)
        return letterArr[letterIndex - 22];
    return letterArr[letterIndex - 65];
}

void displayInfo(struct player p, struct letter l) {
    printf("Level -  %d\n", p.currentLevel);
    printf("Lives: %d\n", p.lives);
    printf("Wins: %d\n" , p.currentWins);
    printf("\n LETTER[%c] - MORSE CODE[%s]\n" , l.letter, l.morse_Code);
}

void displayWelcome() {
    printf("\n             WELCOME TO GROUP 18's MORSE CODE TEACHER\n");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n              INSTRUCTIONS:\n ");
    printf("\n     ~You will be shown a sequence of characters and\n");
    printf("\n     ~It's your job to input the correct corresponding morse code!\n");
    printf("\n     ~Dots are inputed when you press the button for under a second.\n");
    printf("\n     ~Spaces are inputed when you press the button for over a second.\n");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n              USE GP21 TO SELECT A LEVEL\n");
    printf("\n          \"-----\"  - LEVEL 01\n");
    printf("\n          \".----\"  - LEVEL 02\n");
    printf("\n          \"..---\"  - LEVEL 03\n");
    printf("\n          \"...--\"  - LEVEL 04\n");
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\n                  GOOD LUCK!\n\n");
}

char* getMorseInput() {
    char * userInput = malloc(40*sizeof(char));
    userInput[0] = '\0';
    bool endOfString = false;
    char currentChar = '\0', prevChar = '\0';
    printf("\nPlease enter morse code:\n ");
    while(!endOfString) {
        prevChar = currentChar;
        currentChar = gpio_get_next_input();
        printf("%c", currentChar);
        if(prevChar == ' ' && currentChar == ' ') 
            endOfString = (strlen(userInput) > 0); // end string assuming at least one character enterred
        else {
            if(strlen(userInput) > 0 || currentChar != ' ')
                strncat(userInput, &currentChar, 1);
        }

    }
    userInput[strlen(userInput)-1]='\0'; //remove extra space from end
    return userInput;
}

void updateLights(int lives)
{
        switch(lives)
        {
            case 0:
                put_pixel(urgb_u32(0x7f, 0x00, 0x00));//RED 
                break;
            case 1:
                put_pixel(urgb_u32(0x7f, 0x32, 0x00));//ORANGE  
                break;
            case 2:
                put_pixel(urgb_u32(0xFF, 0xFF, 0x00));//YELLOW
                break;
            case 3:
                put_pixel(urgb_u32(0x00, 0x7f, 0x00));//GREEN 
                break;
            default:
                put_pixel(urgb_u32(0x00, 0x00, 0x7f));//Blue 
                break;
        }
}

void rgbOff()
{
    put_pixel(urgb_u32(0x00, 0x00, 0x00));
}

char characterFromMorse(char* userInput) {
    int i;
    for(i = 0; i < 36; i++) {
        if(strcmp(userInput, letterArr[i].morse_Code) == 0) {
            return letterArr[i].letter;
        }
    }
    // Return space character if no match found
    return '?';
}

//Generate Word, take a word from a list
char* generateWord(){

    int index = rand() % WORD_LIST_LEN;
    return WORD_LIST[index];
}

char* wordtoMorse(char* word){
    char* morse_word = malloc(sizeof(char)*100);
    morse_word[0] = '\0';
    
    for(int i = 0; i < 4; i++){
        char * letterMorse = letterGetter(word[i]).morse_Code;
        strcat(morse_word, letterMorse);
        if(i < 3)
            strcat(morse_word," ");
    }
    return morse_word;
} 

bool playLevel(int levelNo, struct player currentPlayer) {
    srand((int)time(NULL));
    if(levelNo<=4 && levelNo!=1)
    {
        printf("\nCONGRATS, MOVING TO NEXT LEVEL!\n");
    }
    printf("\nYOU ARE NOW PLAYING LEVEL %d\n", levelNo);
    while(true) {   // run this until we return
        printf("\nLives: %d\n", currentPlayer.lives);
        updateLights(currentPlayer.lives);
        if(levelNo <= 2) {
            char currentChar = (rand() % 36);
            if((int)currentChar < 26)
                currentChar = currentChar + 'A';
            else 
                currentChar = (currentChar-26) + '0';
            struct letter currentLetter = letterGetter(currentChar);
            printf("\nLetter: %c\n\nMorse Code: %s\n", currentChar, (levelNo==1)?currentLetter.morse_Code:"HIDDEN");
            char* userInput = getMorseInput();
            printf("\nYou entered: %c\n", characterFromMorse(userInput));
            if(strcmp(userInput, currentLetter.morse_Code) == 0) {
                currentPlayer.currentWins++;
                currentPlayer.totalWins++;
                printf("CORRECT, you entered %d in a row\n", currentPlayer.currentWins);
                if(currentPlayer.lives < 3) {
                    currentPlayer.lives++;
                    updateLights(currentPlayer.lives);
                }
                if(currentPlayer.currentWins >= 5) {
                    if(levelNo >= 4) {
                        printf("You Win!\n");
                        printStats(currentPlayer);
                        return true;
                    } else {
                        currentPlayer.totalWins=0;
                        return playLevel(levelNo+1, currentPlayer);
                    }
                }
            } else {
                currentPlayer.lives--;
                currentPlayer.currentWins = 0;
                currentPlayer.totalLoses++;
                updateLights(currentPlayer.lives);
                if(currentPlayer.lives <= 0) {
                    printf("\nGAME OVER!\n");
                    printStats(currentPlayer);
                    return false;
                }
            }
                
        } else { 
            char * currentWord = generateWord();
            char * morseCodeWord = wordtoMorse(currentWord);
            printf("Word: %s\nMorse Code: %s\n", currentWord, (levelNo==3)?morseCodeWord:"HIDDEN");
            char* userInput = getMorseInput();
            printf("You entered: %s\n", characterFromMorse(userInput));
            if(strcmp(userInput, morseCodeWord) == 0) {
                currentPlayer.currentWins++;
                currentPlayer.totalWins++;
                printf("CORRECT, %d in a row\n", currentPlayer.currentWins);
                if(currentPlayer.lives < 3) {
                    currentPlayer.lives++;
                    updateLights(currentPlayer.lives);
                }
                if(currentPlayer.currentWins >= 5) {
                    if(levelNo >= 4) {
                        printf("You Win!\n");
                        printStats(currentPlayer);
                        return true;
                    } else {
                        currentPlayer.totalWins=0;
                        return playLevel(levelNo+1, currentPlayer);
                    }
                }
            } else {
                currentPlayer.lives--;
                currentPlayer.currentWins = 0;
                currentPlayer.totalLoses++;
                updateLights(currentPlayer.lives);
                if(currentPlayer.lives <= 0) {
                    printf("\nGAME OVER!\n");
                    printStats(currentPlayer);
                    return false;
                }
            }
        }
    }
    return true;
}

bool selectLevel() {
    struct player currentPlayer = *newPlayer();
    playerReset(&currentPlayer);
    char* levelInput = getMorseInput();
    if(strcmp(levelInput, LEVEL_ONE) == 0)
        playLevel(1, currentPlayer);
    else if(strcmp(levelInput, LEVEL_TWO) == 0)
        playLevel(2, currentPlayer);
    else if(strcmp(levelInput, LEVEL_THREE) == 0)
        playLevel(3, currentPlayer);
    else if(strcmp(levelInput, LEVEL_FOUR) == 0)
        playLevel(4, currentPlayer);
    else {
        printf("Invalid level code %s, please try again\n", levelInput);
        return false;
    }
    return true;

}


void testMorseInput() {
    while(true) {
        char currentChar = gpio_get_next_input();
        printf("%c", currentChar);
    }
}

void printStats(struct player p){
    printf("Total Correct Answers: %d\n", p.totalWins);
    printf("Total Incorrect Answers: %d\n", p.totalLoses);
    printf("Accuracy: %.03f\n", ((float)p.totalWins) / ((float)(p.totalWins + p.totalLoses)));

}

/*
 * Main entry point for the code - simply calls the main assembly function.
 */
int main() {
    timer_hw->dbgpause = 0; //this is just here for the timer to work properly when debugging with openocd. It can be removed when not in debug mode
    stdio_init_all();              // Initialise all basic IO
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, 0, offset, WS2812_PIN, 800000, IS_RGBW);
    watchdog_reboot(0, 0, 0x7fffff);
    watchdog_enable(0x7fffff, false);
    watchdog_start_tick(12);
    main_asm();

    new_Letter_Array();

    updateLights(5);
    displayWelcome();
    while(!selectLevel());


    return(0);
}
