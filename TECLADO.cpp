
#include <teclado.h>
#include <msp430.h>

TECLADO *TECLADO_instance = nullptr;// puntero global que apunta a la clase

TECLADO::TECLADO(volatile unsigned char* pDIR_,
                 volatile unsigned char* pREN_,
                 volatile unsigned char* pOUT_,
                 volatile unsigned char* pIE_,
                 volatile unsigned char* pIES_,
                 volatile unsigned char* pIFG_,
                 unsigned char bit2_,
                 unsigned char bit3_,
                 unsigned char bit4_,
                 unsigned char bit5_):
                 pDIR(pDIR_), pREN(pREN_), pOUT(pOUT_), pIE(pIE_), pIES(pIES_), pIFG(pIFG_), bit2(bit2_),
                 bit3(bit3_), bit4(bit4_), bit5(bit5_), boton(0)
{
    // Aquí, inicializas el puntero de botones //
    TECLADO_instance = this;

    // Resto de la inicialización de pines //
    *pDIR &= ~(bit2 | bit3 | bit4 | bit5);      // Configura los pines como entradas (botones 1, 2, 3, 4)
    *pREN |= (bit2 | bit3 | bit4 | bit5);       // Habilita las resistencias de pull-up en los pines
    *pOUT |= (bit2 | bit3 | bit4 | bit5);       // Configura los pines en HIGH (pull-up)
    *pIE |= (bit2 | bit3 | bit4 | bit5);        // Habilita interrupciones en los pines
    *pIES |= (bit2 | bit3 | bit4 | bit5);       // Configura las interrupciones para flanco de bajada (presión de botón)
    *pIFG &= ~(bit2 | bit3 | bit4 | bit5);      // Limpia las banderas de interrupción
}


int TECLADO::botonValor()
{
    int boton = -1;

    if (*pIFG & bit2) { boton = 0; *pIFG &= ~bit2; __delay_cycles(1000000);}
    if (*pIFG & bit3) { boton = 1; *pIFG &= ~bit3; __delay_cycles(1000000);}
    if (*pIFG & bit4) { boton = 2; *pIFG &= ~bit4; __delay_cycles(1000000);}
    if (*pIFG & bit5) { boton = 3; *pIFG &= ~bit5; __delay_cycles(1000000);}

    return boton;
}

