// Testing pins etc for RGB LED
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

#define NUM_RGB_LEDs 4
#define NUM_COLORS 3

uint8_t en_rgb_led[NUM_RGB_LEDs] = {EN_RGB1, EN_RGB2, EN_RGB3, EN_RGB4};
uint8_t colors[NUM_COLORS] = {R_PWM, G_PWM, B_PWM};

void setup()
{
    // enable output
    DDRA = EN_RGB4 | EN_RGB3 | EN_RGB2 | EN_RGB1;
    DDRB = B_PWM | G_PWM | R_PWM;

    // diable all LEDs
    PORTA &= ~EN_RGB4 & ~EN_RGB3 & ~EN_RGB2 & ~EN_RGB1;

    // enable all LEDS
    //PORTA |= EN_RGB4 | EN_RGB3 | EN_RGB2 | EN_RGB1;

    // RBG LEDs are common Anode, logic one on cathod turns them off
    PORTB |= R_PWM | G_PWM | B_PWM;

}

int main()
{
    uint8_t led;
    uint8_t color;

    setup();

    for (;;) {
        for (led = 0; led < NUM_RGB_LEDs; led++) {
            for (color = 0; color < NUM_COLORS; color++) {
                PORTA |= en_rgb_led[led];
                PORTB &= ~colors[color];
                _delay_ms(250);
                PORTB |= colors[color];
                PORTA &= ~(en_rgb_led[led]);
            }
        }
    }

    return 0;
}
