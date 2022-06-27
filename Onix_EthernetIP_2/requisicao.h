/* 
 * File: Requisicao
 * Author: Eng. Tarcisio Souza de Melo
 * Comments:
 * 
 */

#ifndef __REQUISICAO_H_INCLUDED
#define __REQUISICAO_H_INCLUDED

/*Includes--------------------------------------------------------------------------------*/
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "CRC16.h"
#include "main.h"
#include "string.h"

/*End Includes----------------------------------------------------------------------------*/

/*Defines---------------------------------------------------------------------------------*/
/*MODBUS LIBERADO*/
#define SetTara     0x01
#define SetZero     0x02
#define SetDestara  0x03
#define SetImpr     0x04

#define GetWeight   0x0A
#define GetTara     0x0C

#define GetFilters  0x10
#define SetFilters  0x11

#define GetFSensib  0x12
#define SetFSensib  0x13

#define GetPrAP     0x14
#define SetPrAP     0x15


/*MODBUS TRAVADO*/
#define GetUnL      0x16
#define SetUnL      0x17

#define GetCAPAC    0x18
#define SetCAPAC    0x19

#define GetDecim    0x1A
#define SetDecim    0x1B

#define GetLowDiv   0x1C
#define SetLowDiv   0x1D

#define GetPECAL    0x1E
#define SetPECAL    0x1F

#define GetSPESO    0x20
#define SetSPESO    0x21

#define GetCPESO    0x22
#define SetCPESO    0x23

#define SizeRequest     0x08
#define SizeRequestCalc 0x06

#define CalibActive 0x00

/*NetIC*/
#define STARTADDNETIC   0x03E8  
#define QTDREGISTER     0x000C
#define ADDNETIC        0x01

/*End Defines-----------------------------------------------------------------------------*/

/*Global Variables------------------------------------------------------------------------*/
extern uint8_t bufferNetIC[100];

extern SoftwareSerial netIC_Serial;
/*End Global Variables--------------------------------------------------------------------*/

/*Prototypes Functions--------------------------------------------------------------------*/
void vRequesition(uint8_t u8_address, uint8_t u8_action, uint8_t *u8_requisition);

extern void vCalcCRC16(DataCRC *CalcCRC16);

/*End Prototype Functions-----------------------------------------------------------------*/

/*Functions-------------------------------------------------------------------------------*/

