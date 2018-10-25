// test all the LEDs, PWM, Timers, and Charlieplexing

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>

// yellow led pin definitions
#define ROW1 (1 << PA0)
#define ROW2 (1 << PA1)
#define ROW3 (1 << PA2)
#define ROW4 (1 << PA3)

// Yellow LED cycle speeds in ms
#define SLOW_SPEED 500
#define MED_SPEED 250
#define FAST_SPEED 75
#define SPEED SLOW_SPEED

// RGB LED pin definitions
#define EN_RGB1 (1 << PA4)
#define EN_RGB2 (1 << PA5)
#define EN_RGB3 (1 << PA6)
#define EN_RGB4 (1 << PA7)

// switch pin definitions
#define L_SWITCH (1 << PB4)
#define R_SWITCH (1 << PB6)

// PWM output pins
// FIXME: footprint for the RBG LED has Green and Blue swapped, correcting in FW.
#define R_PWM (1 << PB1)
#define G_PWM (1 << PB5)
#define B_PWM (1 << PB3)

// register bit to enable PWM
#define EN_R_PWM (1 << PWM1A)
#define EN_G_PWM (1 << PWM1D)
#define EN_B_PWM (1 << PWM1B)

// PWM duty cycle registers
// FIXME: footprint for the RBG LED has Green and Blue swapped, correcting in FW.
#define R_DUTY_CYCLE OCR1A
#define G_DUTY_CYCLE OCR1D
#define B_DUTY_CYCLE OCR1B

// misc constants
#define NUM_RGB_LEDs 4
#define NUM_YLW_LEDS 12
#define NUM_COLORS 3
#define PWM_MAX 255
#define SEED 0xDEAD

// index into yellow LED array
#define ANODE 0
#define CATHODE 1

// delay timings for yellow LEDs
#define SLOW_SPEED 500
#define MED_SPEED 250
#define FAST_SPEED 75


// Yellow LED states
enum {SLOW_CYCLE, MED_CYCLE, FAST_CYCLE, MED_RAND, FAST_RAND};

// array of all the yellow LEDs in {ANODE,CATHODE} pairing
uint8_t LED[NUM_YLW_LEDS][2] = { {ROW3, ROW2}, {ROW2, ROW3}, {ROW4, ROW3},
                           {ROW3, ROW4}, {ROW4, ROW2}, {ROW2, ROW4},
                           {ROW4, ROW1}, {ROW1, ROW4}, {ROW3, ROW1},
                           {ROW1, ROW3}, {ROW2, ROW1}, {ROW1, ROW2},
};

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

// ms delay for yellow LEDs blinky
volatile uint16_t YLW_LED_SPEED = MED_SPEED;

// current speed state of yellow LEDs
volatile uint8_t YLW_LED_STATE = SLOW_CYCLE;

