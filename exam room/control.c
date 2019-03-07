
#include	"define.h"

char data[9], rconval;

unsigned char	btncnt=0,lidcnt=0,udata,sdata,counter1=8;		//soros komm változók						

unsigned int duty_cycle=378;	//554 közép:378

int temp;

rom char cr[] = "\r";
rom char crlf[] = "\r\n";
rom char ready[] = "\r\n Ok\r\n";	
rom char welcome[] = "\r\n Digitimer vezerlo, exam room beta v1.1. \r\n reset: ";	

	struct 
	{										
    	unsigned    btn:1;
		unsigned    lid:1;
		unsigned    oldbtn:1;
		unsigned 	oldlid:1;
    	unsigned    btnflag:1;	
    	unsigned 	lidflag:1;		
    	unsigned	oe:1;
   		unsigned 	dis:1;
	} stat; 


//---------------------------------------------------------------------´
// Interrupt Code
//---------------------------------------------------------------------

#pragma interrupt isr save = temp	

#pragma code isrcode=0x0008 //0x0808				// Locate ISR handler code at interrupt vector

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
if (PIR1bits.TMR2IF)
	{
		sdata=((PORTC & 0b00100000) | ((PORTA & 0b00100000) >> 1) | (PORTA & 0b00001111)); // lid,ooc2,pwr1,trig1,ooc1,flt1
		OptoPut(sdata | 0b10000000);
		sdata= ((PORTE & 0b00000111) | ( PORTC & 0b00011000)); // x,oepos_up,oepos_dn,flt2,pwr2,trig2 
		OptoPut(sdata | 0b11000000);

	counter1-=1;
	if (!counter1)
		{
		
			counter1=8;

			if (stat.btnflag)
			
				{
				btncnt+=1;
				
				if (!btncnt) 
					{
					stat.btnflag=0;
					stat.btn=stat.oldbtn;
					piezo=0;
					}

				}


			if (stat.lidflag) lidcnt+=1;
			if (!lidcnt) 
			{
				stat.lidflag=0;
				stat.lid=stat.oldlid;
				if (stat.lid & stat.btnflag) 
				{	
					stat.btn=0;
					stat.btnflag=0;
					piezo=0;
				}
			}
			
		}	
		PIR1bits.TMR2IF = 0;
	}
}											

//---------------------------------------------------------------------
//	Setup() initializes program variables and peripheral registers
//---------------------------------------------------------------------

void Setup(void)
	{

	rconval=RCON;		//RESET-status register
	RCON=0b00010011;

	stat.btn=0;
	stat.lid=0;
	stat.oldbtn=0;
	stat.oldlid=0;
	stat.btnflag=0;
	stat.lidflag=0;
	stat.oe=0;
	stat.dis=0;

	ANCON0=0;	// A/D port config, all digital
	ANCON1=0;	// A/D port config, all digital
	
	PORTA=0;
	TRISA=0b11111111;
	
	PORTB=0;
	TRISB=0b11111111;
	
	PORTC=0;
	TRISC=0b11011111;
	
	TRISD=0b11100001;
	PORTDbits.RD0=0;
	
	PORTE=0;
	TRISE=0b00000111;

	piezo=0;
	dtmr1trig=0;
	dtmr2trig=0;
	
	T2CON=0b00111111;	// TMR2 on, TMR2 prescaler = 16, postscaler = 8 -> T2INT ~37,56Hz

	OpenEPWM1(207, ECCP_1_SEL_TMR12);            //Configure PWM module and initialize PWM period (~300,48Hz)
	SetDCEPWM1(duty_cycle);        //set the duty cycle	

    CloseADC();
    ADCON0 = 0b00100001; //CH8
	ADCON1 = 0b00100000; //Vref=2,048V 
	ADCON2 = 0b10111001; // right just, acq=2TAD clock:Fosc/2
	ANCON0 = 0b00000000;	//all dig
	ANCON1 = 0b00000001;	// AN8 (RB1) analog

	RCONbits.IPEN=0;		//no priority
	INTCONbits.PEIE=1;
	PIE1bits.RC1IE=1;
	PIE1bits.TMR2IE=1;
	INTCONbits.GIE=0;
	}


unsigned char OutputEnable(void)
	{
	duty_cycle=445;
	SetDCEPWM1(duty_cycle);
	PORTDbits.RD0=0;	
	servo_pwr = 0;
	DelayMs(500);
	servo_pwr = 1;
	if (oepos_up & !oepos_dn) return 1;
	else return 0;
	}