void vRequesition(uint8_t u8_address, uint8_t u8_action, uint8_t *u8_requisition)
/*Atributo u8_requisition com valor default*/
{
    uint8_t u8_aux00 = 0x00;
    DataCRC xCalcCRC16;

    uint8_t u8_request[8];

    u8_request[0] = u8_address;
    u8_request[2] = 0x00;
    u8_request[4] = 0x00;

    if (u8_action == SetDestara) u8_action = SetZero;

    if(u8_action == SetTara)
    {
        u8_request[1] = 0x06;
        u8_request[3] = 0x02;
        u8_request[5] = 0x01;
    }
    else if (u8_action == SetZero)
    {
        u8_request[1] = 0x06;
        u8_request[3] = SetZero;
        u8_request[5] = 0x02;
    }
    else if (u8_action == SetImpr)
    {
        u8_request[1] = 0x06;
        u8_request[3] = SetImpr;
        u8_request[5] = 0x04;       
}
    else if (u8_action == GetWeight)
    {
        u8_request[1] = 0x03;
        u8_request[3] = GetWeight;
        u8_request[5] = 0x02;
    }
    else if (u8_action == GetTara)
    {
        u8_request[1] = 0x03;
        u8_request[3] = GetTara;
        u8_request[5] = 0x02;
    }
    else if (u8_action == GetFilters)
    {
        u8_request[1] = 0x03;
        u8_request[3] = 0x14;
        u8_request[5] = 0x01;
    }
    else if (u8_action == SetFilters)
    {
        u8_request[1] = 0x06;
        u8_request[3] = 0x14;
        u8_request[4] = *u8_requisition;
        u8_requisition += 1;
        u8_request[5] = *u8_requisition;
    }
    else if (u8_action == GetFSensib)
    {
        u8_request[1] = 0x03;
        u8_request[3] = 0x13;
        u8_request[5] = 0x01;
    }
    else if (u8_action == SetFSensib)
    {
        u8_request[1] = 0x06;
        u8_request[3] = 0x13;
        u8_request[4] = *u8_requisition;
        u8_requisition += 1;
        u8_request[5] = *u8_requisition;
    }
    else if (u8_action == GetPrAP)
    {
        u8_request[1] = 0x03;
        u8_request[3] = 0x12;
        u8_request[5] = 0x01;
    }
    else if (u8_action == SetPrAP)
    {
        u8_request[1] = 0x06;
        u8_request[3] = 0x12;
        u8_request[4] = *u8_requisition;
        u8_requisition += 1;
        u8_request[5] = *u8_requisition;
    }
    else if (u8_action == GetUnL)
    {
        u8_request[1] = 0x03;
        u8_request[3] = 0x11;
        u8_request[5] = 0x01;
    }
    else if (u8_action == SetUnL)
    {
        u8_request[1] = 0x06;
        u8_request[3] = 0x11;
        u8_request[4] = *u8_requisition;
        u8_requisition += 1;
        u8_request[5] = *u8_requisition;
    }

        /*
        #ifdef CalibActive

        case GetCAPAC:
            u8_request[1] = 0x03;
            u8_request[3] = 0x66;
            u8_request[5] = 0x01;
            break;
        case SetCAPAC:
            u8_request[1] = 0x06;
            u8_request[3] = 0x66;
            u8_request[4] = *u8_requisition;
            u8_requisition += 1;
            u8_request[5] = *u8_requisition;
            break;
        case GetDecim:
            u8_request[1] = 0x03;
            u8_request[3] = 0x64;
            u8_request[5] = 0x01;
            break;
        case SetDecim:
            u8_request[1] = 0x06;
            u8_request[3] = 0x64;
            u8_request[4] = *u8_requisition;
            u8_requisition += 1;
            u8_request[5] = *u8_requisition;
            break;
        case GetLowDiv:
            u8_request[1] = 0x03;
            u8_request[3] = 0x65;
            u8_request[5] = 0x01;
            break;
        case SetLowDiv:
            u8_request[1] = 0x06;
            u8_request[3] = 0x65;
            u8_request[4] = *u8_requisition;
            u8_requisition += 1;
            u8_request[5] = *u8_requisition;
            break;
        case GetPECAL:
            u8_request[1] = 0x03;
            u8_request[3] = 0x67;
            u8_request[5] = 0x01;
            break;
        case SetPECAL:
            u8_request[1] = 0x06;
            u8_request[3] = 0x67;
            u8_request[4] = *u8_requisition;
            u8_requisition += 1;
            u8_request[5] = *u8_requisition;
            break;
        case GetSPESO:
            u8_request[1] = 0x03;
            u8_request[3] = 0x68;
            u8_request[5] = 0x01;
            break;
        case SetSPESO:
            u8_request[1] = 0x06;
            u8_request[3] = 0x68;
            u8_request[4] = *u8_requisition;
            u8_requisition += 1;
            u8_request[5] = *u8_requisition;
            break;
        case GetCPESO:
            u8_request[1] = 0x03;
            u8_request[3] = 0x69;
            u8_request[5] = 0x01;
            break;
        case SetCPESO:
            u8_request[1] = 0x06;
            u8_request[3] = 0x69;
            u8_request[4] = *u8_requisition;
            u8_requisition += 1;
            u8_request[5] = *u8_requisition;
            break;

        #endif
        */

    xCalcCRC16.pointerData = u8_request;
    xCalcCRC16.lenghtData = SizeRequestCalc;

    vCalcCRC16(&xCalcCRC16);

    u8_request[6] = xCalcCRC16.lowCRC16;
    u8_request[7] = xCalcCRC16.highCRC16;

    for (u8_aux00 = 0; u8_aux00 < SizeRequest; u8_aux00++)
    {
        Serial.write(u8_request[u8_aux00]);
    }
    
    //Serial.write(u8_request, SizeRequest);
}


void vRequestNetIC(uint16_t u16position , uint16_t u16_qtd, uint8_t u8_num_address )
{
    DataCRC xCalcCRC16;

    memset(bufferNetIC, 0x00, 100);
    
    bufferNetIC[0] = u8_num_address;
    bufferNetIC[1] = 0x03;
    bufferNetIC[2] = (u16position >> 8);
    bufferNetIC[3] = u16position;
    bufferNetIC[4] = (u16_qtd >> 8);
    bufferNetIC[5] = u16_qtd;

    xCalcCRC16.pointerData = bufferNetIC;
    xCalcCRC16.lenghtData = 6;
    vCalcCRC16(&xCalcCRC16);

    bufferNetIC[6] = xCalcCRC16.lowCRC16;
    bufferNetIC[7] = xCalcCRC16.highCRC16;

    netIC_Serial.write(bufferNetIC, 8);
    //netIC_Serial.write(bufferNetIC, 8);

    memset(bufferNetIC, 0x00, 100);

}













/*End Functions---------------------------------------------------------------------------*/









#endif