// set up timer0 for timed events
void timer0_init()
{
    // set timer counter to compare to OCR0A
    TCCR0A |= (1 << CTC0);

    // set clock source to prescaler CLK/256, Timer0 Freq = 8Mhz/256 = 31.25Khz
    TCCR0B |= (1 << CS02);

    // RGB LED Mux timer, 31.25Khz/8 3.906Khz
    OCR0A = 8; 

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

// turn off all the yellow LEDs
void all_off_yellow_led()
{
    // high Z for all ROW pins
    DDRA &= ~ROW4 & ~ROW3 & ~ROW2 & ~ROW1;
    PORTA &= ~ROW4 & ~ROW3 & ~ROW2 & ~ROW1;
}


// set up GPIO pin directions and logic levels
void pins_init()
{
    // enable output
    DDRA |= EN_RGB4 | EN_RGB3 | EN_RGB2 | EN_RGB1;
    DDRB |= B_PWM | G_PWM | R_PWM;

    all_off_yellow_led();

    // enable switches for input 
    DDRB &= ~R_SWITCH & ~L_SWITCH; 

    //enable pullup resistors on switches
    PORTB |= R_SWITCH | L_SWITCH;

    // diable all RGB LEDs
    PORTA &= ~EN_RGB4 & ~EN_RGB3 & ~EN_RGB2 & ~EN_RGB1;

    // RBG LEDs are common Anode, logic one on cathod turns them off
    PORTB |= R_PWM | G_PWM | B_PWM;

    // start with all LEDs off
    R_DUTY_CYCLE = PWM_MAX;
    G_DUTY_CYCLE = PWM_MAX;
    B_DUTY_CYCLE = PWM_MAX;
}


// run all the initialization functions and turn on interupts
void setup()
{
    timer0_init();
    timer1_init();
    pins_init();
    srand(SEED);
    sei();
}


// Multiplex the RGB LEDs
ISR(TIMER0_COMPA_vect)
{
    tick++;

    // turn on a given rgb_led
    PORTA |= en_rgb_led[rgb_led];

    // cycle through the RGB LEDs
    rgb_led++;
    if (NUM_RGB_LEDs == rgb_led)
        rgb_led = 0;

    // read the switch value, ACTIVE LOW!
    if ((PINB & R_SWITCH) == 0) {
        _delay_us(40);
        // transition to next state and speed
        switch(YLW_LED_STATE)
        {
            case SLOW_CYCLE: YLW_LED_STATE = MED_CYCLE;  YLW_LED_SPEED = MED_SPEED; break;
            case MED_CYCLE:  YLW_LED_STATE = FAST_CYCLE; YLW_LED_SPEED = FAST_SPEED; break;
            case FAST_CYCLE: YLW_LED_STATE = MED_RAND;   YLW_LED_SPEED = MED_SPEED; break;
            case MED_RAND:   YLW_LED_STATE = FAST_RAND;  YLW_LED_SPEED = FAST_SPEED; break;
            case FAST_RAND:  YLW_LED_STATE = SLOW_CYCLE; YLW_LED_SPEED = SLOW_SPEED; break;
        }
    }

    // Vary the PWM duty cycle every ~1ms 
    if ((tick % 32) == 0) {
        // set PWM duty cycle for a given color
        *rgb_duty_cycle[color] = PWM_MAX - duty_cycle;
        
        duty_cycle++; //FIXME: chaning the step value sometimes breaks functionality

        if (duty_cycle == PWM_MAX) {
            // turn off the current colore before switching to the next
            *rgb_duty_cycle[color] = PWM_MAX; 
            color++;
            duty_cycle = 0;
        }

        if (NUM_COLORS == color)
            color = 0;
    }

    // turn off a given rgb_led
    PORTA &= ~(en_rgb_led[rgb_led]);
}

// workaround for avr-libc _delay_ms requiring a compile time constant
void _my_delay_ms(uint16_t ms)
{
    while(ms--)
        _delay_ms(1);
}

// turn on a given LED, param is a an {ANODE,CATHODE} pair
void on_yellow_led(uint8_t *LED)
{
    // Set pins to output
    DDRA |= LED[ANODE] | LED[CATHODE];

    // Set output logic
    PORTA |= LED[ANODE];
    PORTA &= ~LED[CATHODE];
}

// turn off a given LED by high Z the LED pins, param is a an {ANODE,CATHODE} pair
void off_yellow_led(uint8_t *LED)
{
    DDRA &= ~LED[ANODE] & ~LED[CATHODE];
    PORTA &= ~LED[ANODE] & ~LED[CATHODE];
}

void blink_yellow_led(uint8_t *LED)
{
    on_yellow_led(LED);
    _my_delay_ms(YLW_LED_SPEED);
    off_yellow_led(LED); 
}

void random_yellow_led()
{
    int rand_led = rand() % NUM_YLW_LEDS;
    blink_yellow_led(LED[rand_led]);
}

void cycle_led()
{
    uint8_t i;

    for (i = 0; i < NUM_YLW_LEDS; i++) {
        if (YLW_LED_STATE >= MED_RAND) break;
        blink_yellow_led(LED[i]);
    }
}

int main()
{
    setup();

    for (;;) {
        if (YLW_LED_STATE >= MED_RAND) {
            random_yellow_led();
        } else {
            cycle_led();
        }
    }
}
