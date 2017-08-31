/*
 * [cbrow025]_lab_part1.c
 * Cory Brown, cbrow025@ucr.edu
 * Lab Partner: Sean Pickman
 * Lab Section: B21
 * Assignment: Lab 9 Exercise 1
 */ 
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.c"

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;



const unsigned long timerPeriod = 1;
unsigned long screen_SM_elapsedTime = 50;

void TimerOn() {
	
	TCCR1B = 0x0B;
	OCR1A = 125;
	TIMSK1 = 0x02;
	TCNT1=0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;
}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}


ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if (_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}


void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR0B &= 0x08; } //stops timer/counter
		else { TCCR0B |= 0x03; } // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR0A = 0xFFFF; }
		
		// prevents OCR0A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR0A = 0x0000; }
		
		// set OCR3A based on desired frequency
		else { OCR0A = (short)(8000000 / (128 * frequency)) - 1;} 
		

		TCNT0 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR0A = (1 << COM0A0) | (1 << WGM01);
	// COM3A0: Toggle PB3 on compare match between counter and OCR0A
	TCCR0B = (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}





//notes used

#define a4s 466.16
#define f4 349.23
#define g4s 415.30
#define f4s 369.99
#define g4 392.00
#define a4 440.00
#define c5 523.25
#define d5 587.33
#define d5s 622.25
#define f5 698.46
#define a3s 245.00
#define c4 261.63
#define d4 293.66
#define d4s 311.13
#define f5s 739.99
#define g5s 830.61
#define a5s 932.33
#define c5s 554.37
#define g5 783.99
#define e5 659.26
#define d7 2349.32
#define c7 2093.00
#define b6 1975.53
#define d6 1174.66
#define e6 1318.51
#define f6 1396.91
#define a6 1760.00
#define g6 1567.98
#define c6 1046.50
#define g6s 1661.22
#define c6s 1108.73
#define a6s 1864.66
#define c7s 2217.46
#define e4 329.63
#define b4 493.88

//song 1
double song1notes[] = {a4s, f4, f4, a4s, g4s, f4s, g4s, a4s, f4, f4, a4s, a4, g4, a4, a4s, f4, a4s, a4s, c5, d5, d5s, f5, f5, f5, f5s, g5s, a5s, a5s, a5s, g5s, f5s, g5s, f5s, f5, f5, d5s, d5s, f5, f5s, f5, d5s, c5s, c5s, d5s, f5, d5s, c5s, c5, c5, d5, e5, g5, f5};
double song1noteTimes[] = {900, 300, 300, 300, 200, 200, 800, 900, 300, 300, 250, 200, 200, 800, 300, 500, 200, 200, 200, 200, 200, 800, 200, 200, 200, 300, 600, 200, 200, 200, 200, 400, 200, 500, 400, 200, 200, 200, 400, 200, 200, 200, 200, 200, 500, 200, 200, 200, 200, 200, 500, 300, 800,0};
double song1timeBetweenNotes[] = {100, 10, 10, 10, 10, 10, 500, 100, 10, 10, 10, 10, 10, 1000, 10, 200, 10, 10, 10, 10, 10, 500, 10, 10, 10, 10, 200, 10, 10, 10, 10, 10, 10, 200, 10, 10, 10, 10, 200, 10, 10, 10, 10, 10, 200, 10, 10, 10, 10, 10, 10, 10, 10};
	
	
	
//song 2
double song2notes[] = {d4, f4, a4, a4, b4, d4, f4, a4, a4, b4, a4, d4, f4, g4, f4, e4, g4, f4s};
double song2noteTimes[] = {400, 400, 400, 400, 800, 400, 400, 400, 400, 800, 400, 600, 300, 200, 200, 300, 300, 900, 0};
double song2timeBetweenNotes[] = {50, 10, 10, 10, 200, 50, 10, 10, 10, 200, 10, 100, 50, 10, 10, 50, 50, 10};
	
//song 3
double song3notes[] = {d4, f4, d5, d4, f4, d5, e5, f5, e5, f5, e5, c5, a4, a4, d4, f4, g4, a4, a4, d4, f4, g4, e4, d4, f4, d5, d4, f4, d5, e5, f5, e5, f5, e5, c5, a4, a4, d4, f4, g4, a4, a4, d4};
double song3noteTimes[] = {200, 200, 600, 200, 200, 600, 200, 150, 150, 150, 150, 200, 600, 200, 200, 150, 150, 400, 200, 200, 150, 150, 400, 200, 200, 600, 200, 200, 600, 200, 150, 150, 150, 150, 200, 600, 200, 200, 150, 150, 400, 200, 500, 0};
double song3timeBetweenNotes[] = {10, 10, 100, 10, 10, 100, 50, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 50, 10, 10, 10, 50, 10, 10, 10, 10, 50, 10, 10, 10, 10, 100, 50, 10 };

