/*Includes--------------------------------------------------------------------------------*/
#include <Arduino.h>
#include "main.h"
#include <string.h>
#include <SoftwareSerial.h>
#include <Ticker.h>

#include "requisicao.h"
#include "handle_bits.h"
#include "Send_Uart.h"
#include "CRC16.h"
#include "error.h"
#include "ISR_TICKER.h"
/*End Includes----------------------------------------------------------------------------*/
/*COM Indicador Matrix*/

/*Defines---------------------------------------------------------------------------------*/

/*End Defines-----------------------------------------------------------------------------*/

/*Global Variables------------------------------------------------------------------------*/

extern Ticker xTimeout_MatrixCom;
extern Ticker xledLife;

extern DataCRC xCalcCRC16;

extern uint32_t u32_stateMachine;

extern uint8_t u8_address;

extern uint8_t u8_errorCOM;

extern uint8_t bufferRxMatrix[100];

extern uint8_t bufferNetIC[100];

extern uint8_t u8_reqTEST[2];

extern uint8_t u8_reqInfo1;

extern uint8_t u8_reqInfo2[11];

extern uint8_t u8_SetMatrix[14];

extern SendMsg xMsg;

extern uint8_t u8_req_netic;

extern uint8_t u8_lock;
extern uint8_t u8_lock_req;
extern uint8_t u8_enable_netic;

extern uint8_t u8_answer_netic[15];

/*End Global Variables--------------------------------------------------------------------*/

/*Prototypes Functions--------------------------------------------------------------------*/

void vRequest(uint8_t u8_requisition, uint8_t bitConfig, uint8_t *u8_dataRequisition);

/*End Prototype Functions-----------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------*/
void setup()
{
    Serial.begin(BAUDRATE_MATRIX, SERIAL_8N2);
    netIC_Serial.begin(BAUDRATE, SWSERIAL_8N1, matrixRx, matrixTx, false, 100, 100);

    pinMode(LED, OUTPUT);
    pinMode(TOGGLE_RS485, OUTPUT);
    xledLife.attach_ms(LEDLIFE, ISR_LifeLED);
    delay(1000);

    digitalWrite(TOGGLE_RS485, 1);
    u32_stateMachine = 1;
    u8_errorCOM = 0;
}

