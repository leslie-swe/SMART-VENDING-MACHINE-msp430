/*
 * stepper.h
 *
 *  Created on: 16 nov 2024
 *      Author: luismax
 */

#ifndef STEPPER_H_
#define STEPPER_H_

class stepper
{
public:
    stepper(volatile unsigned char *portdir_2, volatile unsigned char *portout_2,
            unsigned int bit_2,
            unsigned int bit_3,
            unsigned int bit_4,
            unsigned int bit_5);
    void stepperMove();

private:
    int bit1, bit2, bit3, bit4;  // Pines de control del motor
    volatile unsigned char *portdir1;  // Punteros a los registros de dirección
    volatile unsigned char *portout1;  // Punteros a los registros de salida
};

#endif /* LEDS_H_ */

