#ifndef LOCACOES_H_INCLUDED
#define LOCACOES_H_INCLUDED

#include "structs.h"
#include "dias.h"

int localiza_locacao(FILE *arqLocacao,unsigned long long int codigo);
int lista_ocupantes(FILE *arqVeiculo, int ocupantes);
void cadastrar_locacao(FILE *arqLocacao, FILE *arqVeiculo, FILE *arqCliente);
void baixa_locacao(FILE *arqLocacao, FILE *arqVeiculo, FILE *arqCliente);

#endif // LOCACOES_H_INCLUDED
