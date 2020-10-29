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

    //start ADC
    ADC_DelSig_Start();
    
    //start UART
    UART_Start();
    
    //start PWM
    PWM_LED_Start();
    
    //start AMux
    AMux_Start();
    
    //start irs ADC and irs UART
    isr_ADC_StartEx(Custom_isr_ADC);
    isr_RX_StartEx(Custom_isr_UART);
    
    
    //inizialize and end the buffer 
    DataBuffer[0] = 0xA0;
    DataBuffer[TRANSMIT_BUFFER_SIZE-1] = 0xC0; 
    
    PacketReadyFlag = 0;
    
    //start conversion
    ADC_DelSig_StartConvert(); 
  
    for(;;)
    {
        if (PacketReadyFlag==1) 
        {   
            //send data
            UART_PutArray(DataBuffer, TRANSMIT_BUFFER_SIZE);
            PacketReadyFlag=0;
        }
    }
}

/* [] END OF FILE */
