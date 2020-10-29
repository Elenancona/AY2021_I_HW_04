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
    //read Timer status register to bring interrupt line low
    Timer_ADC_ReadStatusRegister();
        
        
    if (SendBytesFlag==1)
    {
        //select the channel of the AMux 
        AMux_Select(PHOTO); 
        
        //set the value digit as the value read the ADC
        value_digit = ADC_DelSig_Read32(); 
    
        //value digit must be between 0 and 65535
        if(value_digit<0)       value_digit=0;
        if(value_digit>65535)   value_digit=65535;
        
        //put the MSB and LSB in DataBuffer
        DataBuffer[1] = value_digit >> 8;
        DataBuffer[2] = value_digit & 0xFF;
        
        
        //set output_foto_mv as the light intesity in the room
        output_foto_mv=ADC_DelSig_CountsTo_mVolts(value_digit); 
   
        
        //if the light intensity in the room is less than the threshold switch ON the LED
        if (output_foto_mv < THRESHOLD) 
        {
            //select AMux chanell
            AMux_Select(POT); 
            
            //read ADC
            value_digit = ADC_DelSig_Read32(); 
    
            //value digit must be between 0 and 65535
            if(value_digit<0)       value_digit=0;
            if(value_digit>65535)   value_digit=65535;
            
            //put the MSB and LSB in DataBuffer        
            DataBuffer[3] = value_digit >> 8;
            DataBuffer[4] = value_digit & 0xFF;
            
            //LED on with intensity given by the potentiometer
            PWM_LED_WriteCompare(value_digit); 
        }
        
        //if the light intensity of the room is grater than the threshold switch OFF the LED
        else PWM_LED_WriteCompare(0); 
        
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
