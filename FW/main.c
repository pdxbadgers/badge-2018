#define F_CPU 1000000UL

#include <util/delay.h>
#include <avr/io.h>

#define ROW_1 (1 << PA0)
#define ROW_2 (1 << PA1)
#define ROW_3 (1 << PA2)
#define ROW_4 (1 << PA3)


int main()
{
    DDRA = ROW_4 | ROW_3 | ROW_2 | ROW_1;

    char LED[D0, D1, D2];

    for(;;) {
        PORTA |= (1 << PA2);
        _delay_ms(500);
        PORTA &= ~(1 << PA2);
        _delay_ms(500);
    }
}
