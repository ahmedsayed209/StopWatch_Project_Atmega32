/*
 * Project.c
 *
 *  Created on: Oct 19, 2023
 *      Author: Ahmed Sayed
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char seconds_count=0;
unsigned char minutes_counts=0;
unsigned char hours_counts=0;
unsigned char I_flag=0;
ISR(TIMER1_COMPA_vect)
{
	I_flag=1;
}
ISR(INT0_vect)
{
	 seconds_count=0;
	 minutes_counts=0;
	 hours_counts=0;
}
ISR(INT1_vect)
{
	TCCR1B = (TCCR1B&0XF8);
}
ISR(INT2_vect)
{
	TCCR1B |= (1<<CS10) | (1<<CS12);
}
void Timer1_ini (unsigned short tim)
{
	TCCR1A =(1<<FOC1A)|(1<<FOC1B) ;
	TCCR1B =(1<<WGM12)|(1<<CS12) | (1<<CS10);
	TCNT1=0;
	OCR1A=tim;
	TIMSK|=(1<<OCIE1A);
	SREG|=(1<<7);
}

void INT0_INI(void)
{
	MCUCR|=(1<<ISC01);
	GICR|=(1<<INT0);
	DDRD &=~(1<<2);
	PORTD|=(1<<2);
	SREG|=(1<<7);
}

void INT1_INI(void)
{
	MCUCR|=(1<<ISC11) | (1<<ISC10);
	GICR|=(1<<INT1);
	DDRD &=~(1<<3);
	PORTD|=(1<<3);
	SREG|=(1<<7);
}
void INT2_INI(void)
{
	MCUCSR &=~(1<<ISC2);
	GICR|=(1<<INT2);
	DDRB&=~(1<<2);
	PORTB|=(1<<2);
	SREG|=(1<<7);
}

int main(void)
{
	DDRC |=0X0F;
	PORTC=0X00;
	DDRA=0x3F;
	PORTA|=0X3F;
	Timer1_ini(1000);
	INT0_INI();
	INT1_INI();
	INT2_INI();
	for(;;)
	{
		if(I_flag==1)
		{
			seconds_count++;
			if(seconds_count==60)
			{
				seconds_count=0;
				minutes_counts++;
			}
			if(minutes_counts==60)
			{
				minutes_counts=0;
				hours_counts++;
			}
			if(hours_counts==24)
			{
				hours_counts=0;
			}
			I_flag=0;
		}
		else
		{
			PORTA=(PORTA&0x00) | 0X01;
			PORTC=(PORTC&0XF0) | (seconds_count%10);
			_delay_ms(2);
			PORTA=(PORTA&0x00) | 0X02;
			PORTC=(PORTC&0XF0) | (seconds_count/10);
			_delay_ms(2);
			PORTA=(PORTA&0x00) | 0X04;
			PORTC=(PORTC&0XF0) | (minutes_counts%10);
			_delay_ms(2);
			PORTA=(PORTA&0x00) | 0X08;
			PORTC=(PORTC&0XF0) | (minutes_counts/10);
			_delay_ms(2);
			PORTA=(PORTA&0x00) | 0X10;
			PORTC=(PORTC&0XF0) | (hours_counts%10);
			_delay_ms(2);
			PORTA=(PORTA&0x00) | 0X20;
			PORTC=(PORTC&0XF0) | (hours_counts/10);
			_delay_ms(2);

		}

	}
}
