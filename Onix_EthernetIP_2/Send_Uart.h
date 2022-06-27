/* 
 * File: Send_Uart
 * Author: Eng. Tarcisio Souza de Melo
 * Comments:
 * 
 */

#ifndef __SEND_UART_H_INCLUDED
#define __SEND_UART_H_INCLUDED

/*Includes--------------------------------------------------------------------------------*/
#include <Arduino.h>
#include "main.h"
#include <SoftwareSerial.h>
#include "CRC16.h"
#include <string.h>
#include <math.h>
/*End Includes----------------------------------------------------------------------------*/



/*Defines---------------------------------------------------------------------------------*/
#define ADDRESS_NET_IC      0x01

#define ADDRANSWERNET_IC    0x07D0 

#define NUMBER_REQUISITIONS 0x0E

#define RS485               5

/*End Defines-----------------------------------------------------------------------------*/


/* Private typedef------------------------------------------------------------------------*/
typedef struct{
    uint8_t *u8_bufferMsg;
    uint8_t u8_sizeBuffer;
}SendMsg;
/* Private variables----------------------------------------------------------------------*/



/*Global Variables------------------------------------------------------------------------*/
extern SoftwareSerial netIC_Serial;

extern uint16_t u16_RegisterModbus[16];
extern uint8_t bufferRxMatrix[100];
extern uint8_t bufferNetIC[100];

extern uint16_t u16_RegisterModbus[16];
/*End Global Variables--------------------------------------------------------------------*/


/*Prototypes Functions--------------------------------------------------------------------*/
void vSMatrix(SendMsg msg);

/*End Prototype Functions-----------------------------------------------------------------*/



/*Functions-------------------------------------------------------------------------------*/
void vSMatrix(SendMsg msg)
{
    uint8_t u8_aux00;
    uint8_t u8_aux01;
    
    for(u8_aux01 = 0; u8_aux01 < msg.u8_sizeBuffer; u8_aux01++)
    {
        u8_aux00 = msg.u8_bufferMsg[u8_aux01];
        //msg.u8_bufferMsg++;
        Serial.write(u8_aux00);
    }
}


void vSend_netIC(SendMsg msg)
{
    uint8_t u8_aux00;
    uint8_t u8_aux01;
    
    for(u8_aux01 = 0; u8_aux01 < msg.u8_sizeBuffer; u8_aux01++)
    {
        u8_aux00 = msg.u8_bufferMsg[u8_aux01];
        //msg.u8_bufferMsg++;
        netIC_Serial.write(u8_aux00);
    }  
}

void vSend_Modbus_netIC(uint8_t u8_addrstart, uint u8_qtd, uint16_t u16_posWrite)
{
    uint8_t u8_aux00;
    SendMsg xmsg;
    DataCRC xCalcCRC16;

    memset(bufferRxMatrix, 0x00, 100);

    bufferRxMatrix[0] = ADDRESS_NET_IC; 
    bufferRxMatrix[1] = 0x0A;
    bufferRxMatrix[2] = (u16_posWrite >> 8);
    bufferRxMatrix[3] = u16_posWrite;
    bufferRxMatrix[4] = u8_qtd * 2;
    u8_aux00 = 0x05;

    for (uint8_t i = 0; i < u8_qtd; i++)
    {
        bufferRxMatrix[u8_aux00] = (u16_RegisterModbus[u8_addrstart] >> 8);
        //bufferRxMatrix[u8_aux00] = 0xAA;
        u8_aux00++;
        bufferRxMatrix[u8_aux00] = (u16_RegisterModbus[u8_addrstart]);
        //bufferRxMatrix[u8_aux00] = 0xBB;
        u8_addrstart++;
        u8_aux00++;
    }
    xCalcCRC16.pointerData = bufferRxMatrix;
    xCalcCRC16.lenghtData = u8_aux00;
    vCalcCRC16(&xCalcCRC16);
    
    bufferRxMatrix[u8_aux00] = xCalcCRC16.lowCRC16;
    u8_aux00 += 1;
    bufferRxMatrix[u8_aux00] = xCalcCRC16.highCRC16;
    xmsg.u8_bufferMsg = bufferRxMatrix;
    u8_aux00 += 1;
    xmsg.u8_sizeBuffer = u8_aux00;
    vSend_netIC(xmsg);
    memset(bufferRxMatrix, 0x00, 100);
    
}

void vSendAnswerNetIC(uint8_t *u8_requisitions)
{
    uint8_t u8_aux00 = 0x00;
    uint8_t u8_aux01 = 0x00;
    uint8_t u8_pos_mb[2] = {0xFF, 0xFF};

    SendMsg xmsg;
    DataCRC xCalcCRC16;

    memset(bufferNetIC, 0x00, 100);

    for(u8_aux00 = 0x00; u8_aux00 <= NUMBER_REQUISITIONS; u8_aux00++)
    {
        u8_aux01 = *u8_requisitions;
        if (u8_aux01 == 0x01 && u8_pos_mb[0] == 0xFF)
        {
            u8_pos_mb[0] = u8_aux00;
        }
        if (u8_aux01 == 0x01 && u8_pos_mb[0] != 0xFF)
        {
            u8_pos_mb[1] = u8_aux00;
            break;
        }
        u8_requisitions++;
    }

    bufferNetIC[0] = ADDRESS_NET_IC;
    bufferNetIC[1] = 0x10;
    bufferNetIC[2] = (ADDRANSWERNET_IC >> 8);
    bufferNetIC[3] = ADDRANSWERNET_IC;
    bufferNetIC[4] = 0x00; 
    bufferNetIC[5] = abs(u8_pos_mb[1] - u8_pos_mb[0]) + 1;
    bufferNetIC[6] = 2 * bufferNetIC[5]; 

    u8_aux01 = 0x07;
    for (u8_aux00 = u8_pos_mb[0]; u8_aux00 <= u8_pos_mb[1]; u8_aux00++)
    {
        bufferNetIC[u8_aux01] = (u16_RegisterModbus[u8_aux00] >> 8);
        u8_aux01++;
        bufferNetIC[u8_aux01] = (u16_RegisterModbus[u8_aux00]);
        u8_aux01++;
    }
    
    xCalcCRC16.pointerData = bufferNetIC;
    xCalcCRC16.lenghtData = u8_aux01;
    vCalcCRC16(&xCalcCRC16);
    u8_aux01++; 
    bufferNetIC[u8_aux01] = xCalcCRC16.lowCRC16;
    u8_aux01++; 
    bufferNetIC[u8_aux01] = xCalcCRC16.highCRC16;
    u8_aux01++; 
    xmsg.u8_bufferMsg = bufferNetIC;
    xmsg.u8_sizeBuffer = u8_aux01;
    
    digitalWrite(RS485, 0);
    vSend_netIC(xmsg);
    digitalWrite(RS485, 1);
}

#endif
