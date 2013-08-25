#include <avr/io.h>
#define F_CPU 8000000UL
#define BAUD 19200
#include <util/setbaud.h>

//ATtiny2313 �ł� PD0:RxD PD1:TxD
#define UART_RX   0
#define UART_TX   1
#define OUT_RELAY 0

#define sbi(PORT,BIT) PORT|=_BV(BIT)
#define cbi(PORT,BIT) PORT&=~_BV(BIT)

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
	sbi(UCSRA, U2X);
	#else
	cbi(UCSRA, U2X);
	#endif
	
	// UART��M
	sbi(UCSRB, RXEN);
	sbi(UCSRB, TXEN);
	UCSRC = 0b00000110;
}

void uart_putchar(unsigned char c)
{
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = c;
	//loop_until_bit_is_set(UCSRA, TXC);
}

unsigned char uart_getchar(void)
{
	loop_until_bit_is_set(UCSRA, RXC);
	return UDR;
}
	
int main(void)
{
	ioinit();
	
	for(;;)
	{
		unsigned char c = uart_getchar();
		uart_putchar(c);
		switch(c) {
			case 0xfe:
				c = uart_getchar();
				switch(c) {
					case 0x01:
						sbi(PORTB, OUT_RELAY);
						uart_putchar(0x11);
						break;
					case 0x02:
						cbi(PORTB, OUT_RELAY);
						uart_putchar(0x11);
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

