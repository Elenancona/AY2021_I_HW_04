/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "InterruptRoutines.h"
#include "project.h"

uint8 SendBytesFlag=0;
uint8 ch_recived;

CY_ISR(Custom_isr_ADC)
{
   // riporto lo status register del timer ad un livello basso 
    
    Timer_ADC_ReadStatusRegister();
    if (SendBytesFlag==1)
    {
        
    }
    
}

CY_ISR_PROTO(Custom_isr_UART)
{
    ch_recived =UART_GetChar(); //salvo in una variabile il carattere ricevuto
    
    switch (ch_recived)
    {
    
        case'B':
        case'b':
            SendBytesFlag = 1;
            OnBoard_LED_Write(1); //accendo il LED del PsoC se ricevo il comando 'b' o 'B'
            Timer_ADC_Start();
        break;
        
        case'S':
        case's':
            SendBytesFlag=0;
            OnBoard_LED_Write(0); //spengo il LED del PsoC quando ricevo il byte 's' o 'S'
            Timer_ADC_Stop();
            PWM_LED_WriteCompare(0); // spengo il led quando il sistema è spento
        break;
    default:
        break;
        }
}
