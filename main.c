#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdbool.h>

#include "HD44780.h"

void setup() {
    // LCD
    LCD_Initalize();

    // PWM
    DDRB |= (1 << PB3);
    TCCR0 |= (1 << WGM00) | (1 << WGM01); // fast PWM mode
    TCCR0 |= (1 << COM01); // override PB3 output, non-inverting mode
    TCCR0 |= (1 << CS00); // no prescaler, frequency F_CPU/256
    OCR0 = 0; // turned off

    // ADC
    ADCSRA |= (1 << ADEN); // turn on ADC
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128
    ADMUX |= (1 << REFS0) | (1 << REFS1); // Vref = 2.56 V
    // PA0 as input
    DDRA &= ~(1 << PA0);
    PORTA &= ~(1 << PA0);
    ADMUX &= ~(1 + 2 + 4 + 8 + 16); 
}

uint16_t adc() {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}


int main(void) {
    setup();

    char display[16 + 1];
    while (1) {
        sprintf(display, "adc = %d", adc());
        LCD_Clear();
        LCD_WriteText(display);
        _delay_ms(1000);
    }
}
