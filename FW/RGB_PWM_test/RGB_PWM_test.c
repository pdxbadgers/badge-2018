#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// pins to enable the RGB LEDs
#define EN_RGB1 (1 << PA4) // 10
#define EN_RGB2 (1 << PA5) // 11
#define EN_RGB3 (1 << PA6) // 12
#define EN_RGB4 (1 << PA7) // 13

// PWM output pins
// footprint for the RBG LED has Green and Blue swapped, correcting in FW.
#define R_PWM   (1 << PB1) // 8
#define G_PWM   (1 << PB5) // 4
#define B_PWM   (1 << PB3) // 6

// register bit to enable PWM
#define EN_R_PWM (1 << PWM1A)
#define EN_G_PWM (1 << PWM1B)
#define EN_B_PWM (1 << PWM1D)

// PWM duty cycle registers
// footprint for the RBG LED has Green and Blue swapped, correcting in FW.
#define R_DUTY_CYCLE OCR1A
#define G_DUTY_CYCLE OCR1D
#define B_DUTY_CYCLE OCR1B

#define NUM_RGB_LEDs 4
#define NUM_COLORS 3
#define PWM_MAX 255

uint8_t en_rgb_led[NUM_RGB_LEDs] = {EN_RGB1, EN_RGB2, EN_RGB3, EN_RGB4};
volatile uint8_t *rbg_duty_cycle[NUM_COLORS] = {&R_DUTY_CYCLE, &G_DUTY_CYCLE, &B_DUTY_CYCLE};

void setup()
{
    // enable output
    DDRA = EN_RGB4 | EN_RGB3 | EN_RGB2 | EN_RGB1;
    DDRB = B_PWM | G_PWM | R_PWM;

    // turn off LED to start
    PORTB = B_PWM | G_PWM;

    // Cleared on compare match, enable Red PWM
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | EN_R_PWM;

    // Set presacle to CLK/4 -> PWM Freq 3.926Khhz
    TCCR1B = (1 << CS11) | (1 << CS10);

    //TCCR1C = ( 1 << COM1D1) | EN_B_PWM;

    // "turn off" Green and Blue LEDs
    //OCR1A = PWM_MAX;
    //OCR1B = PWM_MAX;
    //OCR1D = PWM_MAX;
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
                    R_DUTY_CYCLE = PWM_MAX - i;
                    _delay_ms(32);
                }
                PORTA &= ~(en_rgb_led[led]);
            //}
        }
    }

    return 0;
}
