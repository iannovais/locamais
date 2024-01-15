#ifndef DIAS_H_INCLUDED
#define DIAS_H_INCLUDED

#include "structs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

int dias_no_mes(int mes, int ano);
int data_valida(int dia, int mes, int ano);
int conferir_dias(dr *retirada, dr *entrega);
int dias_juros(dr retir, dr entr);

#endif // DIAS_H_INCLUDED
