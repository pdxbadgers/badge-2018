#include <util/delay.h>
#include <stdint.h>

const uint8_t EN_RGB1 = (1 << PA4); // 10
const uint8_t EN_RGB2 = (1 << PA5); // 11
const uint8_t EN_RGB3 = (1 << PA6); // 12
const uint8_t EN_RGB4 = (1 << PA7); // 13
const uint8_t R_PWM = (1 << PB1);   // 8
const uint8_t G_PWM = (1 << PB3);   // 6
const uint8_t B_PWM = (1 << PB5);   // 4

const uint8_t NUM_RGB_LEDs = 4;
const uint8_t PWM_MAX = 255;
const uint8_t NUM_COLORS = 3;

const uint8_t en_rgb_led[NUM_RGB_LEDs] = {EN_RGB1, EN_RGB2, EN_RGB3, EN_RGB4};


void setup()
{
    DDRA = EN_RGB4 | EN_RGB3 | EN_RGB2 | EN_RGB1;

/*
    pinMode(EN_RGB1, OUTPUT);
    pinMode(EN_RGB2, OUTPUT);
    pinMode(EN_RGB3, OUTPUT);
    pinMode(EN_RGB4, OUTPUT);
*/
    pinMode(R_PWM, OUTPUT);
    pinMode(G_PWM, OUTPUT);
    pinMode(B_PWM, OUTPUT);

    // Cleared on compare match, enable PWM on OC1A
    TCCR1A = (1 << COM1A1) | (1 << PWM1A); 

    // Set presacle to CLK/ -> PWM Freq 3.926Khhz
    TCCR1B = (1 << CS11) | (1 << CS10);

    // turn off LED to start
    //digitalWrite(R_PWM, HIGH);
    digitalWrite(G_PWM, HIGH);
    digitalWrite(B_PWM, HIGH);
}

int main()
{
    uint8_t led;
    uint8_t color;
    uint8_t i;

    setup();

    for(;;) {
        // loop through all RGB LEDs pulsing red
        for (led = 0; led < NUM_RGB_LEDs; led++) {
            PORTA |= en_rgb_led[led];
            for (color = 0; i < NUM_COLORS; color++) {
                for (i = 0; i < PWM_MAX; i++) {
                    OCR1A = PWM_MAX - i;
                    _delay_ms(4);
                }
                PORTA &= ~(en_rgb_led[led]);
            }
        }
    }
}
