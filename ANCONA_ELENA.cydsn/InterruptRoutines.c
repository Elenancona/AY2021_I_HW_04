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

#define PHOTO 0 //Il fotoresistore è nel canale di imput "0" dell'AMux 
#define POT 1  //Il potenziomentro è nel canale di imput "1" dell'AMux
#define THRESHOLD 2000

uint8 SendBytesFlag=0;
uint8 ch_recived;
int32 output_foto_mv;

CY_ISR(Custom_isr_ADC)
{
    // riporto lo status register del timer ad un livello basso 
    Timer_ADC_ReadStatusRegister();
        
        
    if (SendBytesFlag==1)
    {
        AMux_Select(PHOTO); 
        value_digit = ADC_DelSig_Read32(); //Lettura ADC
    
        if(value_digit<0)       value_digit=0;
        if(value_digit>65535)   value_digit=65535;
        
        DataBuffer[1] = value_digit >> 8;
        DataBuffer[2] = value_digit & 0xFF;
        
        output_foto_mv=ADC_DelSig_CountsTo_mVolts(value_digit); //output_foto_mv è l'intensità luminosa nella stanza 
   
        if (output_foto_mv < THRESHOLD) //se l'intensità nella stanza è minore della soglia allora accendo il LED
        {
            AMux_Select(POT); 
            value_digit = ADC_DelSig_Read32(); //Lettura ADC
    
            if(value_digit<0)       value_digit=0;
            if(value_digit>65535)   value_digit=65535;
        
            DataBuffer[3] = value_digit >> 8;
            DataBuffer[4] = value_digit & 0xFF;
            
            PWM_LED_WriteCompare(value_digit); //Accendo il LED con il valore dato dal potenziometro 
        }
        else PWM_LED_WriteCompare(0); //Se l'intensità luminosa nella stanza è abbastanza alta non accendo il LED
        
        PacketReadyFlag=1;
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
