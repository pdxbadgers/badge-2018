#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define EN_RGB1 (1 << PA4) // 10
#define EN_RGB2 (1 << PA5) // 11
#define EN_RGB3 (1 << PA6) // 12
#define EN_RGB4 (1 << PA7) // 13
#define R_PWM   (1 << PB1) // 8
#define G_PWM   (1 << PB3) // 6
#define B_PWM   (1 << PB5) // 4

// PWM1A RED
// PWM1B Green
// PWM1D Blue

#define NUM_RGB_LEDs 4
#define PWM_MAX 255
#define NUM_COLORS 3

uint8_t en_rgb_led[NUM_RGB_LEDs] = {EN_RGB1, EN_RGB2, EN_RGB3, EN_RGB4};

void setup()
{
    // enable output
    DDRA = EN_RGB4 | EN_RGB3 | EN_RGB2 | EN_RGB1;
    DDRB = B_PWM | G_PWM | R_PWM;

    // turn off LED to start
    PORTB = B_PWM | G_PWM | R_PWM;

    // Cleared on compare match, enable PWM on OC1A
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << PWM1A); 

    // Set presacle to CLK/4 -> PWM Freq 3.926Khhz
    TCCR1B = (1 << CS11) | (1 << CS10);
}

int main()
{
    uint8_t led;
    //uint8_t color;
    uint8_t i;

    setup();

    for(;;) {
        // loop through all RGB LEDs pulsing red
        for (led = 0; led < NUM_RGB_LEDs; led++) {
            PORTA |= en_rgb_led[led];
            //for (color = 0; i < NUM_COLORS; color++) {
                for (i = 0; i < PWM_MAX; i++) {
                    OCR1A = PWM_MAX - i;
                    _delay_ms(4);
                }
                PORTA &= ~(en_rgb_led[led]);
            //}
        }
    }

    return 0;
}
