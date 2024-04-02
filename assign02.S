#include "hardware/regs/addressmap.h"
#include "hardware/regs/m0plus.h"
#include "hardware/regs/timer.h"
#include "hardware/regs/io_bank0.h"

.syntax unified                                                 @ Specify unified assembly syntax
.cpu    cortex-m0plus                                           @ Specify CPU type is Cortex M0+
.thumb                                                          @ Specify thumb assembly for RP2040
.global main_asm                                                @ Provide program starting address to the linker
.global gpio_get_next_input
.align 4                                                        @ Specify code alignment

.equ GPIO_INPUT_BUTTON, 21
.equ GPIO_DIR_IN, 0                             @ Input direction for GPIO pin
.equ GPIO_DIR_OUT, 1
.equ GPIO_LED_PIN, 25
.equ GPIO_BUTTON_VAL_ON, 0
.equ GPIO_BUTTON_VAL_OFF, 1  
.equ GPIO_ISR_OFFSET, 0x74                      @ GPIO is int #13 (vector table entry 29)
.equ BIT_13_MASK, 0x6000                        @ a bitmask with only bit 13 set (for GPIO interrupts)
.equ GPIO_INPUT_PRESS_MASK, 0x00400000          @Bit-22 for falling edge event on GP21
.equ GPIO_INPUT_RELEASE_MASK, 0x00800000        


@ Entry point to the ASM portion of the program
main_asm:
    PUSH {LR}
    BL   install_gpio_interrupt
    BL   initialise_buttons
    BL   initialise_led

    POP  {PC}

inf_loop:
    BAL  inf_loop

install_gpio_interrupt:
    PUSH {LR}                                   @ Push link register to stack
    LDR  R2, =(PPB_BASE + M0PLUS_VTOR_OFFSET)   @ Load R2 with Vector table offset
    LDR  R1, [R2]                               @ load from that address into R1
    MOVS R2, #GPIO_ISR_OFFSET                   @ load the GPIO isr offset into R2
    ADD  R2, R1                                 @ add the two together
    LDR  R0, =gpio_press_handler              @ Load R0 with the button press handler's address
    STR  R0, [R2]                               @ Store that pointer into the previously calculated memory address
    POP  {PC}                                   @ return

initialise_buttons:
    PUSH {LR}
    MOVS R0, #GPIO_INPUT_BUTTON                @ Move the value of the pin to be used to input morse code into R0 as parameter for asm_gpio_init
    BL   asm_gpio_init                          @ Initialise the GPIO pin
    MOVS R0, #GPIO_INPUT_BUTTON                @ This value is the GPIO pin for the button to use
    MOVS R1, #GPIO_DIR_IN                       @ This needs to be setup as an input pin
    BL   asm_gpio_set_dir                       @ Call the subroutine to set the GPIO pin specified by r0 to state specified by r1
    MOVS R0, #GPIO_INPUT_BUTTON                @ Input button pin into R0
    BL   asm_gpio_set_irq                       @ Set irq on the input button
    LDR  R1, =BIT_13_MASK                       @ Load R1 with a bitmask that has only bit 13 set
    LDR  R2, =(PPB_BASE + M0PLUS_NVIC_ICPR_OFFSET)    @ Load R2 with the ICPR address
    STR  R1, [R2]                               @ disable interrupt on gpio 
    LDR  R2, =(PPB_BASE + M0PLUS_NVIC_ISER_OFFSET)    @ Load R2 with ISER address
    STR  R1, [R2]                               @ enable interrupt on gpio
    POP  {PC}

initialise_led:
    PUSH {LR}
    MOVS R0, #GPIO_LED_PIN                      @ put LED pin number in R0
    BL   asm_gpio_init                          @ initialise the pin at that number
    MOVS R0, #GPIO_LED_PIN                      @ put LED pin number into R0 again
    MOVS R1, #GPIO_DIR_OUT                      @ load the value corresponding to GPIO out into R1
    BL   asm_gpio_set_dir                       @ set the direction for the LED to be treated as an output pin
    POP  {PC}


@ halts the execution until the user enters something
gpio_get_next_input:
    PUSH {R4-R5, LR}
.LWhileNoNewInput:
    LDR  R4, =input_status                      @ check whether there is a new input (value would be set by the interrupt)
    LDR  R4, [R4]
    CMP  R4, #1                                 @ 
    BNE  .LWhileNoNewInput                      @ if there is a new input:
    LDR  R4, =time_button_was_held
    LDR  R0, [R4]                               @ return value
    LDR  R4, =input_status
    MOVS R5, #0
    LDR  R5, [R4]

    POP  {R4-R5, PC}


    .type gpio_press_handler %function
gpio_press_handler:
    PUSH {R4-R6, LR}

    LDR  R4, =GPIO_INPUT_BUTTON
    BL   asm_gpio_get
    CMP  R0, GPIO_BUTTON_VAL_ON     @ on means falling edge
    BNE  .LElseIfReleased
    LDR  R4, =time_at_button_press
    LDR  R5, =(TIMER_BASE + TIMER_TIMELR_OFFSET)@ address of lower half of current time
    LDR  R5, [R5]                               @ R5 now holds the lower half of the current time
    STR  R5, [R4]
    LDR  R6, =GPIO_INPUT_PRESS_MASK             @ store this to clear interrupt later
    BAL  .LEndIfPushed
.LElseIfReleased:
    LDR  R4, =time_at_button_press
    LDR  R4, [R4]
    LDR  R5, =(TIMER_BASE + TIMER_TIMELR_OFFSET)@ address of lower half of current time
    LDR  R5, [R5]                               @ R5 now holds the lower half of the current time
    SUBS R5, R4                                 @ R4 now holds the time difference
    LDR  R4, =time_button_was_held
    STR  R5, [R4]
    LDR  R4, =input_status
    MOVS R5, #1
    STR  R5, [R4]
    LDR  R6, =GPIO_INPUT_RELEASE_MASK           @ store this to clear interrupt later
.LEndIfPushed:

    LDR  R4, =(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET) @ calculate address needed to be able to cancel the interrupt
    STR  R6, [R4]                               @ store the correct bitmask into that address

    POP  {R4-R6, PC}

@ Set data alignment
.data
    .align 4

current_button_state:
    .word GPIO_BUTTON_VAL_OFF
time_at_button_press:
    .word 0
time_button_was_held:
    .word 0
input_status:
    .word 0

test_string:
    .asciz "TEST"