unsigned char OutputDisable(void)
	{
	duty_cycle=277;
	SetDCEPWM1(duty_cycle);	
	PORTDbits.RD0=0;
	servo_pwr = 0;
	DelayMs(500);
	servo_pwr = 1;
	if (!oepos_up & oepos_dn) return 1;
	else return 0;	
	}

unsigned int tempread(void)
{
  ConvertADC();
    while(BusyADC());
    return  ((ReadADC()-1000)/20);
		
}

//---------------------------------------------------------------------
// main()
//---------------------------------------------------------------------

void main(void)
{
	Setup();								// Setup peripherals and software
	OptoInit();								// init serial over optic

	itoa(rconval, data);					//reset status
	OptoPutROMString(welcome);
	OptoPutString(data);
	OptoPutROMString(ready);
    
	INTCONbits.GIE=1;
	


while(1)                    		

{
if (stat.btnflag & stat.btn)
	{
	if (((btncnt==212) | (btncnt>229 & btncnt<232) | (btncnt > 242) & (!oe_sw))) piezo=1;
	else piezo=0;
	}

	if (stat.oldbtn ^ enbtn)
	{
		stat.oldbtn=enbtn;
		btncnt=1;						// kikapcs elott hosszú késleltetés
		if (stat.oldbtn) btncnt=253;	// bekeapcs elott pergésmentesítés
		stat.btnflag=1;
	}


	if (stat.oldlid ^ lidsw)
	{
		stat.oldlid=lidsw;
		lidcnt=253;
		stat.lidflag=1;
	}

	if ((stat.btn ^ !pwr) & !stat.lid)
		{
		DelayMs(300);
		pwr=!stat.btn;		
		if (pwr) piezo=0;
		
		if (stat.btn)
			{
			OutputDisable();
			DelayMs(4000);
			OutputEnable();
			}
		}

		if (!stat.btn & !pwr & !stat.lid)
		{
			pwr=1;	// lid off -> pwr OFF		
		}

//****************** seriall comm. *********************************************

		if(OptoIsGetReady)						
   			{
   			switch(udata = Opto_RCREG)
	    		{
					case 'T':
						itoa(tempread(), data);
						OptoPutROMString(crlf);   
						OptoPutString(data);
					break;

					case 'L':
						if (stat.lid) OptoPutROMString("lid");
						else OptoPutROMString("err");
					break;
	
					case '9':
						if (OutputEnable()) OptoPutROMString("en");
						else OptoPutROMString("err");
					break;
	
					case '8':
						if (OutputDisable()) OptoPutROMString("dis");
						else OptoPutROMString("err");					
					break;
					
					case 'P':
						if (!stat.lid)
						{
							OptoPutROMString("lid!");
							break;
						}

						if (pwr)
						{
							DelayMs(250);
							pwr=0;		
							DelayMs(250);
						}
				
						if (!(pwr1 & pwr2)) 
						{
							OptoPutROMString("errp");
							break;
						}


						if (flt1 | flt2 | oepos_dn)
							{
								if (!(OutputDisable()))
								{
									OptoPutROMString("errd");
									break;	
								}

								DelayMs(4000);

								if (!(OutputEnable()))
								{
									OptoPutROMString("erre");
									break;	
								}
							}	

						OptoPutROMString("pwr");	
																						 
					break;

					case 'O':
						if (!stat.btn)
							{
								DelayMs(250);
								pwr=1;		
								OptoPutROMString("off");
							}
					break;


				//	case 's':	servo_pwr^=1;		
				//				if (servo_pwr) OptoPutROMString("\r\n srv pwr off");
				//				else OptoPutROMString("\r\n srv pwr on");	
					break;
								
					case '1':	
								dtmr1trig=1;		
								Nop();
								Nop();
								dtmr1trig=0;
								OptoPutROMString("pain");
					break;

					case '2':		

								dtmr2trig=1;		
								Nop();
								Nop();
								dtmr2trig=0;
								OptoPutROMString("touch");
					break;
				
					case 'y':	
								servo_pwr=0;
								if (duty_cycle==451) duty_cycle=299;
								else duty_cycle=451;
								SetDCEPWM1(duty_cycle);        //set the duty cycle
								itoa(duty_cycle, data);
								OptoPutString(data);
					break;
       					

					case '.':	
	     						OptoPutROMString("\r\n");
								duty_cycle+=1;
								SetDCEPWM1(duty_cycle);        //set the duty cycle
								itoa(duty_cycle, data);
								OptoPutString(data);
					break;
       				
					case '-':	
								OptoPutROMString("\r\n");
								duty_cycle-=1;
								SetDCEPWM1(duty_cycle);        //set the duty cycle
								itoa(duty_cycle, data);
								OptoPutString(data);
					break;
       

         		}     										//end switch(udata)
	   	    }       									
	}	
}		

	

