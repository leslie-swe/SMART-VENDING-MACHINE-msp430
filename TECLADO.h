
#ifndef TECLADO_H_
#define TECLADO_H_

class TECLADO
{
public:
    TECLADO (volatile unsigned char* pDIR_,
             volatile unsigned char* pREN_,
             volatile unsigned char* pOUT_,
             volatile unsigned char* pIE_,
             volatile unsigned char* pIES_,
             volatile unsigned char* pIFG_,
             unsigned char bit2_,
             unsigned char bit3_,
             unsigned char bit4_,
             unsigned char bit5_);

            // METODOS //
             int botonValor();

private:

             unsigned char bit2;
             unsigned char bit3;
             unsigned char bit4;
             unsigned char bit5;
             volatile unsigned char* pDIR;
             volatile unsigned char* pREN;
             volatile unsigned char* pOUT;
             volatile unsigned char* pIE;
             volatile unsigned char* pIES;
             volatile unsigned char* pIFG;
             int boton;
};

#endif /* TECLADO_H_ */
