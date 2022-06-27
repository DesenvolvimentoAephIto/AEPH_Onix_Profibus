/* 
 * File: ISR_TICKER
 * Author: Eng. Tarcisio Souza de Melo
 * Comments:
 * 
 */
#ifndef __ISR_TICKER_H_INCLUDED
#define __ISR_TICKER_H_INCLUDED

/*Includes--------------------------------------------------------------------------------*/
#include "main.h"
#include <Arduino.h>
#include <string.h>
#include "CRC16.h"
/*End Includes----------------------------------------------------------------------------*/

/*Defines---------------------------------------------------------------------------------*/

/*End Defines-----------------------------------------------------------------------------*/

/* Private typedef------------------------------------------------------------------------*/

/* Private variables----------------------------------------------------------------------*/

/*Global Variables------------------------------------------------------------------------*/
extern uint8_t u8_errorCOM;
extern uint32_t u32_stateMachine;
extern uint8_t bufferRxMatrix[100];
extern uint8_t bufferNetIC[100];

extern SoftwareSerial netIC_Serial;

extern uint8_t u8_lock;
extern uint8_t u8_lock_req; 

extern SendMsg xMsg;

extern DataCRC xCalcCRC16;

const uint8_t Led = 2;

extern uint16_t u16_RegisterModbus[16];

extern uint8_t u8_enable_netic;
extern uint8_t u8_req_netic;
/*End Global Variables--------------------------------------------------------------------*/

/*Prototypes Functions--------------------------------------------------------------------*/

/*End Prototype Functions-----------------------------------------------------------------*/

/*Functions-------------------------------------------------------------------------------*/
//antes não tinha *buffer era direto  
uint8_t vAuxCheckRecieve(uint8_t *buffer, uint8_t sizeMSG)
{
    u8_errorCOM++;
    if (Serial.available() >= sizeMSG)
    {
        Serial.readBytes(buffer, sizeMSG);
        xCalcCRC16.pointerData = buffer;
        xCalcCRC16.lenghtData = sizeMSG;
        vCalcCRC16(&xCalcCRC16);
        if (xCalcCRC16.lowCRC16 == 0x00 && xCalcCRC16.highCRC16 == 0x00)
        {
            /*Apenas teste  modificar*/
            //u8_errorCOM = 0x00;
            //xMsg.u8_bufferMsg = bufferRxMatrix;
            //xMsg.u8_sizeBuffer = sizeMSG;
            //vSend_netIC(xMsg);
            return 0x01;
        }
        return 0x00;
    }
}

uint8_t vAuxCheckRecieveNetIC(uint8_t *buffer, uint8_t sizeMSG)
{
    u8_errorCOM++;
    if (netIC_Serial.available() >= sizeMSG)
    {
        netIC_Serial.readBytes(buffer, sizeMSG);
        xCalcCRC16.pointerData = buffer;
        xCalcCRC16.lenghtData = sizeMSG;
        vCalcCRC16(&xCalcCRC16);
        if (xCalcCRC16.lowCRC16 == 0x00 && xCalcCRC16.highCRC16 == 0x00)
        {
            /*Apenas teste  modificar*/
            //u8_errorCOM = 0x00;
            //xMsg.u8_bufferMsg = buffer;
            //xMsg.u8_sizeBuffer = sizeMSG;
            //vSend_netIC(xMsg);
            return 0x01;
        }
        return 0x00;
    }
}



void vSetRegisterModbus(uint8_t u8_posMen, uint8_t u8_posbuff1, uint8_t u8_posbuff2)
{
    u16_RegisterModbus[u8_posMen] =     bufferRxMatrix[u8_posbuff1];
    u16_RegisterModbus[u8_posMen] =   u16_RegisterModbus[u8_posMen] << 8;
    u16_RegisterModbus[u8_posMen] |=    bufferRxMatrix[u8_posbuff2];
}

void ISR_LifeLED()
{
    digitalWrite(Led, !(digitalRead(Led)));
}

