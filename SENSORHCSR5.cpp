#include <SENSORHCSR5.h>
#include <msp430.h>

SENSORHCSR5::SENSORHCSR5(volatile unsigned char *_dirSensor,
                         volatile unsigned char *_dirOUT,
                         volatile unsigned char *_dirIFG,
                         volatile unsigned char *_dirIES,
                         volatile unsigned char *_dirIE,
                         volatile int _portTrigger,
                         volatile int _portEcho,
                         volatile unsigned int *_TCCTL,
                         volatile unsigned int *_TCCR,
                         volatile unsigned int *_TCTL,
                         volatile unsigned int _TCLR) : dirSensor(_dirSensor), dirOUT(_dirOUT),
                         dirIFG(_dirIFG), dirIES(_dirIES), dirIE(_dirIE), portTrigger(_portTrigger),
                         portEcho(_portEcho), TCCTL(_TCCTL), TCCR(_TCCR), TCTL(_TCTL), TCLR(_TCLR),
                         miliseconds(0), sensor(0), distance(0)
{

//    *dirSensor |= portEcho;
//    *dirSensor &= ~portEcho;
//    *dirIFG  = 0x00;

    *TCCTL = CCIE;
    *TCCR = 1000;
    *TCTL = TASSEL_2 + MC_1;
}

void SENSORHCSR5::configureSensor() {
    *dirSensor |= portTrigger;           // TRIGGER //
    *dirSensor &= ~portEcho;             // ECHO //
    *dirIFG = 0x00;
}

int SENSORHCSR5::pulse() {
    *dirIES &= ~portEcho;
    *dirOUT |= portTrigger;
    __delay_cycles(10);
    *dirOUT &= ~portTrigger;
    *dirIFG = 0x00;
    *dirIE |= portEcho;
    *dirIES &= ~portEcho;

    __delay_cycles(30000);

    distance = (sensor * 0.01715);

    return distance;
}


//int SENSORHCSR5::getDistance() {
//    return distance;
//}


bool SENSORHCSR5::presence() {

    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    bool no_presence = false;
    int rule = pulse();

    if (rule > 100 && rule != 0){

        for(volatile unsigned int i = 0; i <= 20; i++) {

            P1OUT |= BIT0;

            rule = pulse();
            if (rule < 100 && rule != 0){
                no_presence = false;
                break;
            }

            else
                no_presence = true;

            __delay_cycles(500000);
        }
    }
    P1OUT &= ~BIT0;
    return no_presence;
}


void SENSORHCSR5::PORT2_ISR_() {
    if (!(*dirIES & portEcho)) {
        *TCTL |= TCLR;
        miliseconds = 0;
        *dirIES |= portEcho;
    } else {
        sensor = (long)miliseconds * 1000 + (long)TA0R;
    }
    *dirIFG &= ~portEcho;
}


void SENSORHCSR5::TIMER0_ISR_() {
    miliseconds++;
}
