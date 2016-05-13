#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdbool.h>

#include "HD44780.h"

void setup() {
    LCD_Initalize();
}


int main(void) {
    setup();
    LCD_WriteText("Hello world");
    while (1);
}
