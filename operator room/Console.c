#include "define.h"

/************************ VARIABLES ********************************/
rom unsigned char CharacterArray[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    
/************************ DEFINITIONS ******************************/
    #define SPBRG2_VAL   ( ((CLOCK_FREQ/BAUD_RATE_OPTO)/16) - 1)

#if SPBRG2_VAL > 255
    #error "Calculated SPBRG value is out of range for currnet CLOCK_FREQ."
#endif

/************************ FUNCTIONS ********************************/
    
/*********************************************************************
* Side Effects:	    UART2 is configured
*
* Overview:		    This function will configure the UART2 for use at 
*                   in 8 bits, 1 stop, no flowcontrol mode
********************************************************************/
void OptoInit(void)
{		
		BAUDCON2bits.DTRXP=1;	//RX inverted polarity
		BAUDCON2bits.CKTXP=1;	//TX inverted polarity
        TRISDbits.TRISD7 = 1;
        TRISDbits.TRISD6 = 0;
        TXSTA2 = 0x24;
    
        RCSTA2 = 0x90; // 0b10010000;
        SPBRG2 = SPBRG2_VAL;
}

/*********************************************************************
* Overview:		    This function will print the inputed ROM string
********************************************************************/
void OptoPutROMString(rom char* str)
{
    BYTE c;

    while( c = *str++ )
        OptoPut(c);
}

/*********************************************************************
* Side Effects:	    c is printed to the console
********************************************************************/
void OptoPut(BYTE c)
{
    while( !OptoIsPutReady() );
        TXREG2 = c;
}

/*********************************************************************
* Overview:		    This function will receive one byte from UART
********************************************************************/
BYTE OptoGet(void)
{
        // Clear overrun error if it has occured
        // New bytes cannot be received if the error occurs and isn't cleared
        if(RCSTA2bits.OERR)
        {
            RCSTA2bits.CREN = 0;   // Disable UART receiver
            RCSTA2bits.CREN = 1;   // Enable UART receiver
        }

        return RCREG2;
}


/*********************************************************************
* Overview:		    This function will print the inputed BYTE to 
********************************************************************/
void OptoPrintChar(BYTE toPrint)
{
    BYTE PRINT_VAR;
    PRINT_VAR = toPrint;
    toPrint = (toPrint>>4)&0x0F;
    OptoPut(CharacterArray[toPrint]);
    toPrint = (PRINT_VAR)&0x0F;
    OptoPut(CharacterArray[toPrint]);
    return;
}

/*********************************************************************
* Overview:		    This function will print the inputed BYTE to 
*                   the console in decimal form
********************************************************************/
void OptoPrintDec(BYTE toPrint)
{
    if( toPrint >= 100 )
        OptoPut(CharacterArray[toPrint/100]);
    if( toPrint >= 10 )
        OptoPut(CharacterArray[(toPrint%100)/10]);
    OptoPut(CharacterArray[toPrint%10]);
}



/************************ DEFINITIONS ******************************/
#define SPBRG1_VAL   ( ((CLOCK_FREQ/BAUD_RATE_USB)/16) - 1)

#if SPBRG1_VAL > 255
    #error "Calculated SPBRG value is out of range for currnet CLOCK_FREQ."
#endif

/************************ FUNCTIONS ********************************/
    
/*********************************************************************
* Side Effects:	    UART1 is configured
*
* Overview:		    This function will configure the UART2 for use at 
*                   in 8 bits, 1 stop, no flowcontrol mode
********************************************************************/
void USBInit(void)
{
        TRISCbits.TRISC7 = 1;
        TRISCbits.TRISC6 = 0;
        TXSTA1 = 0x24;
        RCSTA1 = 0x90; // 0b10010000;
        SPBRG1 = SPBRG1_VAL;
}

/*********************************************************************
* Overview:		    This function will print the inputed ROM string
********************************************************************/
void USBPutROMString(rom char* str)
{
    BYTE c;

    while( c = *str++ )
        USBPut(c);
}


/*********************************************************************
* Overview:		    This function will print the inputed string
********************************************************************/
void USBPutString(char* s)
{
    BYTE c;

    while( c = *s++ )
        USBPut(c);
}


/*********************************************************************
* Side Effects:	    c is printed to the console
********************************************************************/
void USBPut(BYTE c)
{
    while( !USBIsPutReady() );
        TXREG1 = c;
}

/*********************************************************************
* Overview:		    This function will receive one byte from UART
********************************************************************/
BYTE USBGet(void)
{
        // Clear overrun error if it has occured
        // New bytes cannot be received if the error occurs and isn't cleared
        if(RCSTA1bits.OERR)
        {
            RCSTA1bits.CREN = 0;   // Disable UART receiver
            RCSTA1bits.CREN = 1;   // Enable UART receiver
        }

        return RCREG1;
}


/*********************************************************************
* Overview:		    This function will print the inputed BYTE to 
********************************************************************/
void USBPrintChar(BYTE toPrint)
{
    BYTE PRINT_VAR;
    PRINT_VAR = toPrint;
    toPrint = (toPrint>>4)&0x0F;
    USBPut(CharacterArray[toPrint]);
    toPrint = (PRINT_VAR)&0x0F;
    USBPut(CharacterArray[toPrint]);
    return;
}

/*********************************************************************
* Overview:		    This function will print the inputed BYTE to 
*                   the console in decimal form
********************************************************************/
void USBPrintDec(BYTE toPrint)
{
    if( toPrint >= 100 )
        USBPut(CharacterArray[toPrint/100]);
    if( toPrint >= 10 )
        USBPut(CharacterArray[(toPrint%100)/10]);
    USBPut(CharacterArray[toPrint%10]);
}