void ISR_TIM_COM_MATRIX1()
{
    /*LEITURA DE PESO*/
    if (u32_stateMachine == 1)          
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 9))
        {
            vSetRegisterModbus(0x00, 0x03, 0x04);
            vSetRegisterModbus(0x01, 0x05, 0x06);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_req_netic++;
                u8_lock_req = 0x00;
            }
        }
        else
        {
            if(u8_enable_netic)
            {
                u8_req_netic = 0x01;
                u8_lock_req = 0x00;
            }

            u8_lock = 0;
        }
    }
    /*LEITURA TARA*/
    else if (u32_stateMachine == 2)
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 9))
        {
            vSetRegisterModbus(0x02, 0x03, 0x04);
            vSetRegisterModbus(0x03, 0x05, 0x06);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0xEE;
            }
        }
        else
        {
            u8_lock = 0;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0x00;
            }            
        }
    }
    /*LEITURA DE FILTRO*/
    else if (u32_stateMachine == 3)
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 7))
        {
            vSetRegisterModbus(0x04, 0x03, 0x04);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0xEE;
            }
        }
        else
        {
            if(u8_enable_netic) 
            {
                u8_lock_req = 0x00;
            } 
            u8_lock = 0;
        }
    }
    /*LEITURA DE FATOR DE SENSIBILIDADE*/
    else if (u32_stateMachine == 4)
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 7))
        {
            vSetRegisterModbus(0x05, 0x03, 0x04);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_req_netic++;
                u8_lock_req = 0x00;
            }
        }
        else
        {
            if(u8_enable_netic)
            {
                u8_req_netic = 0x02;
                u8_lock_req = 0x00;
            }
            u8_lock = 0;
        }
    }
    /*LEITURA DE PRAP*/
    else if (u32_stateMachine == 5)
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 7))
        {
            vSetRegisterModbus(0x06, 0x03, 0x04);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_req_netic++;
                u8_lock_req = 0x00;
            }
        }
        else
        {
            if(u8_enable_netic)
            {
                u8_req_netic = 0x02;
                u8_lock_req = 0x00;
            }
            u8_lock = 0;
        }
    }
    /*LEITURA DE UNIDADE DE LEITURA*/
    else if (u32_stateMachine == 6)
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 7))
        {
            vSetRegisterModbus(0x07, 0x03, 0x04);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_req_netic++;
                u8_lock_req = 0x00;
            }
        }
        else
        {
            if(u8_enable_netic)
            {
                u8_req_netic = 0x02;
                u8_lock_req = 0x00;
            }
            u8_lock = 0;
        }
    }
    /*RESPOSTA DE AÇÃO DE TARA*/
    else if (u32_stateMachine == 7)
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 8))
        {
            vSetRegisterModbus(0x08, 0x03, 0x04);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_req_netic++;
                u8_lock_req = 0x00;
            }
        }
        else
        {
            if(u8_enable_netic)
            {
                u8_req_netic = 0x02;
                u8_lock_req = 0x00;
            }
            u8_lock = 0;
        }
    }
    /*RESPOSTA DE AÇÃO DE ZERO*/
    else if (u32_stateMachine == 8)
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 8))
        {
            vSetRegisterModbus(0x09, 0x03, 0x04);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0xEE;
            }
        }
        else
        {
            u8_lock = 0;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0x00;
            }
        }
    }
    /*RESPOSTA DE IMPRESSÃO*/
    else if (u32_stateMachine == 9)
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 8))
        {
            vSetRegisterModbus(0x0A, 0x00, 0x01);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0xEE;
            }
        }
        else
        {
            if(u8_enable_netic) 
            {
                u8_lock_req = 0x00;
            }
            u8_lock = 0;
        }
    }
    /*RESPOSTA DE AÇÃO DE PRAP*/
    else if (u32_stateMachine == 10)
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 7))
        {
            vSetRegisterModbus(0x0B, 0x00, 0x01);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0xEE;
            }
        }
        else
        {
            if(u8_enable_netic) 
            {
                u8_lock_req = 0x00;
            }
            u8_lock = 0;
        }
    }
    /*RESPOSTA DE ALTERAÇÃO DE FILTRO*/
    else if (u32_stateMachine == 11)
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 8))
        {
            vSetRegisterModbus(0x0C, 0x00, 0x01);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0xEE;
            }
        }
        else
        {
            if(u8_enable_netic) 
            {
                u8_lock_req = 0x00;
            }
            u8_lock = 0;
        }
    }
    /*RESPOSTA DE ALTERAÇÃO DE FATOR DE SENSIBILIDADE*/
    else if (u32_stateMachine == 12)
    {
        if(vAuxCheckRecieve(bufferRxMatrix, 8))
        {
            vSetRegisterModbus(0x0D, 0x03, 0x04);
            u8_lock = 2;
            u8_errorCOM = 0;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0xEE;
            }
        }
        else
        {
            if(u8_enable_netic) 
            {
                u8_lock_req = 0x00;
            }
            u8_lock = 0;
        }
    }
    /*RESPOSTA DE REQUISIÇÕES ARMAZENADAS EM NETIC*/
    else if (u32_stateMachine == 13)
    {
        if(vAuxCheckRecieveNetIC(bufferNetIC, 29))
        {
            u8_errorCOM = 0;
            u8_lock = 3;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0xEE;
            }
        }
        else
        {
            u8_lock = 0;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0x00;
            }
        }
    }
    /*RESPOSTA DO ENVIO DE REQUISIÇÕES */
    else if (u32_stateMachine == 14)
    {
        if(vAuxCheckRecieveNetIC(bufferNetIC, 7))
        {
            u8_errorCOM = 0;
            u8_lock = 3;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0xEE;
            }
        }
        else
        {
            u8_lock = 0;
            if(u8_enable_netic) 
            {
                u8_lock_req = 0x00;
            }
        }
    }
    
    memset(bufferRxMatrix, 0x00, 0x64);
    //memset(bufferNetIC, 0x00, 0x64);

    vError0(u8_errorCOM);
}

#endif
