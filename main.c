#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdbool.h>

#include "HD44780.h"
#include "ds18b20.h"

// In fast PWM mode output can't go down to 0
// max freq F_CPU/256
void _fast_pwm() {
    TCCR0 |= (1 << WGM00) | (1 << WGM01); // fast PWM mode
}

// In phase correct PWM output can be any value
// but frequency is half the above
void _phase_correct_pwm() {
    TCCR0 &= ~(1 << WGM01);
    TCCR0 |= (1 << WGM00);
}

// PWM using 8-bit Timer0
void setup_pwm() {
    DDRB |= (1 << PB3);
    _phase_correct_pwm();
    //_fast_pwm();  
    TCCR0 |= (1 << COM01); // override PB3 output, non-inverting mode
    TCCR0 |= (1 << CS00); // no prescaler
    OCR0 = 0; // turned off
}

void setup_lcd() {
    LCD_Initalize();
}

void setup_adc() {
    ADCSRA |= (1 << ADEN); // turn on ADC
    ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // prescaler 128
    ADMUX |= (1 << REFS0) | (1 << REFS1); // Vref = 2.56 V
    // PA0 as input
    DDRA &= ~(1 << PA0);
    PORTA &= ~(1 << PA0);
    ADMUX &= ~(1 + 2 + 4 + 8 + 16); 
}

// Interrupts using 16-bit Timer1
void setup_interrupt() {
    // no prescaler
    TCCR1B &= ~((1 << CS12) | (1 << CS11)); 
    TCCR1B |= (1 << CS10);
    TIMSK |= (1 << TOIE1); // enable overflow interrupt
    sei(); // enable global interrupts
}

// serial 8N1
void setup_serial() {
#define BAUD 9600
#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1) 
    UBRRH = BAUDRATE >> 8;
    UBRRL = BAUDRATE;
    UCSRB |= (1 << TXEN) | (1 << RXEN);
    UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
}

void setup() {
    setup_lcd();
    setup_pwm();
    setup_adc();
    setup_interrupt();
    setup_serial();
}

void uart_send_byte(unsigned char data) {
    while (!(UCSRA & (1 << UDRE)));
    UDR = data;
}

unsigned char uart_receive_byte(void) {
    while(!((UCSRA) & (1 << RXC)));
    return UDR;
}

void uart_send_string(unsigned char* data) {
    while (*data) {
        uart_send_byte(*data++);
    }
}


ISR(TIMER1_OVF_vect) {
    // runs every 2^16 CPU cycles
}

uint16_t adc() {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

float temp_read() {
	unsigned char ds18b20_pad[9];

	ds18b20_ConvertT();
	_delay_ms(750);
	ds18b20_Read(ds18b20_pad);

	return ((ds18b20_pad[1] << 8) + ds18b20_pad[0]) / 16.0;
}

int main(void) {
    setup();

    while(1) {

    }
}