void loop()
{
    uint16_t u16_aux00 = 0x0000;
    uint8_t u8_aux01 = 0x00;
    uint8_t u8_aux02 = 0x00;
    uint8_t u8_dataReq[2];
    if (u32_stateMachine == 1 && u8_lock == 0x00)
    {
        vRequest(GetWeight, 0x01, NULL); // Weight
        u8_lock = 0x01;
    }
    else if (u32_stateMachine == 13 && u8_lock == 0)
    {
        vRequest(0x00, 0x00, NULL);
        u8_lock = 1;
    }
    
    if (u32_stateMachine == 1 && u8_lock == 0x02)
    {
        u32_stateMachine = 13;
        u8_lock = 0;
        u8_req_netic = 0;
    }
    else if (u32_stateMachine == 13 && u8_lock == 0x03)
    {
        u8_req_netic = 1;
        u8_enable_netic = 1;
    }






    /*Efetuando processos requisitados via NetIC*/
    if (u8_req_netic == 0x01)
    {
        /*Req Act tara?*/
        if (bufferNetIC[16] == 0x01)
        {
            while (1)
            {
                /*Requisitou Act Tara*/
                if (u8_req_netic == 0x01 && u8_lock_req == 0x00)
                {
                    u32_stateMachine = 7;
                    vRequest(SetTara, 0x01, NULL);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                /*Requisitou Peso*/
                else if (u8_req_netic == 0x02 && u8_lock_req == 0x00)
                {
                    u32_stateMachine = 1;
                    vRequest(GetWeight, 0x01, NULL);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                /*Verificação de ação de tara com sucesso com peso novo == 0*/
                else if (u8_req_netic == 0x03 && u8_lock_req == 0x00)
                {
                    u16_aux00 = u16_RegisterModbus[0];
                    u16_aux00 &= 0x00FF;

                    if (u16_aux00 == 0x00000)
                    {
                        u16_aux00 = u16_RegisterModbus[1];
                        if (u16_aux00 == 0x00000)
                        {
                            /*Tara com sucesso*/
                            vSetRegisterModbus(0x08, 0x00, 0x01);
                        }
                        else
                        {
                            /*Tara sem sucesso*/
                            vSetRegisterModbus(0x08, 0x00, 0x02);
                        }
                        u8_answer_netic[8] = 0x01;
                    }
                    else
                    {
                        /*Tara sem sucesso*/
                        vSetRegisterModbus(0x08, 0x00, 0x02);
                    }
                    break;
                }
            } 
        }

        /*Req Act Zero or Destara?*/
        if (bufferNetIC[18] == 0x01)
        {
            
            while (1)
            {
                /*Requisitou Act Tara*/
                if(u8_lock_req == 0x00)
                {
                    u32_stateMachine = 8;
                    vRequest(SetZero, 0x01, NULL);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_answer_netic[9] = 0x01;
                    u8_lock_req = 0x00;
                    break;
                }
            }
            
                    
        }
        
        /*Req Get Peso?*/
        if (bufferNetIC[4] == 0x01)
        {
            /* processo de leitura de peso é efetuado all time, desta forma não se faz necessário ser efetuado o processo mais uma vez, visto que muito provavelmente o equipamento não teve tempo hábil de alterar valores. Este espaço foi deixado aqui para caso haja a necessidade de mudanças/ e implementações futuras 
            while (1)
            {
            }
            */
            u8_answer_netic[0] = 0x01;
            u8_answer_netic[1] = 0x01;
        
        }

        /*Req Get Tara?*/
        if (bufferNetIC[6] == 0x01)
        {
            while (1)
            {
                if(u8_lock_req == 0x00)
                {
                    u32_stateMachine = 2;
                    vRequest(GetTara, 0x01, NULL);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_answer_netic[2] = 0x01;
                    u8_answer_netic[3] = 0x01;
                    u8_lock_req = 0x00;
                    break;
                }
            }
        
        }

        /*Req Act Filtro?*/
        if (bufferNetIC[24] != 0x00)
        {
            while (1)
            {
                if(u8_lock_req == 0x00)
                {
                    u32_stateMachine = 11;
                    memcpy(&bufferNetIC[23], u8_dataReq, 2);
                    vRequest(SetFilters, 0x02, u8_dataReq);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_answer_netic[12] = 0x01;
                    u8_lock_req = 0x00;
                    break;
                }
            }
        }

        /*Req Act F_Sensibilidade?*/
        if (bufferNetIC[26] != 0x00)
        {
            while(1)
            {
                if(u8_lock_req == 0x00)
                {
                    u32_stateMachine = 12;
                    memcpy(&bufferNetIC[25], u8_dataReq, 2);
                    vRequest(SetFSensib, 0x02, u8_dataReq);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_answer_netic[13] = 0x01;
                    u8_lock_req = 0x00;
                    break;
                }
            }
        }

        /*Req Act PrAP?*/
        if (bufferNetIC[22] != 0x00)
        {
            while(1)
            {    
                if(u8_lock_req == 0x00)
                {
                    u32_stateMachine = 10;
                    memcpy(&bufferNetIC[21], u8_dataReq, 2);
                    vRequest(SetPrAP, 0x02, u8_dataReq);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_answer_netic[11] = 0x01;
                    u8_lock_req = 0x00;
                    break;
                }
            }
        }

        /*Req Act Impr?*/
        if (bufferNetIC[20] != 0x00)
        {
            while(1)
            {
                if(u8_lock_req == 0x00)
                {
                    u32_stateMachine = 9;
                    vRequest(SetImpr, 0x01, NULL);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_answer_netic[10] = 0x01;
                    u8_lock_req = 0x00;
                    break;
                }
            }
        }

        /*Req Act UnL?*/
        if (bufferNetIC[28] != 0x00)
        {
            while(1)
            {
                if(u8_lock_req == 0x00)
                {
                    u32_stateMachine = 13;
                    memcpy(&bufferNetIC[27], u8_dataReq, 2);
                    vRequest(SetUnL, 0x02, u8_dataReq);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_answer_netic[14] = 0x01;
                    u8_lock_req = 0x00;
                    break;
                }
            }
  
        }

        /*Get Filtro?*/
        if (bufferNetIC[8] != 0x00)
        {
            while(1)
            {
                if(u8_lock_req == 0x00)
                {
                    u32_stateMachine = 3;
                    vRequest(GetFilters, 0x01, NULL);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_answer_netic[4] = 0x01;
                    u8_lock_req = 0x00;
                    break;
                }
            }
        }

        /*Get F_Sensib?*/
        if (bufferNetIC[10] != 0x00)
        {
            while(1)
            {
                if(u8_lock_req == 0x00)
                {
                    u32_stateMachine = 4;
                    vRequest(GetFSensib, 0x01, NULL);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_answer_netic[5] = 0x01;
                    u8_lock_req = 0x00;
                    break;
                }
            }
        }


        /*Get PrAP?*/
        if (bufferNetIC[12] != 0x00)
        {
            while(1)
            {    
                if(u8_lock_req == 0x00)
                {
                    u32_stateMachine = 5;
                    vRequest(GetPrAP, 0x01, NULL);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_answer_netic[6] = 0x01;
                    u8_lock_req = 0x00;
                    break;
                }
            }
        }


        /*Get UnL?*/
        if (bufferNetIC[14] != 0x00)
        {
            while(1)
            {
                if(u8_lock_req == 0x00)
                {
                    u32_stateMachine = 6;
                    vRequest(GetUnL, 0x01, NULL);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_answer_netic[7] = 0x01;
                    u8_lock_req = 0x00;
                    break;
                }
            }
        }

        for (u8_aux01 = 0; u8_aux01 < 15; u8_aux01++)
        {
            if(u8_answer_netic[u8_aux01] == 0x01)
            {
                u8_aux02 = 0x01;
                break;
            }
        }
        
        /*send info netIC*/
        if (u8_aux02 != 0x00)
        {
            while(1)
            {
                if(u8_lock_req == 0x00)
                {
                    //xMsg.u8_bufferMsg = bufferNetIC;
                    //xMsg.u8_sizeBuffer = 29;
                    //vSend_netIC(xMsg);
                    u32_stateMachine = 14;
                    vRequest(0x00, 0x03, u8_answer_netic);
                    u8_lock_req = 0xFF; //não ficar entrando toda hora
                }
                if(u8_lock_req == 0xEE)
                {
                    u8_lock_req = 0x00;
                    break;
                }
            }
        }
        u8_req_netic = 0x00;
        u8_enable_netic = 0x00;
        memset(bufferNetIC, 0x00, 100);
        memset(u8_answer_netic, 0x00, 15);
        u32_stateMachine = 1; 
        u8_lock = 0x00; 
        u8_aux02 = 0x00; 
    }     
}

/*FUNCTIONS--------------------------------------------------------------------*/

/*ISR==========================================================================*/

/*=============================================================================*/

void vRequest(uint8_t u8_requisition, uint8_t opt, uint8_t *u8_dataRequisition)
{
    uint8_t time_out = 0;
    memset(bufferRxMatrix, 0x00, 100);

    if (opt == 0x00)
    {
        vRequestNetIC(STARTADDNETIC, QTDREGISTER, ADDNETIC);
        time_out = TIMEOUT_COM_NETIC;
    }
    else if (opt == 0x01)
    {
        vRequesition(u8_address, u8_requisition, NULL);
        time_out = TIMEOUT_COM;
    }
    else if(opt == 0x02)
    {
        vRequesition(u8_address, u8_requisition, u8_dataRequisition);
        time_out = TIMEOUT_COM;
    }
    else if (opt == 0x03)
    {
        vSendAnswerNetIC(u8_dataRequisition);
        time_out = TIMEOUT_COM_NETIC;
    }
    xTimeout_MatrixCom.once_ms(time_out, ISR_TIM_COM_MATRIX1);
}

/*END FUNCTIONS----------------------------------------------------------------*/
