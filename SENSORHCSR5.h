#ifndef SENSORHCSR5_H
#define SENSORHCSR5_H

class SENSORHCSR5 {

public:
    SENSORHCSR5(volatile unsigned char *_dirSensor,
                volatile unsigned char *_dirOUT,
                volatile unsigned char *_dirIFG,
                volatile unsigned char *_dirIES,
                volatile unsigned char *_dirIE,
                volatile int _portTrigger,
                volatile int _portEcho,
                volatile unsigned int *_TCCTL,
                volatile unsigned int *_TCCR,
                volatile unsigned int *_TCTL,
                volatile unsigned int _TCLR);

    void configureSensor();     // CONFIGURACION DEL SENSOR ULTRASONICO //
    int pulse();                // GENERAR PULSO y CALCULO DE DISTANCIA //
    //int getDistance();        //          OBTENER DISTANCIA           //
    bool presence();
    void PORT2_ISR_();
    void TIMER0_ISR_();

private:
    volatile unsigned char *dirSensor;
    volatile unsigned char *dirOUT;
    volatile unsigned char *dirIFG;
    volatile unsigned char *dirIES;
    volatile unsigned char *dirIE;
    volatile int portTrigger;
    volatile int portEcho;
    volatile unsigned int *TCCTL;
    volatile unsigned int *TCCR;
    volatile unsigned int *TCTL;
    volatile unsigned int TCLR;

    int miliseconds;
    long sensor;
    int distance;

};

#endif // SENSORHCSR5_H
