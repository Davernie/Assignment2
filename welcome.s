.syntax unified                                                 @ Specify unified assembly syntax
.cpu    cortex-m0plus                                           @ Specify CPU type is Cortex M0+
.thumb                                                          @ Specify thumb assembly for RP2040
.global main_asm                                                @ Provide program starting address to the linker
.align 4                                                   @ Specify code alignment


@ Entry point to the ASM portion of the program
main_asm:
    ldr     r0, =welcome_msg  
    bl      printf   
    ldr     r0, =line                                  
    bl      printf    
    ldr     r0, =instruct1  
    bl      printf   
    ldr     r0, =instruct2  
    bl      printf   
    ldr     r0, =instruct3  
    bl      printf   
    ldr     r0, =instruct4 
    bl      printf   
    ldr     r0, =line                                   
    bl      printf  
    ldr     r0, =level 
    bl      printf  
    ldr     r0, =level1
    bl      printf  
    ldr     r0, =level2 
    bl      printf  
    ldr     r0, =level3 
    bl      printf  
    ldr     r0, =level4 
    bl      printf  


@ Set data alignment
.data
    .align 4
    welcome_msg:    .asciz  "            WELCOME TO GROUP 18's MORSE CODE TEACHER\n"
    line:    .asciz         "+--------------------------------------------------------------+\n"
    instruct1:    .asciz    " You will be shown a sequence of characters and \n"
    instruct2:    .asciz    " it's your job to input the correct corresponding morse code!\n"
    instruct3:    .asciz    " Dots are inputed when you perss the button for under a second.\n"
    instruct4:    .asciz    " Spaces are inputed when you perss the button for over a second.\n"
    level: .asciz           "                 USE GP21 TO SELECT LEVEL\n"
    level1: .asciz          "                  \"-----\"  - LEVEL 01\n"
    level2: .asciz          "                  \".----\"  - LEVEL 02\n"
    level3: .asciz          "                  \"..---\"  - LEVEL 03\n"
    level4: .asciz          "                  \"...--\"  - LEVEL 04\n"
