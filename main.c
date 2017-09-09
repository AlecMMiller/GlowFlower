/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC16F1708
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
 */

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

#include "mcc_generated_files/mcc.h"
#include <stdlib.h>
#include <math.h>

#define TIME_TO_TARGET 1000
#define HEARTBEAT_TIME 100
#define PI 3.14

/*
                         Main application
 */
float Pwm_1_Desired = 0;
float Pwm_1_Current = 0;
float Pwm_1_Step = 0;
float Pwm_2_Desired = 0;
float Pwm_2_Current = 0;
float Pwm_2_Step = 0;
float Pwm_3_Desired = 0;
float Pwm_3_Current = 0;
float Pwm_3_Step = 0;

uint16_t timeSinceLastUpdate = 0;
uint16_t heartbeat = 0;

float Heartbeat(float SetVal){
    if(SetVal < 0){
        SetVal = 0;
    } else if (SetVal > 1023){
        SetVal = 1023;
    }
    
    SetVal -= SetVal * 0.66 * sin(heartbeat * PI / (HEARTBEAT_TIME));
    
    return SetVal;
}

void Generate_Desired(){
    timeSinceLastUpdate = 0;
    float rand1 = rand();
    float rand2 = rand();
    float rand3 = rand();
    float sum = rand1+rand2+rand3;
    if(sum < 1){
        sum = 1;
    }
    
    Pwm_1_Desired = (rand1 / sum) * 1023;
    Pwm_2_Desired = (rand2 / sum) * 1023;
    Pwm_3_Desired = (rand3 / sum) * 1023;
    
    Pwm_1_Step = (Pwm_1_Desired - Pwm_1_Current) / TIME_TO_TARGET;
    Pwm_2_Step = (Pwm_2_Desired - Pwm_2_Current) / TIME_TO_TARGET;
    Pwm_3_Step = (Pwm_3_Desired - Pwm_3_Current) / TIME_TO_TARGET;
}

void Step() {
    Pwm_1_Current += Pwm_1_Step;
    Pwm_2_Current += Pwm_2_Step;
    Pwm_3_Current += Pwm_3_Step;
    
    PWM1_LoadDutyValue(Heartbeat(Pwm_1_Current));
    PWM2_LoadDutyValue(Heartbeat(Pwm_2_Current));
    PWM3_LoadDutyValue(Heartbeat(Pwm_3_Current));
    
    __delay_ms(10);
    
    timeSinceLastUpdate++;
    
    if(timeSinceLastUpdate > (TIME_TO_TARGET * 1.1)){
        Generate_Desired();
    }
    
    heartbeat++;
    
    if(heartbeat > HEARTBEAT_TIME){
        heartbeat = 0;
    }
}

void Go_To_Desired() {
    if (Pwm_1_Step > 0) {
        while (Pwm_1_Current < Pwm_1_Desired) {
            Step();
        }
    } else if (Pwm_1_Step < 0) {
        while (Pwm_1_Current > Pwm_1_Desired) {
            Step();
        }
    }
}

void main(void) {
    // initialize the device
    SYSTEM_Initialize();

    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    //INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    srand(123);

    while (1) {
        Generate_Desired();
        Go_To_Desired();
    }
}
/**
 End of File
 */