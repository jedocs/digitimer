
/************************ HEADERS **********************************/
#include "GenericTypeDefs.h"

#define BAUD_RATE_USB 19200

#define BAUD_RATE_OPTO 9600

/************************ DEFINITIONS ******************************/
        #define USBIsPutReady()     (TXSTA1bits.TRMT)
		#define OptoIsPutReady()     (TXSTA2bits.TRMT)
	//	#define USB_RCIF	PIR1bits.RC1IF
		#define USB_RCREG	RCREG1
		#define Opto_RCREG	RCREG2

        void USBPut(BYTE c);
        void USBPutString(BYTE *s);
        void USBPutROMString(rom char* str);

		void OptoPut(BYTE c);
        void OptoPutString(BYTE *s);
        void OptoPutROMString(rom char* str);

        #define OptoIsGetReady     PIR3bits.RC2IF
		#define USBIsGetReady     PIR1bits.RC1IF

        BYTE USBGet(void);
        BYTE USBGetString(char *buffer, BYTE bufferLen);
        void USBPrintChar(BYTE);
        void USBPrintDec(BYTE);

//		#define Printf1(x) USBPutROMString((rom char*)x)

		BYTE OptoGet(void);
        BYTE OptoGetString(char *buffer, BYTE bufferLen);
        void OptoPrintChar(BYTE);
        void OptoPrintDec(BYTE);


//		#define Printf2(x) OptoPutROMString((rom char*)x)

