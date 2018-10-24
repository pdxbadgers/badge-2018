// Test the charlieplexing and LED mapping
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// pin definitions
#define ROW1 (1 << PA0) // 0
#define ROW2 (1 << PA1) // 1
#define ROW3 (1 << PA2) // 2
#define ROW4 (1 << PA3) // 14

#define ANODE 0
#define CATHODE 1
#define NUMB_LEDS 12

#define SLOW_SPEED 500
#define MED_SPEED 250
#define FAST_SPEED 75

volatile uint16_t SPEED = 1000;

// array of all the LEDs in {ANODE,CATHODE} pairing
uint8_t LED[NUMB_LEDS][2] = { {ROW3, ROW2}, {ROW2, ROW3}, {ROW4, ROW3},
                           {ROW3, ROW4}, {ROW4, ROW2}, {ROW2, ROW4},
                           {ROW4, ROW1}, {ROW1, ROW4}, {ROW3, ROW1},
                           {ROW1, ROW3}, {ROW2, ROW1}, {ROW1, ROW2},
};

// turn off all LEDs
void all_off_led()
{
    // high Z for all ROW pins
    DDRA &= ~ROW4 & ~ROW3 & ~ROW2 & ~ROW1;
    PORTA &= ~ROW4 & ~ROW3 & ~ROW2 & ~ROW1;
}


// turn on a given LED, param is a an {ANODE,CATHODE} pair
void on_led(uint8_t *LED)
{
    // Set pins to output
    DDRA |= LED[ANODE] | LED[CATHODE];

    digitalWrite(LED[ANODE], HIGH);
    digitalWrite(LED[CATHODE], LOW);
}

// turn off a given LED, param is a an {ANODE,CATHODE} pair
void off_led(uint8_t *LED)
{
    pinMode(LED[ANODE], INPUT);
    pinMode(LED[CATHODE], INPUT);
}

void blink_led(uint8_t *LED)
{
    on_led(LED);
    _delay_ms(SPEED);

    off_led(LED); 
}

void cycle_led()
{
    uint8_t i;

    for (i = 0; i < NUMB_LEDS; i++)
        blink_led(LED[i]);
}

int main()
{
    all_off_led();
    cycle_led();
}
