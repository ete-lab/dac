#ifndef COMUM_H
#define COMUM_H

#include <winsock.h>

#define MODULO_DAC_1082 6
#define BRANCH 0
#define CRATE 0
#define F_WRITE  16

extern struct camac_ext ext;
const float FEB1082 = 1023;           // fundo de escala binário do DAC1082 (10 bits)

typedef struct{ 
    short data;
	int qx;
} retorno;



//funções


#endif