//sm1 variables
int cnt1 = 0;
int cnt2 = 0;
int i  = 0;
int j = 0;
int k = 0;
int songSelect = 0;

//sm2 variables
int screen_cnt = 0;
int start_screen = 1;

enum SM_States { SM_init, SM_playNote1, SM_wait1, SM_inc1, SM_idle, SM_playNote2, SM_wait2, SM_inc2, SM_playNote3, SM_wait3, SM_inc3, SM_selectUp1, SM_SelectUp2, SM_selectDwn1, SM_selectDwn2, SM_pause1, SM_pause2, SM_unpause} SM_State;

void playMusic_SM()
{
	unsigned char tmpA = 0x00;
	tmpA = ~PINA;
	
	switch(SM_State) {   // Transitions
		case SM_init:  // SM_initial transition
		SM_State = SM_idle;
		break;
		//song 1 states
		case SM_playNote1:
		if (cnt1 < song1noteTimes[i])
		{
			SM_State = SM_playNote1;
		}
		else if (song1noteTimes[i] == 0)
		{
			SM_State = SM_idle;
		}
		else if (tmpA == 0x01)
		{
			SM_State = SM_pause1;
		}
		else if (tmpA == 0x08)
		{
			SM_State = SM_idle;
		}
		else
		{
			SM_State = SM_wait1;
		}
		break;

		case SM_wait1:
		if (cnt2 < song1timeBetweenNotes[i])
		{
			SM_State = SM_wait1;
		}
		else if (tmpA == 0x01)
		{
			SM_State = SM_pause1;
		}
		else if (tmpA == 0x08)
		{
			SM_State = SM_idle;
		}
		else
		{
			SM_State = SM_inc1;
		}
		break;
		
		case SM_inc1:
		if (tmpA == 0x01)
		{
			SM_State = SM_pause1;
		}
		else if (tmpA == 0x08)
		{
			SM_State = SM_idle;
		}
		else
		{
			SM_State = SM_playNote1;
		}
		break;
		
		//song 2 states
		case SM_playNote2:
		if (cnt1 < song2noteTimes[j])
		{
			SM_State = SM_playNote2;
		}
		else if (song2noteTimes[j] == 0)
		{
			SM_State = SM_idle;
		}
		else if (tmpA == 0x01)
		{
			SM_State = SM_pause1;
		}
		else if (tmpA == 0x08)
		{
			SM_State = SM_idle;
		}
		else
		{
			SM_State = SM_wait2;
		}
		break;

		case SM_wait2:
		if (cnt2 < song2timeBetweenNotes[j])
		{
			SM_State = SM_wait2;
		}
		else if (tmpA == 0x01)
		{
			SM_State = SM_pause1;
		}
		else if (tmpA == 0x08)
		{
			SM_State = SM_idle;
		}
		else
		{
			SM_State = SM_inc2;
		}
		break;
		
		case SM_inc2:
		if (tmpA == 0x01)
		{
			SM_State = SM_pause1;
		}
		else if (tmpA == 0x08)
		{
			SM_State = SM_idle;
		}
		else
		{
			SM_State = SM_playNote2;
		}
		break;
		
		//song 3 states
		
		case SM_playNote3:
		if (cnt1 < song3noteTimes[k])
		{
			SM_State = SM_playNote3;
		}
		else if (song3noteTimes[k] == 0)
		{
			SM_State = SM_idle;
		}
		else if (tmpA == 0x01)
		{
			SM_State = SM_pause1;
		}
		else if (tmpA == 0x08)
		{
			SM_State = SM_idle;
		}
		else
		{
			SM_State = SM_wait3;
		}
		break;

		case SM_wait3:
		if (cnt2 < song3timeBetweenNotes[k])
		{
			SM_State = SM_wait3;
		}
		else if (tmpA == 0x01)
		{
			SM_State = SM_pause1;
		}
		else if (tmpA == 0x08)
		{
			SM_State = SM_idle;
		}
		else
		{
			SM_State = SM_inc3;
		}
		break;
		
		case SM_inc3:
		if (tmpA == 0x01)
		{
			SM_State = SM_pause1;
		}
		else if (tmpA == 0x08)
		{
			SM_State = SM_idle;
		}
		else
		{
			SM_State = SM_playNote3;
		}
		break;
		
		//menu
		case SM_idle:
		if (tmpA == 0x02 && start_screen == 0)
		{
			SM_State = SM_selectUp1;
		}
		else if (tmpA == 0x04 && start_screen == 0)
		{
			SM_State = SM_selectDwn1;
		}
		else if (tmpA == 0x01 && songSelect == 0)
		{
			SM_State = SM_playNote1;
		}
		else if (tmpA == 0x01 && songSelect == 1)
		{
			SM_State = SM_playNote2;
		}
		else if (tmpA == 0x01 && songSelect == 2)
		{
			SM_State = SM_playNote3;
		}
		else
		{
			SM_State = SM_idle;	
		}
		break;
		
		case SM_selectUp1:
		SM_State = SM_SelectUp2;
		break;
		
		case SM_SelectUp2:
		if (tmpA == 0x02)
		{
			SM_State = SM_SelectUp2;
		}
		else
		{
			SM_State = SM_idle;
		}
		break;
		
		case SM_selectDwn1:
		SM_State = SM_selectDwn2;
		break;
		
		case SM_selectDwn2:
		if (tmpA == 0x04)
		{
			SM_State = SM_selectDwn2;
		}
		else
		{
			SM_State = SM_idle;
		}
		break;
		
		case SM_pause1:
		if (tmpA == 0x01)
		{
			SM_State = SM_pause1;
		}
		else
		{
			SM_State = SM_pause2;
		}
		break;
		
		case SM_pause2:
		if (tmpA == 0x01)
		{
			SM_State = SM_unpause;
		}
		else
		{
			SM_State = SM_pause2;
		}
		break;
		
		case SM_unpause:
		if (tmpA == 0x00 && songSelect == 0)
		{
			SM_State = SM_playNote1;
		}
		else if (tmpA == 0x00 && songSelect == 1)
		{
			SM_State = SM_playNote2;
		}
		else if (tmpA == 0x00 && songSelect == 2)
		{
			SM_State = SM_playNote3;
		}
		break;

		default:
		SM_State = SM_init;
		break;
	} // Transitions

	switch(SM_State) {   // Transitions
		case SM_init:  // SM_initial transition
		cnt1 = 0;
		cnt2 = 0;
		i = 0;
		j = 0;
		k = 0;
		break;
		
		//song 1 actions
		case SM_playNote1:
		set_PWM(song1notes[i]);
		lightShow1();
		cnt1++;
		break;

		case SM_wait1:
		set_PWM(0);
		cnt2++;
		break;
		
		case SM_inc1:
		i++;
		cnt1 = 0;
		cnt2 = 0;
		break;
		
		//song 2 actions
		case SM_playNote2:
		set_PWM(song2notes[j]);
		lightShow2();
		cnt1++;
		break;

		case SM_wait2:
		set_PWM(0);
		cnt2++;
		break;
		
		case SM_inc2:
		j++;
		cnt1 = 0;
		cnt2 = 0;
		break;
		
		//song 3 actions
		case SM_playNote3:
		set_PWM(song3notes[k]);
		lightShow3();
		cnt1++;
		break;

		case SM_wait3:
		set_PWM(0);
		cnt2++;
		break;
		
		case SM_inc3:
		k++;
		cnt1 = 0;
		cnt2 = 0;
		break;
		
		//menu actions
		case SM_selectUp1:
		if (songSelect >= 2)
		{
			songSelect = 0;
		}
		else
		{
			songSelect++;
		}
		break;
		
		case SM_SelectUp2:
		break;
		
		case SM_selectDwn1:
		if (songSelect <= 0)
		{
			songSelect =2;
		}
		else
		{
			songSelect--;
		}
		break;
		
		case SM_selectDwn2:
		break;
		
		//pause
		case SM_pause1:
		set_PWM(0);
		break;
		
		case SM_pause2:
		set_PWM(0);
		break;
		
		case SM_unpause:
		break;
		
		 
		
		case SM_idle:
		set_PWM(0);
		cnt1 = 0;
		cnt2 = 0;
		i = 0;
		j = 0;
		k = 0;
		break;
		
		default:
		SM_State = SM_init;
		break;
	}
}

