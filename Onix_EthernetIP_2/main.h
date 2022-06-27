/* 
 * File: main
 * Author: Eng. Tarcisio Souza de Melo
 * Comments:
 * 
 */
#ifndef __MAIN_H_INCLUDED
#define __MAIN_H_INCLUDED

/*Includes--------------------------------------------------------------------------------*/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Ticker.h>

#include "requisicao.h"
#include "handle_bits.h"
#include "Send_Uart.h"
#include "CRC16.h"
#include "error.h"
#include "ISR_TICKER.h"
/*End Includes----------------------------------------------------------------------------*/

/*Defines---------------------------------------------------------------------------------*/
#define BAUDRATE_MATRIX     38400   
#define BAUDRATE            57600            
#define matrixTx            6
#define matrixRx            7
#define TOGGLE_RS485        5

#define REQUEST_MATRIX      0x00
#define AWAIT_REQUISITION   0x01

#define TIMEOUT_COM         50
#define TIMEOUT_COM_NETIC   30
#define LEDLIFE             250

#define LED                 2



/*End Defines-----------------------------------------------------------------------------*/


/* Private typedef------------------------------------------------------------------------*/

/* Private variables----------------------------------------------------------------------*/


/*Global Variables------------------------------------------------------------------------*/
SoftwareSerial netIC_Serial;

Ticker xTimeout_MatrixCom;
Ticker xledLife;

DataCRC xCalcCRC16;


uint32_t u32_stateMachine = 0x00;

uint8_t u8_address = 0x01;

uint8_t u8_lock = 0x00;
uint8_t u8_lock_req = 0x00;

uint8_t u8_errorCOM = 0x00;

uint8_t bufferRxMatrix[100];

uint8_t bufferNetIC[100];

uint8_t u8_reqTEST[2] = {0x00, 0x01};

/*Verifica se existiu requisição e armazena em bool neste array na ordem
0  -> GetPeso
1  -> GetPeso
2  -> GetPeso
3  -> GetPeso
4  -> GetFiltro
5  -> GetFSensib
6  -> GetPrAP
7  -> GetUnL
8  -> SetActTara
9  -> SetActZero
10 -> SetActImpr
11 -> SetActPrAP
12 -> SetActFiltro
13 -> SetActFSensib
14 -> SetActUnL
*/
uint8_t u8_answer_netic[15] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

uint8_t u8_reqInfo1     = GetWeight;


uint8_t u8_reqInfo2[11] = {GetTara,
                           GetFilters,
                           GetFSensib,
                           GetPrAP,
                           GetUnL,
                           GetCAPAC,
                           GetDecim,
                           GetLowDiv,
                           GetPECAL,
                           GetSPESO,
                           GetCPESO};
                        
uint8_t u8_SetMatrix[14] = {SetTara,
                            SetZero,
                            SetDestara,
                            SetImpr,
                            SetFilters,
                            SetFSensib,
                            SetPrAP,
                            SetUnL,
                            SetCAPAC,
                            SetDecim,
                            SetLowDiv,
                            SetPECAL,
                            SetSPESO,
                            SetCPESO};

uint16_t u16_RegisterModbus[16] = {0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000,
                                   0x0000};

SendMsg xMsg;

uint8_t u8_req_netic = 0;
uint8_t u8_enable_netic = 0x00;
/*End Global Variables--------------------------------------------------------------------*/


/*Prototypes Functions--------------------------------------------------------------------*/

/*End Prototype Functions-----------------------------------------------------------------*/

/*Functions-------------------------------------------------------------------------------*/
#endif
