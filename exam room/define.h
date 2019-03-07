#include 	<delays.h>
#include	"GenericTypeDefs.h"

#define USE_OR_MASKS

#include 	<adc.h>
#include	<pwm.h>
#include  	<stdlib.h>
//#include 	<string.h>
#include 	<timers.h>					// Timer library functions
#include 	<p18f46k80.h>

#include "Console.h"

#define CLOCK_FREQ          4000000
#define GetInstructionClock()   (CLOCK_FREQ / 4)
#define CIRCBUF_LEN 16

#define on			1
#define off			0

//******************* inputs ***********************************************

#define flt1		PORTAbits.RA0
#define ooc1		PORTAbits.RA1
#define trig1		PORTAbits.RA2
#define pwr1		PORTAbits.RA3

#define ooc2		PORTAbits.RA5
#define trig2		PORTEbits.RE0
#define pwr2		PORTEbits.RE1
#define flt2		PORTEbits.RE2

#define lidsw		PORTCbits.RC5
#define enbtn		PORTCbits.RC0
#define oe_sw		PORTCbits.RC1
#define oepos_dn	PORTCbits.RC3
#define oepos_up	PORTCbits.RC4


//******************* outputs ***********************************************

#define piezo		PORTDbits.RD1
#define pwr			TRISCbits.TRISC2
#define servo_pwr	TRISDbits.TRISD0
#define PWM			PORTDbits.RD4
#define dtmr2trig	PORTDbits.RD2
#define dtmr1trig	PORTDbits.RD3

/*****   COMMON FUNCTION PROTOTYPES   *****/

void isr(void);	
void isrhandler(void);		// Located at high interrupt vector

void Setup(void);			// Configures peripherals and variables

void Delay10us( UINT32 tenMicroSecondCounter );
void DelayMs( UINT16 ms );
void DoCommand(void);