enum Screen_SM_States {screen_init, screen_welcome1, screen_welcome2, screen_song1, screen_song2, screen_song3, screen_begin, screen_idle} screen_state;

	
void screen_SM()
{
	unsigned char tmpA;
	tmpA = ~PINA;
	
	switch(screen_state)
	{
		case screen_init:
		screen_state = screen_welcome1;
		break;
		
		case screen_welcome1:
		if (screen_cnt == 25)
		{
			screen_cnt = 0;
			screen_state = screen_welcome2;
		}
		else
		{
			screen_state = screen_welcome1;
		}
		break;
		
		
		
		case screen_welcome2:
		if (tmpA != 0x00)
		{
			
			screen_state = screen_begin;
		}
		else
		{
			screen_state = screen_welcome2;
		}
		break;
		
		case screen_begin:
		if (tmpA == 0x00)
		{
			screen_state = screen_song1;
		}
		else
		{
			screen_state = screen_begin;
		}
		break;
		
		case screen_song1:
		screen_state = screen_idle;
		break;
		
		case screen_song2:
		screen_state = screen_idle;
		break;
		
		case screen_song3:
		screen_state = screen_idle;
		break;
		
		case screen_idle:
		if (songSelect == 0)
		{
			screen_state = screen_song1;
		}
		else if (songSelect == 1)
		{
			screen_state = screen_song2;
		}
		
		else if (songSelect == 2)
		{
			screen_state = screen_song3;
		}
		else
		{
			screen_state = screen_idle;
		}
		break;
		
	}
	
	switch(screen_state)
	{
		case screen_init:
		start_screen = 1;
		break;
		
		case screen_welcome1:
		LCD_DisplayString(1, "JukeBox V1.0");
		screen_cnt++;
		break;
		
		
		case screen_welcome2:
		LCD_DisplayString(1, "Use up or down  to select songs");
		screen_cnt++;
		break;
		
		case screen_song1:
		start_screen = 0;
		LCD_DisplayString(1, "Zelda 1 Theme");
		break;
		
		case screen_song2:
		LCD_DisplayString(1, "Serenade of     Water");
		break;
		
		case screen_song3:
		LCD_DisplayString(1, "Song of Storms");
		break;
		
		case screen_idle:
		break;
		
		case screen_begin:
		break;
		
	}
	
}

