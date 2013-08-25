#define F_CPU 8000000UL
#define BAUD 38400

#include <avr/io.h>
#include <util/setbaud.h>

//ATtiny2313 �ł� PD0:RxD PD1:TxD
#define UART_RX   0
#define UART_TX   1
#define OUT_RELAY 0


/**
 *
 * PB0: �����[��ON/OFF�M�����o���B
 * PD0: UART��RX
 * PD1: UART��TX
 *
 */
void ioinit(void)
{
	sbi(DDRB, OUT_RELAY);
	
	sbi(DDRD, UART_TX);
	cbi(DDRD, UART_RX);
	
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	#if USE_2X
	UCSRA |= (1 << U2X);
	#else
	UCSRA &= ~(1 << U2X);
	#endif
	
	// UART��M
	UCSRB = _BV(RXEN);
}

void uart_putchar(char c)
{
	//loop_until_bit_is_set(UCSR0A, UDRE0);
	UDR0 = c;
	loop_until_bit_is_set(UCSR0A, TXC0);
}

char uart_getchar(void)
{
	loop_until_bit_is_set(UCSR0A, RXC0);
	return UDR0;
}
	
int main(void)
{
	ioinit();
	
	for(;;)
	{
		char c = uart_getchar();
		switch(c) {
			case 0xfe:
				c = uart_getchar();
				switch(c) {
					case 0x01:
						sbi(PORTB, OUT_RELAY);
						break;
					case 0x02:
						cbi(PORTB, OUT_RELAY);
						break;
					default:
						uart_putchar(0xee);
				}
				break;
			default:
				uart_putchar(0xee);
		}
	}
	
	return 0;
}

