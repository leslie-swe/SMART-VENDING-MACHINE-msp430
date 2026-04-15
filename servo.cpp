/*
 * servo.h
 *
 *  Created on: 14 oct 2024
 *      Author: luismax
 */

#include "msp430.h"
#include <servo.h>

servo *servo_instance = nullptr;//Puntero global que apunta a la clase
servo::servo(volatile unsigned char *portdir_, volatile unsigned char *portout_, unsigned int pin_):
        portdir(portdir_), portout(portout_), pin(pin_)

{
    // TODO Auto-generated constructor stub
    servo_instance = this;
    *portdir |= pin;
    *portout |= pin;
}

void servo::servoDegree(unsigned int degree_)
{

             // Configuración del pin del servo
             P2DIR |= BIT0; // Establecer el pin como salida
             P2SEL |= BIT0;
             TA1CTL =TASSEL__ACLK+MC_3+ID_1;//modo de conteo, modo de division
             TA1CCTL1=OUTMOD_6;//modo de salida del pwm
             TA1CCR0= 339;
             int degree;
             int maxCycles=335;
             int minCycles=318;
             int maxDegree =180-1;
             int minDegree=1;
             int cycles;
             degree=degree_;
             cycles=((degree-minDegree)*(maxCycles-minCycles)/(maxDegree-minDegree)+minCycles);
             __delay_cycles(1000000);
             TA1CCR1=cycles;
}



