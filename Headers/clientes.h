#ifndef CLIENTES_H_INCLUDED
#define CLIENTES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

#include "structs.h"

void rank_cliente(cliente *c);
int achar_cliente(FILE *arqCliente,int codigoC);
void cadastrar_cliente(FILE *arqCliente);
void informacoes_cliente(FILE *arqCliente);
void locacoes_cliente(FILE *arqLocacao, FILE *arqCliente);
void kit_locaMais(FILE *arqCliente);

#endif // CLIENTES_H_INCLUDED
