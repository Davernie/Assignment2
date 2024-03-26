.syntax unified                                                 @ Specify unified assembly syntax
.cpu    cortex-m0plus                                           @ Specify CPU type is Cortex M0+
.thumb                                                          @ Specify thumb assembly for RP2040
.global main_asm                                                @ Provide program starting address to the linker
.align 4                                                        @ Specify code alignment


@ Entry point to the ASM portion of the program
main_asm:
	


install_gpio_interrupt:
	PUSH {LR}


	POP  {PC}

initialise_buttons:
	PUSH {LR}


	POP  {PC}


gpio_get_next_input:
	PUSH {LR}


	POP  {PC}



gpio_press_handler:
	


@ Set data alignment
.data
	.align 4
