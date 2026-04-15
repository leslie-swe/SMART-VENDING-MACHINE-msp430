#include <msp430.h>
#include <stdio.h>
#include <string.h>

#include <SENSORHCSR5.h>
#include <TECLADO.h>

#include <servo.h>
#include <stepper.h>


// DECLARACION DE FUNCIONES //
void sendData(char *str);
void confirmarCompra();
void manejarBoton(int boton);
void recibirMontoDesdeTerminal(int valor);
bool presence();
void verificarCambio();


// VARIABLES GLOBALES //
char msg[] = "\033[2J\033[H";  // LIMPIAR PANTALLA //

int rule = 0;
int rule_ = 0;

char precio[4];

int productoSeleccionado = -1;

int estado = 0;

/*
    0:  SELECCION DE PRODUCTOS.
    1:  INGRESO DE MONEDAS.
    2:  CONFIRMACION.
    10: MODO ADMINISTRADOR.
    11: MODO INGRESO DE MONEDAS
*/

char *productoSeleccionado_[4] = {"AGUA EMBOTELLADA", "CHOCOLATES", "PANQUESITOS", "PAPAS FRITAS"};
int precioProducto[4] = {10, 15, 20, 23};
int stockProduct[4] = {10, 10, 10, 10};
int productoVendido[4] = {0, 0, 0, 0};

// CANTIDAD DE MONEDAS EN MAQUINA //
int mon1 = 10;
int mon2 = 10;
int mon5 = 10;

int pre_mon1 = mon1;
int pre_mon2 = mon2;
int pre_mon5 = mon5;

char mon1_[10];
char mon2_[10];
char mon5_[10];

char pre_mon1_[10];
char pre_mon2_[10];
char pre_mon5_[10];

int cam_mon5 = 0;
int cam_mon2 = 0;
int cam_mon1 = 0;

int montoIngresado = 0;
char montoIngresado_[10];

int totalRestante = 0;

int ganancia = 0;
char ganancia_[0];

// ACTIVADORES //
bool state = false;
bool comeBack = false;
bool adBreak = false;


// CREACION DE LA CLASE //
/* 1 */ SENSORHCSR5 sensor(&P2DIR, &P2OUT, &P2IFG, &P2IES, &P2IE, BIT4, BIT5, &TA0CCTL0, &TA0CCR0, &TA0CTL, TACLR);     // SENSOR ULTRASONICO //
/* 2 */ servo myservo(&P2DIR,&P2SEL,BIT0);                                                                            // SERVOMOTOR //
/* 3 */ stepper mystepper(&P6DIR, &P6OUT, BIT5, BIT4, BIT3, BIT2);                                                    // MOTOR A PASOS //
/* 4 */ TECLADO myteclado(&P1DIR, &P1REN, &P1OUT, &P1IE, &P1IES, &P1IFG, BIT2, BIT3, BIT4, BIT5);              // TECLADO //



