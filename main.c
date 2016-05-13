#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdbool.h>

int main(void) {
    DDRA = 0xff;

    while (1) {
        PORTA++;
        _delay_ms(500);    
    }
}
