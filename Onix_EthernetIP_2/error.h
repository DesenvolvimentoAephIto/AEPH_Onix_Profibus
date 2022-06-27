/* 
 * File: error
 * Author: Eng. Tarcisio Souza de Melo
 * Comments:
 * 
 */
#ifndef __ERROR_H_INCLUDED
#define __ERROR_H_INCLUDED

/*Includes--------------------------------------------------------------------------------*/
#include <Arduino.h>
#include "main.h"
/*End Includes----------------------------------------------------------------------------*/

/*Defines---------------------------------------------------------------------------------*/
#define MAX_NUM_ERROR       0x0A
/*End Defines-----------------------------------------------------------------------------*/

/* Private typedef------------------------------------------------------------------------*/
/* Private variables----------------------------------------------------------------------*/

/*Global Variables------------------------------------------------------------------------*/

/*End Global Variables--------------------------------------------------------------------*/

/*Prototypes Functions--------------------------------------------------------------------*/
/*End Prototype Functions-----------------------------------------------------------------*/

/*Functions-------------------------------------------------------------------------------*/
void vError0(uint8_t tstError)
{
    if(tstError == MAX_NUM_ERROR)
    {
        /*
         * inserir comando de reset do indicador 
         * inserir comando de reset do ethernet 
         * inserir comando de reset do esp8266
         */
    }
}

#endif