void main(void) {
    WDTCTL = WDTPW | WDTHOLD;

    P4DIR |= BIT7;
    P4OUT &= ~BIT7;

    P1DIR |= BIT0;
    P1OUT &= ~BIT0;

    estado = 0;
    productoSeleccionado = -1;

    pre_mon5 = mon5;
    pre_mon2 = mon2;
    pre_mon1 = mon1;

    cam_mon5 = 0;
    cam_mon2 = 0;
    cam_mon1 = 0;

    // INGRESAR MONEDAS //
    montoIngresado = 0;

    // INICIALIZACION //
    sensor.configureSensor();               // SENSOR ULTRASONICO //

    // RS-232 //
    P4SEL = BIT4 + BIT5;
    UCA1CTL1 |= UCSWRST;
    UCA1CTL1 |= UCSSEL_2;
    UCA1BR0 = 9;
    UCA1BR1 = 0;
    UCA1MCTL = UCBRS_1 + UCBRF_0;
    UCA1CTL1 &= ~UCSWRST;
    UCA1IE |= UCRXIE;

    sendData(msg);              // OLAF //

    __bis_SR_register(GIE);
    //__no_operation();

    //myservo.servoDegree(0);

    while (1) {

        rule = sensor.pulse();
        sendData(msg);

            while((rule < 100 && rule != 0) || rule_ == 1){         // INICIAR SI SE CUMPLE LA DISTANCIA //


                // SELECCION DE PRODUCTO //
                if (estado == 0){

                    sendData(msg);

                    sendData("\n[SELECCIONAR PRODUCTO]\n\n\r");

                    sendData("[1] AGUA EMBOTELLADA......... $10\n\r");
                    sendData("[2] CHOCOLATES............... $15\n\r");
                    sendData("[3] PANQUESITOS.............. $20\n\r");
                    sendData("[4] PAPAS FRITAS............. $23\n\r");


                    sprintf(pre_mon1_,"\n\n\r %d", mon1);
                    sprintf(pre_mon2_,"\n\n\r %d", mon2);
                    sprintf(pre_mon5_,"\n\n\r %d", mon5);

                    sendData(pre_mon1_);
                    sendData(pre_mon2_);
                    sendData(pre_mon5_);

                    state = sensor.presence();
                    if (state) {
                        return main();
                    }

                    __delay_cycles(3000000);
                }


                // INGRESO DE DINERO //
                if (estado == 1){

                    sendData(msg);

                    sendData("\nPRODUCTO SELECCIONADO: ");
                    sendData(productoSeleccionado_[productoSeleccionado]);

                    sprintf(precio, "%d", precioProducto[productoSeleccionado]);
                    sendData("\r\nPRECIO: $");
                    sendData(precio);

                    sendData("\r\n\n[INGRESA EL DINERO]\n\n\r");
                    sendData("[1]----- INGRESAR $1\n\r");
                    sendData("[2]----- INGRESAR $2\n\r");
                    sendData("[3]----- INGRESAR $5\n\r");
                    sendData("\n[4]----- [FIN]\n\r");

                    sprintf(montoIngresado_, "%d", montoIngresado);
                    sendData("\nMONTO INGRESADO: $");
                    sendData(montoIngresado_);
                    sendData("\n\r");

                    state = sensor.presence();
                    if (state) {
                        return main();
                    }

                    __delay_cycles(3000000);
                }


                // CONFIRMAR COMPRA //
                if (estado == 2){

                    sendData(msg);

                    sendData("\n\r [CONFIRMAR COMPRA]\n\r");

                    sendData("\n\nPRODUCTO SELECCIONADO: ");
                    sendData(productoSeleccionado_[productoSeleccionado]);

                    sendData("\r\nDINERO INGRESADO: ");
                    sendData(montoIngresado_);

                    char restante[10];
                    sprintf(restante, "%d", totalRestante);
                    sendData("\n\rCAMBIO TOTAL: ");
                    sendData(restante);

                    char mensaje[70];

                    sprintf(mensaje, "\n\n\r[CAMBIO ENTREGADO] \n\rMONEDA $5: %d\n\rMONEDA $2: %d\n\rMONEDA $1: %d\n\r", cam_mon5, cam_mon2, cam_mon1);
                    sendData(mensaje);

                    sendData("\n\r [1] CONFIRMAR.");
                    sendData("\n\r [2] CANCELAR.");

                    __delay_cycles(1000000);


                    state = sensor.presence();
                    if (state) {
                        return main();
                    }

                }


                // MODO ADMINISTRADOR //
                if (estado == 10) {
                    sendData("\033[2J\033[H");
                    sendData("\n[MODO ADMINISTRADOR]\n\n\r");

                    sendData("BOTON [1] ---> RELLENAR PRODUCTOS\n\r");
                    sendData("BOTON [2] ---> CORTE DE CAJA\n\r");
                    sendData("BOTON [3] ---> INGRESAR MONEDAS\n\r");
                    sendData("BOTON [4] ---> FIN MODO ADMINISTRADOR\n\r");

                    __delay_cycles(3000000);

                }


                // RELLENADO DE MONEDAS //
                if (estado == 11) {
                    sendData("\033[2J\033[H");
                    sendData("\n[RELLENADO DE MONEDAS]\n\n\r");

                    sprintf(mon1_, "%d", mon1);
                    sprintf(mon2_, "%d", mon2);
                    sprintf(mon5_, "%d", mon5);

                    sendData("MONEDAS $1: ");
                    sendData(mon1_);
                    sendData("\n\rMONEDAS $2: ");
                    sendData(mon2_);
                    sendData("\n\rMONEDAS $5: ");
                    sendData(mon5_);
                    sendData("\n\r");


                    sendData("\n\n\r[1] ---- INGRESAR $1\n\r");
                    sendData("[2] ---- INGRESAR $2\n\r");
                    sendData("[3] ---- INGRESAR $5\n\r");
                    sendData("[4] ---- FIN de RELLENADO DE MONEDAS\n\r");

                    __delay_cycles(3000000);

                }


                // CORTE DE CAJA //
                if (estado == 12) {

                    sendData(msg);
                    sendData("\n\r  [CORTE DE CAJA]\n");

                    sendData("\n\r  [PRODUCTOS VENDIDOS]");

                    char productosVendidos[50];
                    sprintf(productosVendidos, "\n\n\r  AGUA EMBOTELLADA: %d\n\r  CHOCOLATES: %d\n\r  PANQUESITOS: %d\n\r  PAPAS FRITAS: %d\n\r",
                            productoVendido[0], productoVendido[1], productoVendido[2], productoVendido[3]);
                    sendData(productosVendidos);

                    sprintf(ganancia_, "%d", ganancia);
                    sendData("\n  GANANCIA TOTAL: $");
                    sendData(ganancia_);

                    sendData("\n\n\r  [1] FIN");

                    __delay_cycles(3000000);

                }
        }
    }

    return main();
}




