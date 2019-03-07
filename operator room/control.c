
#include	"define.h"

unsigned char	udata, odata;		//soros komm változók						

int temp;

rom char welcome[] = "\r\n Digitimer vezerlo, operator room\r\n";	

//---------------------------------------------------------------------´
// Interrupt Code
//---------------------------------------------------------------------

#pragma interrupt isr save = temp	

#pragma code isrcode=0x0008 //0x0808	// Locate ISR handler code at interrupt vector

void isrhandler(void)					// This function directs execution to the
{										// actual interrupt code										
_asm
goto isr
_endasm
}

#pragma code

//---------------------------------------------------------------------
// isr()
//---------------------------------------------------------------------

void isr(void)
{

}											

//---------------------------------------------------------------------
//	Setup() initializes program variables and peripheral registers
//---------------------------------------------------------------------

void Setup(void)
	{

	ANSELA=0;
	PORTA=0;
	TRISA=0b11000000;
	
	ANSELB=0;
	PORTB=0;
	TRISB=0b00000000;
	
	ANSELC=0;
	PORTC=0;
	piezo=off;
	TRISC=0b11111110;
	
	ANSELD=0;
	PORTD=0;
	TRISD=0b11111111;
	
	ANSELE=0;
	PORTE=0;
	TRISE=0b00000110;
	

	ctrlPWR = 1;
	linkNOK = 0;
	lid=0;
	blue=0;	
//	OpenTimer2(T2_PS_1_4 & T2_POST_1_4 & TIMER_INT_ON);   // 244 Hz int.
	
//	T2CON=0b00000111;
	RCONbits.IPEN=0;		//no priority
	INTCONbits.PEIE=0;
	PIE1bits.RC1IE=1;
	PIE3bits.RC2IE=1;
	INTCONbits.GIE=0;
	}

//---------------------------------------------------------------------
// main()
//---------------------------------------------------------------------

void main(void)
{
	Setup();								// Setup peripherals and software
	USBInit();
	OptoInit();

  	USBPutROMString(welcome);


	while(1)                    		// Loop forever
	{	

		if (!PORTCbits.RC6 | !PORTCbits.RC7) blue=1;
		else blue=0;	

		if(OptoIsGetReady)						// Check for USART interrupt
   		{
			odata=Opto_RCREG;
			
			if (!(odata & 0b10000000)) USBPut(odata);
			else 
			{
				if (odata & 0b01000000)

				{
					PORTB = ((PORTB & 0b00011111) | ((odata & 0b00000111)<< 5));// x,oepos_up,oepos_dn,flt2,pwr2,trig2
				//	if (odata & 0b00010000) ctrlPWR=1;
				//	else ctrlPWR=0;

				//	if (odata & 0b00001000) linkOK=1;
				//	else linkOK=0;
				}
				else
				{
					PORTB = ((PORTB & 0b11100000) | (odata & 0b00011111));// lid,ooc2,pwr1,trig1,ooc1,flt1
					if (odata&0b00100000) lid = 1;
					else lid=0;
				}
			
				if (PORTB & 0b00110110)  piezo = lid; //csak akkor sípol ha teto rajta van, tehát nem kalibrálnak
				else piezo = off;
			}
		}

		if(USBIsGetReady)						// Check for USART interrupt
   		{
			udata=USB_RCREG;
			OptoPut(udata);
			if (udata==63) USBPutROMString("ok");
		}       										
	}	
		
}


