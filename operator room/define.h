#include 	<delays.h>
#include	"GenericTypeDefs.h"

#define USE_OR_MASKS

#include  	<stdlib.h>
#include 	<string.h>
#include 	<timers.h>					// Timer library functions
#include <p18f45k22.h>

#include "Console.h"

#define CLOCK_FREQ          4000000
#define GetInstructionClock()   (CLOCK_FREQ / 4)
#define CIRCBUF_LEN 16

//******************* common ***********************************************

#define piezo		PORTCbits.RC0
#define on			1
#define off			0

#define FLT1		PORTBbits.RB0
#define	OOC1		PORTBbits.RB1		
#define	TRIG1		PORTBbits.RB2	//out of compliance
#define PWR1		PORTBbits.RB3	//fault
#define OOC2		PORTBbits.RB4
#define	TRIG2		PORTBbits.RB5		
#define	PWR2		PORTBbits.RB6	//out of compliance
#define FLT2		PORTBbits.RB7	//fault


#define ctrlPWR		PORTAbits.RA0		//dtmr1 trigger, paralell port
#define linkOK		PORTAbits.RA1		//dtmr2 trigger, paralell port
#define linkNOK		PORTAbits.RA2		//dtmr output enable, paralell port
#define lid			PORTAbits.RA3		//spare
#define blue		PORTAbits.RA4	//out of compliance hiba, paralell port


/*****   COMMON FUNCTION PROTOTYPES   *****/

void isr(void);	
void isrhandler(void);		// Located at high interrupt vector

void Setup(void);			// Configures peripherals and variables
void Console1Init(void);
void Console2Init(void);

void Delay10us( UINT32 tenMicroSecondCounter );
void DelayMs( UINT16 ms );
void DoCommand(void);