#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void) {
    sensor.PORT2_ISR_();
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_ISR(void) {
    sensor.TIMER0_ISR_();
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void) {

    int boton = myteclado.botonValor();

    // SELECCION DEL PRODUCTO //
    if (estado == 0) {

        if (boton >= 0 && boton < 4 && stockProduct[boton] > 0) {
            productoSeleccionado = boton;
            estado = 1;
        }

        else {
            sendData("\n\r[PRODUCTO AGOTADO]\n\r");
            __delay_cycles(2000000);
        }
    }


    // INGRESO DE MONEDAS //
    else if (estado == 1) {

        if (boton == 0) {
            montoIngresado += 1; pre_mon1++;
            sprintf(montoIngresado_, "%d", montoIngresado);
            sendData("\n\r[$1 INGRESADO]");
        }

        else if (boton == 1) {
            montoIngresado += 2; pre_mon2++;
            sprintf(montoIngresado_, "%d", montoIngresado);
            sendData("\n\r[$2 INGRESADO]");

        }

        else if (boton == 2) {
            montoIngresado += 5; pre_mon5++;
            sprintf(montoIngresado_, "%d", montoIngresado);
            sendData("\n\r[$5 INGRESADO]");
        }

        else if (boton == 3) {

            if (montoIngresado >= precioProducto[productoSeleccionado]) {

                verificarCambio();
                if (comeBack) {

                    sendData("\n\r[POR FAVOR, INGRESE CAMBIO]");

                    estado = 1;
                    comeBack = false;

                    __delay_cycles(2000000);
                }

                else {

                    estado = 2;

                    P4OUT |= BIT7;
                    __delay_cycles(1500000);
                    P4OUT &= ~BIT7;
                }
            }

            else {
                sendData("\n[MONTO INSUFICIENTE]\n\r");
                P1OUT |= BIT0;
                __delay_cycles(1500000);
                P1OUT &= ~BIT0;
            }
        }

    }

    // CONFIRMAR COMPRA //
    else if (estado == 2){
        if (boton == 0) {

            sendData("\n\n\rDISPENSANDO PRODUCTO...");
            __delay_cycles(1000000);

            mystepper.stepperMove();
            __delay_cycles(2000000);

            myservo.servoDegree(90);
            sendData("\n\r[GRACIAS POR SU COMPRA, VUELVA PRONTO]");
            __delay_cycles(3000000);
            myservo.servoDegree(0);
            mon5 = pre_mon5;
            mon2 = pre_mon2;
            mon1 = pre_mon1;

            stockProduct[productoSeleccionado]--;
            productoVendido[productoSeleccionado]++;
            ganancia = ganancia + precioProducto[productoSeleccionado];

            montoIngresado = 0;
            productoSeleccionado = -1;
            estado = 0;

            __delay_cycles(2000000);

            return main();
        }

        if (boton == 1) {

            sendData("\n\n\r[COMPRA CANCELADA]");
            pre_mon5 = 0;
            pre_mon2 = 0;
            pre_mon1 = 0;

            montoIngresado = 0;
            productoSeleccionado = -1;

            estado = 0;

            __delay_cycles(2000000);
            return main();
        }
    }

    // MODO ADMINISTRADOR //                // OLAF //
    else if (estado == 10) {
        if (boton == 0){

            stockProduct[1] = 10;
            stockProduct[2] = 10;
            stockProduct[3] = 10;
            stockProduct[4] = 10;

            sendData("\n[LLENADO DE PRODUCTOS CON EXITO]");
            __delay_cycles(2000000);
        }

        else if(boton == 1) {
            estado = 12;

        }

        else if (boton == 2) {
            estado = 11;
        }

        else if (boton == 3) {
            rule_ = 0;
            estado = 0;

        }
    }


    // MODO RELLENADO DE MONEDAS //
    else if (estado == 11) {

        if (boton == 0) {
            mon1++;
            sendData("\r\n[MONEDA $1 AGREGADA]");
            __delay_cycles(1000000);
        }

        if (boton == 1) {
            mon2++;
            sendData("\r\n[MONEDA $2 AGREGADA]");
            __delay_cycles(1000000);
        }

        if (boton == 2) {
            mon5++;
            sendData("\r\n[MONEDA $5 AGREGADA]");
            __delay_cycles(1000000);
        }

        if (boton == 3) {
            estado = 0;
            rule_ = 0;
        }
    }

    else if(estado == 12) {

        if(boton == 0){
            estado = 10;
        }
    }
}



