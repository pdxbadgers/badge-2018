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


// hold all RGB enable pins
uint8_t en_rgb_led[NUM_RGB_LEDs] = {EN_RGB1, EN_RGB2, EN_RGB3, EN_RGB4};

// hold the address of the PWM duty cycle counter registers
volatile uint8_t *rgb_duty_cycle[NUM_COLORS] = {&R_DUTY_CYCLE, &G_DUTY_CYCLE, &B_DUTY_CYCLE};

// counter for implementing a software timer
volatile uint8_t tick = 0;

// counter for computing PWM duty cycle
volatile uint8_t duty_cycle = 0;

// counter for cycling through the RGB LEDs.
volatile uint8_t rgb_led = 0;

// counter for cylcing thourgh the RGB colors.
volatile uint8_t color = 0;

// set up timer0 for timed events
void timer0_init()
{
    // set timer counter to compare to OCR0A
    TCCR0A |= (1 << CTC0);

    // set clock source to prescaler CLK/256, Timer0 Freq = 8Mhz/256 = 31.25Khz
    TCCR0B |= (1 << CS02);

    // RGB LED Mux timer, 31.25Khz/3 10.416Khz
    OCR0A = 3; 

    // enable interupt OCR0A match
    TIMSK |= (1 << OCIE0A);
}


// set up timer1 for PWM
void timer1_init()
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
void pins_init()
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


// Multiplex the RGB LEDs
ISR(TIMER0_COMPA_vect)
{
    tick++;
    //PORTA ^= EN_RGB1;

    // Vary the PWM duty cycle every 8ms 
    if ((tick % 32) == 0) {
        // set PWM duty cycle for a given color
        *rgb_duty_cycle[color] = PWM_MAX - duty_cycle;
        duty_cycle++;

        if (PWM_MAX == duty_cycle) {
            // turn off the current colore before switching to the next
            *rgb_duty_cycle[color] = PWM_MAX; 
            color++;
            duty_cycle = 0;
        }

        if (NUM_COLORS == color)
            color = 0;
    }
}


void setup()
{
    timer0_init();
    timer1_init();
}

int main()
{

    setup();
    pins_init();
    sei();
    PORTA |= EN_RGB1;


    while(1);

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