void lightShow1()
{

	
	
	if (song1notes[i] < 300)
	{
		PORTD = 0x01;
	}
	
	else if (song1notes[i] >= 300 && song1notes[i] < 350)
	{
		PORTD = 0x02;
	}
	
	else if (song1notes[i] >= 350 && song1notes[i] < 450)
	{
		PORTD = 0x04;
	}
	
	else if (song1notes[i] >= 450 && song1notes[i] < 550)
	{
		PORTD = 0x08;
	}
	
	else if (song1notes[i] >= 550 && song1notes[i] < 650)
	{
		PORTD = 0x10;
	}
	
	else if (song1notes[i] >= 650)
	{
		PORTD = 0x20;
	}
	
	else
	{
		PORTD = 0x00;
	}
	
	
}

void lightShow2()
{

	
	
	if (song2notes[j] < 300)
	{
		PORTD = 0x01;
	}
	
	else if (song2notes[j] >= 300 && song2notes[j] < 350)
	{
		PORTD = 0x02;
	}
	
	else if (song2notes[j] >= 350 && song2notes[j] < 450)
	{
		PORTD = 0x04;
	}
	
	else if (song2notes[j] >= 450 && song2notes[j] < 550)
	{
		PORTD = 0x08;
	}
	
	else if (song2notes[j] >= 550 && song1notes[j] < 650)
	{
		PORTD = 0x10;
	}
	
	else if (song2notes[j] >= 650)
	{
		PORTD = 0x20;
	}
	
	else
	{
		PORTD = 0x00;
	}
	
	
}

void lightShow3()
{

	
	
	if (song3notes[k] < 300)
	{
		PORTD = 0x01;
	}
	
	else if (song3notes[k] >= 300 && song3notes[k] < 350)
	{
		PORTD = 0x02;
	}
	
	else if (song3notes[k] >= 350 && song3notes[k] < 450)
	{
		PORTD = 0x04;
	}
	
	else if (song3notes[k] >= 450 && song3notes[k] < 550)
	{
		PORTD = 0x08;
	}
	
	else if (song3notes[k] >= 550 && song3notes[k] < 650)
	{
		PORTD = 0x10;
	}
	
	else if (song3notes[k] >= 650)
	{
		PORTD = 0x20;
	}
	
	else
	{
		PORTD = 0x00;
	}
	
	
}


int main() {
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00; 
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00; 
	
	
	SM_State = SM_init; 
	screen_state = screen_init;
	
	LCD_init();
	
	
	//initilizing PWM
	PWM_on();
	set_PWM(0);
	set_PWM(1);
	set_PWM(0);
	
	TimerSet(timerPeriod);
	TimerOn();
	
	
	

	while(1) {
		
		playMusic_SM();
		if (screen_SM_elapsedTime >= 50)
		{
			screen_SM();
			screen_SM_elapsedTime = 0;
		}
		
		
		
		while (!TimerFlag);
		TimerFlag = 0;
		screen_SM_elapsedTime += timerPeriod;
		
	}
	
	return 0;
}

