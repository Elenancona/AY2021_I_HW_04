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
#ifndef __INTERRUPT_ROUTINES_H__
    #define __INTERRUPT_ROUTINES_H__
    
    #include "cytypes.h"
    #include "stdio.h"
    
    #define BYTE_TO_SEND 4 //Ho due byte per il potenziometro e due per il fotoresistore
    #define TRANSMIT_BUFFER_SIZE 1+BYTE_TO_SEND+1 //dimensione della stringa che mando all'UART, aggiungo un byte di tail e uno di header

    
    CY_ISR_PROTO(Custom_isr_ADC);
    CY_ISR_PROTO(Custom_isr_UART);
    
    uint8 DataBuffer [TRANSMIT_BUFFER_SIZE];
    volatile uint8 PacketReadyFlag;
    int32 value_digit;
    
#endif 