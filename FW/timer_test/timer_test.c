#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

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
#define EN_G_PWM (1 << PWM1D)
#define EN_B_PWM (1 << PWM1B)

// PWM duty cycle registers
// footprint for the RBG LED has Green and Blue swapped, correcting in FW.
#define R_DUTY_CYCLE OCR1A
#define G_DUTY_CYCLE OCR1D
#define B_DUTY_CYCLE OCR1B

#define NUM_RGB_LEDs 4
#define NUM_COLORS 3
#define PWM_MAX 255

uint8_t en_rgb_led[NUM_RGB_LEDs] = {EN_RGB1, EN_RGB2, EN_RGB3, EN_RGB4};
volatile uint8_t *rgb_duty_cycle[NUM_COLORS] = {&R_DUTY_CYCLE, &G_DUTY_CYCLE, &B_DUTY_CYCLE};

// set up timer0 for timed events
void init_timer0()
{
    TCNT0L = 0;

    // set timer counter to compare to OCR0A
    TCCR0A |= (1 << CTC0);

    // set clock source to prescaler CLK/64, Timer0 Freq = 8Mhz/64 = 125Khz
    TCCR0B |= (1 << CS01) | (1 << CS00);

    // count to 10, 125Khz/10 12.5Khz
    OCR0A = 10;

    // enable interupt OCR0A match
    TIMSK |= (1 << OCIE1A);
}


// set up timer1 for PWM
void init_timer1()
{
    // PWM Freq = (8Mhz/TOP) / prescale
    // Set presacle to CLK/16 -> PWM Freq 1.961Khz
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Cleared on compare match, enable Fast PWM for Red
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | EN_R_PWM | EN_B_PWM;

    // Cleared on compare match, enable Fast PWM for Green
    TCCR1C |= (1 << COM1D1) | EN_G_PWM;
}

// set up GPIO pin directions and logic levels
void init_pins()
{
    // enable output
    DDRA |= EN_RGB4 | EN_RGB3 | EN_RGB2 | EN_RGB1;
    DDRB |= B_PWM | G_PWM | R_PWM;

    // diable all LEDs
    PORTA &= ~EN_RGB4 & ~EN_RGB3 & ~EN_RGB2 & ~EN_RGB1;

    // RBG LEDs are common Anode, logic one on cathod turns them off
    PORTB |= R_PWM | G_PWM | B_PWM;

    // start with all LEDs off
    R_DUTY_CYCLE = PWM_MAX;
    G_DUTY_CYCLE = PWM_MAX;
    B_DUTY_CYCLE = PWM_MAX;
}

ISR(TIMER0_COMPA_vect)
{
    PORTA ^= EN_RGB1;
}

void setup()
{
    init_timer0();
}

int main()
{
#if 0
    uint8_t led;
    uint8_t color;
    uint8_t i;
#endif

    setup();
    init_pins();
    sei();
    while(1) {}

#if 0
    while (1) {
        if (TIFR & (1 << OCF0A)) {
            PORTA ^= EN_RGB1;
            TIFR |= (1 << OCF0A);
        }
    };
#endif

#if 0
    for(;;) {
        // loop through all RGB LEDs pulsing red, green, blue
        for (led = 0; led < NUM_RGB_LEDs; led++) {
            for (color = 0; color < NUM_COLORS; color++) {
                for (i = 0; i < PWM_MAX; i++) {
                    PORTA |= en_rgb_led[led];
                    *rgb_duty_cycle[color] = PWM_MAX - i;
                    _delay_ms(8);
                    // turn off the current colore before switching to the next
                    *rgb_duty_cycle[color] = PWM_MAX; 
                    PORTA &= ~(en_rgb_led[led]);
                }
            }
        }
    }
#endif
    return 0;
}