// INGRESO DE MODO ADMINISTRADOR //
#pragma vector=USCI_A1_VECTOR                       // OLAF //
__interrupt void USCI_A1_ISR(void) {
    static char received_char[10];                  // BUFFER PARA RECIBIR CADENA //
    static int index = 0;                           // INDICE DEL BUFFER //

    switch(__even_in_range(UCA1IV, 4)) {
        case 0:
            break;
        case 2:
            received_char[index] = UCA1RXBUF;       // LEER CARACTER RECIBIDO //

            if (received_char[index] == '\r') {     // SI SE RECIBE UN ENTER //
                received_char[index] = '\0';        // TERMINAR LA CADENA CON UN CARACTER NULO //

                // VERIFICAR SI SE INGRESO EL 'admin' //
                if (strcmp(received_char, "admin") == 0) {

                    estado = 10;                    // CAMBIAR EL 'estado' a ADMINISTRADOR //
                    rule_ = 1;
                    sendData("\n\n\r[MODO ADMINSITRADOR INGRESADO]\n\r");
                }

                else {
                    sendData("\n\r[ERROR]\n\r");
                }

                // REINICIAR EL INDICE Y LIMPIAR EL BUFFER //
                index = 0;

                // SALIR DE BAJO CONSUMO SI SE PRESIONA ENTER //
                __bic_SR_register_on_exit(LPM0_bits);
            } else {
                index++;                            // INCREMENTAR EL INDICE PARA EL SIGUIENTE CARACTER //
            }

            break;
        default:
            break;
    }
}




// TRANSMITIR DATOS UART //
void sendData(char *str) {
    while (*str) {
        while (!(UCA1IFG & UCTXIFG));
        UCA1TXBUF = *str++;
    }
}




void verificarCambio() { //montoIngresado, mon5, mon2, mon1, precioProducto[productoSeleccionado]

    totalRestante = montoIngresado - precioProducto[productoSeleccionado];
    int totalRestante_ = totalRestante;

    // MONEDAS $5 //
    if( (totalRestante_/5) <= pre_mon5 && pre_mon5 != 0){
        cam_mon5 = totalRestante_/5;
        pre_mon5 = pre_mon5 - cam_mon5;
        totalRestante_ = (totalRestante_%5);
    }

    // MONEDAS $2 //
    if(totalRestante_/2 <= pre_mon2 && pre_mon2 != 0) {
        cam_mon2 = totalRestante_/2;
        pre_mon2 = pre_mon2 - cam_mon2;
        totalRestante_ = (totalRestante_%2);
    }

    // MONEDAS $1 //
    if(totalRestante_/1 <= pre_mon1 && pre_mon1 != 0){
        cam_mon1 = totalRestante_/1;
        pre_mon1 = pre_mon1 - cam_mon1;
        totalRestante_ = (totalRestante_%1);
    }


    if (totalRestante_ != 0) {
        comeBack = true;
    }

    else
        comeBack = false;
}


