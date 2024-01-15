#ifndef VEICULOS_H_INCLUDED
#define VEICULOS_H_INCLUDED

#include "structs.h"

int localiza_veiculo(FILE *arqVeiculo,int codigoV);
void cadastrar_veiculo(FILE *arqVeiculo);
void informacoes_veiculo(FILE *arqVeiculo);

#endif // VEICULOS_H_INCLUDED
