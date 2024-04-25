/*
 * main.c
 *
 *  Created on: Sep 12, 2023
 *      Author: abdo
 */
/****************************************libraries*************************************************/
#include<avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
/****************************************variable***************************************************/
unsigned char s1 = 0;
unsigned char s = 0;
unsigned char m = 0;
unsigned char m1 = 0;
unsigned char h1 = 0;
unsigned char h = 0;
unsigned char SEV[6];
/****************************************function***************************************************/

void int0() {
	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);
	MCUCR |= (1 << ISC01);
	GICR |= (1 << INT0);

}
void int1() {
	DDRD &= ~(1 << PD3);
	MCUCR |= (1 << ISC11) | (1 << ISC10);
	GICR |= (1 << INT1);

}
void int2() {
	DDRB &= ~(1 << PB2);
	PORTB |= (1 << PB2);
	MCUCSR &= ~(1 << ISC2);
	GICR |= (1 << INT2);

}
void TIM1() {
	TCCR1A |= (1 << FOC1A);
	TCNT1 = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
	OCR1A = 977;
	TIMSK |= (1 << OCIE1A);
}
//void SEV_SEGMANT_W() {
//
//	SEV[0]++;
//	if (SEV[0] == 10) {
//		SEV[0] = 0;
//		SEV[1]++;
//		if (SEV[1] == 6) {
//			SEV[1] = 0;
//			SEV[2]++;
//		}
//	}
//	if (SEV[2] == 10) {
//		SEV[2] = 0;
//		SEV[3]++;
//		if (SEV[3] == 6) {
//			SEV[3] = 0;
//			SEV[4]++;
//		}
//	}
//
//	if (SEV[4] == 10) {
//		SEV[4] = 0;
//		SEV[5]++;
//	}
//	if (SEV[5] == 2 && SEV[4] == 4) { // the watch will rest  when reach 24 hours
//		unsigned char var;
//		for (var = 0; var < 6; ++var) {
//			SEV[var] = 0;
//		}
//	}
//
//}

/*************************************************main*********************************************/
int main() {
	SREG |= (1 << 7);
	DDRC |= 0x0F;
	DDRA |= 0x3f;
	PORTA &= 0x00;
	PORTC &= 0x00;
	TIM1();
	int0();
	int1();
	int2();
//	unsigned char var;

	while (1) {

//		for (var = 0; var < 6; var++) {
//
//			PORTA = 1 << var;
//
//			PORTC = SEV[var];
//
//			_delay_ms(3);
//		}

		PORTA = 0x01;
		PORTC = s;
		_delay_ms(5);

		PORTA = 0x02;
		PORTC = s1;
		_delay_ms(5);

		PORTA = 0x04;
		PORTC = m;
		_delay_ms(5);

		PORTA = 0x08;
		PORTC = m1;
		_delay_ms(5);

		PORTA = 0x10;
		PORTC = h;
		_delay_ms(5);

		PORTA = 0x20;
		PORTC = h1;
		_delay_ms(5);

	}

}
/*******************************************ISR******************************************************/

ISR(TIMER1_COMPA_vect) {
	SREG |= (1 << 7);
//	SEV_SEGMANT_W();

	s++;
	if (s > 9) {
		s = 0;
		s1++;
	}
	if (s1 > 5) {
		s = s1 = 0;
		m++;
	}
	if (m > 9) {
		s = s1 = m = 0;
		m1++;
	}
	if (m1 > 5) {
		s = s1 = m = m1 = 0;
		h++;
	}
	if (h > 9) {
		s = s1 = m = m1 = h = 0;
		h1++;
	}
	if (h1 > 9) {
		s = s1 = m = m1 = h = h1 = 0;

	}

}

ISR(INT0_vect) {
	s = s1 = h = h1 = m = m1 = 0;
	SEV[0] = SEV[1] = SEV[2] = SEV[3] = SEV[4] = SEV[5] = 0;
}

ISR(INT1_vect) {

	TCCR1B &= ~(1 << CS10) & ~(1 << CS12);

}

ISR(INT2_vect) {

	TCCR1B |= (1 << CS12) | (1 << CS10);

}
