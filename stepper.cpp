/*
 * stepper.cpp
 *
 *  Created on: 16 nov 2024
 *      Author: luismax
 */
#include "msp430.h"
#include <stepper.h>

stepper *stepper_instance = nullptr;//Puntero global que apunta a la clase
stepper::stepper(volatile unsigned char *portdir_2, volatile unsigned char *portout_2,
                 unsigned int bit_2, unsigned int bit_3, unsigned int bit_4,
                 unsigned int bit_5) :
                 portdir1(portdir_2), portout1(portout_2), bit1(bit_2), bit2(bit_3), bit3(bit_4), bit4(bit_5)

{
    // TODO Auto-generated constructor stub
    stepper_instance = this;
    *portdir1 |= bit1;
    *portdir1 |= bit2;
    *portdir1 |= bit3;
    *portdir1 |= bit4;


    // Inicializar los pines en bajo (apagados)
    *portout1 &= ~bit1; // Apaga pin1
    *portout1 &= ~bit2; // Apaga pin2
    *portout1 &= ~bit3; // Apaga pin3
    *portout1 &= ~bit4; // Apaga pin4

}

void stepper::stepperMove()
{

    WDTCTL = WDTPW | WDTHOLD;    // Detener el watchdog timer
    // Configurar los pines como salidas
    P6DIR |= BIT5;  // IN4
    P6DIR |= BIT4;  // IN3
    P6DIR |= BIT3;  // IN2
    P6DIR |= BIT2;  // IN1

    TA0CCR0 = 1000;  // Frecuencia del PWM (modificar según se necesite)
    TA0CCTL1 = OUTMOD_7;  // Salida en modo "reset/set" (PWM)
    TA0CCR1 = 500;  // Ciclo de trabajo del 50% (modificar según se necesite)
    TA0CTL = TASSEL_2 + MC_1;  // SMCLK, modo de cuenta hacia arriba (UP mode)

    // Bucle infinito para generar la secuencia de pasos
    volatile int i;
    for (i = 0; i < 1000; i++) {
        // Paso 1
        P6OUT = (P6OUT & ~(BIT5 + BIT4 + BIT3 + BIT2)) | BIT2;  // IN1 = 1, resto = 0
        __delay_cycles(3000);  // Esperar un poco para dar tiempo al motor

        // Paso 2
        P6OUT = (P6OUT & ~(BIT5 + BIT4 + BIT3 + BIT2)) | BIT3;  // IN2 = 1, resto = 0
        __delay_cycles(3000);

        // Paso 3
        P6OUT = (P6OUT & ~(BIT5 + BIT4 + BIT3 + BIT2)) | BIT4;  // IN3 = 1, resto = 0
        __delay_cycles(3000);

        // Paso 4
        P6OUT = (P6OUT & ~(BIT5 + BIT4 + BIT3 + BIT2)) | BIT5;  // IN4 = 1, resto = 0
        __delay_cycles(3000);
    }
}
