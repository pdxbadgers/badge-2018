#include <util/delay.h>

const int EN_RGB1 = (1 << PA4); //10; // PA4
const int EN_RGB2 = (1 << PA5); //11; // PA5
const int EN_RGB3 = (1 << PA6); //12; // PA6
const int EN_RGB4 = (1 << PA7); //13; // PA7
const int R_PWM = 8; // PB1
const int G_PWM = 6; // PB3
const int B_PWM = 4; // PB5

const byte NUM_RGB_LEDs = 4;
const byte PWM_MAX = 255;

byte en_rgb_led[NUM_RGB_LEDs] = {EN_RGB1, EN_RGB2, EN_RGB3, EN_RGB4};

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

void loop()
{
    byte led;
    byte color
    byte i;

    // loop through all RGB LEDs pulsing red
    for (led = 0; led < NUM_RGB_LEDs; led++)
    {
        PORTA |= en_rgb_led[led];
        for (i = 0; i < PWM_MAX; i++)
        {
            OCR1A = PWM_MAX - i;
            _delay_ms(4);
        }
        PORTA &= ~(en_rgb_led[led]);
    }
}
