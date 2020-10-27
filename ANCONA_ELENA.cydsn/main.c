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
#include "project.h"
#include "InterruptRoutines.h"

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    ADC_DelSig_Start();
    UART_Start();
    PWM_LED_Start();
    AMux_Start();
    
    isr_ADC_StartEx(Custom_isr_ADC);
    isr_RX_StartEx(Custom_isr_UART);
    
    DataBuffer[0] = 0xA0;
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0; 
    PacketReadyFlag = 0;
    ADC_DelSig_StartConvert();
  
    for(;;)
    {
        if (PacketReadyFlag==1) 
        {   
            //the samples are sent through UART communication
            UART_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);
            PacketReadyFlag=0;
        }
    }
}

/* [] END OF FILE */
