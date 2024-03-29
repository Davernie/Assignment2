#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

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
