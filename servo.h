/*
 * servo.h
 *
 *  Created on: 14 oct 2024
 *      Author: luismax
 */

#ifndef SERVO_H_
#define SERVO_H_

class servo{
public:
    servo(volatile unsigned char *portdir_, volatile unsigned char *portout_, unsigned int pin_);
    void servoDegree(unsigned int degree_);
    void servoUp(unsigned int numpasosDown);//servooff
    void servoDown(unsigned int numpasosUp);//servodown

private:
    unsigned int pin;
    volatile unsigned char *portdir;
    volatile unsigned char *portout;

};

#endif /* LEDS_H_ */
