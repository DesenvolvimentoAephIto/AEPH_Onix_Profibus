/* 
 * File: CRC16
 * Author: Eng. Tarcisio Souza de Melo
 * Comments:
 * 
 */

#ifndef __CRC16_h_INCLUDED
#define __CRC16_h_INCLUDED

/*Includes--------------------------------------------------------------------------------*/
#include <Arduino.h>
#include "main.h"
/*End Includes----------------------------------------------------------------------------*/


/*Defines---------------------------------------------------------------------------------*/
  
/*End Defines-----------------------------------------------------------------------------*/


/* Private typedef------------------------------------------------------------------------*/
typedef struct{
	uint8_t *pointerData;
	uint8_t lenghtData;
	uint8_t lowCRC16;
	uint8_t highCRC16;
}DataCRC;

/* Private variables----------------------------------------------------------------------*/



/*Global Variables------------------------------------------------------------------------*/

/*End Global Variables--------------------------------------------------------------------*/


/*Prototypes Functions--------------------------------------------------------------------*/
void vCalcCRC16(DataCRC *CalcCRC16);
/*End Prototype Functions-----------------------------------------------------------------*/


/*Functions-------------------------------------------------------------------------------*/
void vCalcCRC16(DataCRC *CalcCRC16)
{
    const uint16_t constCalc = 0xA001;

    uint16_t crc =      0xFFFF;
    uint8_t dataByte =  0x00;
    uint8_t checkcalc = 0x00;
    uint8_t _lenght =   0x00;
    uint8_t aux00 =     0x00;
    uint8_t aux01 =     0x00;

    CalcCRC16->highCRC16 = 0xFF;
    CalcCRC16->lowCRC16 = 0xFF;

    _lenght = CalcCRC16->lenghtData;
    //*_data = CalcCRC16->pointerData;

    for(aux00 = 0x00; aux00 < _lenght; aux00++)
    {
    	dataByte = CalcCRC16->pointerData[aux00];
    	crc ^= dataByte;
    	for(aux01 = 0x00; aux01 < 8; aux01++)
    	{
    		checkcalc = crc & 0x01;
    		crc >>= 1;
    		if(checkcalc)
    		{
    			crc ^= constCalc;
    		}
    	}
    }
    CalcCRC16->lowCRC16 = crc;
    crc >>= 8;
    CalcCRC16->highCRC16 = crc;

}

#endif