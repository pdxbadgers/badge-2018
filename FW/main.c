#define F_CPU 8000000UL

#include <util/delay.h>
#include <avr/io.h>

int main()
{
    DDRA = (1 << PA1) | (1 << PA0);
    PORTA = 0x02;

    for(;;) {
        PORTA |= (1 << PA0);
        _delay_ms(500);
        PORTA &= ~(1 << PA0);
        _delay_ms(500);
    }
}
