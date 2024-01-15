#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <Windows.h>

#include "Headers/structs.h"
#include "Headers/clientes.h"
#include "Headers/veiculos.h"
#include "Headers/locacoes.h"

// Função principal do programa
int main()
{
  FILE *arqCliente;
  FILE *arqVeiculo;
  FILE *arqLocacao;
  FILE *arqPremio;

  int op, op_sub;

  setlocale(LC_ALL, "Portuguese");
  srand(time(NULL));

  // Inicia a pasta que recebe os arquivos de informações
  if (!(CreateDirectory("Arquivos", NULL)))
  {
    if (GetLastError() != ERROR_ALREADY_EXISTS)
    {
      printf("Erro na criação da pasta!");
      exit(1);
    }
  }

  // Inicia os arquivos para guardar as informações
  if (((arqVeiculo = fopen("Arquivos/veiculoDB.dat", "r+b")) == NULL) || ((arqCliente = fopen("Arquivos/clienteDB.dat", "r+b")) == NULL) || ((arqLocacao = fopen("Arquivos/locadoraDB.dat", "r+b")) == NULL))
  {
    if (((arqVeiculo = fopen("Arquivos/veiculoDB.dat", "w+b")) == NULL) || ((arqCliente = fopen("Arquivos/clienteDB.dat", "w+b")) == NULL) || ((arqLocacao = fopen("Arquivos/locadoraDB.dat", "w+b")) == NULL))
    {
      printf("Erro na criação do arquivo!");
      exit(1);
    }
  }

  do
  {
    system("cls");
    printf("Menu Principal\n");
    printf("[1] Cliente\n");
    printf("[2] Veículos\n");
    printf("[3] Locação\n");
    printf("[4] Sair do sistema\n\n");
    printf("Escolha uma opção: ");
    op = getch();

    switch (op)
    {
    case '1':

      do
      {
        system("cls");
        printf("Menu Clientes\n");
        printf("[1] Cadastrar cliente\n");
        printf("[2] Pesquisar cliente\n");
        printf("[3] Locações do cliente\n");
        printf("[4] Vencedores Kit LocaMais\n");
        printf("[5] Menu anterior\n");
        printf("[6] Sair do sistema\n\n");
        printf("Escolha uma opção: ");
        op_sub = getch();

        switch (op_sub)
        {
        case '1':
          system("cls");
          cadastrar_cliente(arqCliente);
          break;

        case '2':
          system("cls");
          informacoes_cliente(arqCliente);
          break;

        case '3':
          system("cls");
          locacoes_cliente(arqLocacao, arqCliente);
          break;

        case '4':
          system("cls");
          kit_locaMais(arqCliente);
          break;

        case '5':
          system("cls");
          break;

        case '6':
          printf("\n\nEncerrando o programa");
          printf(".");
          Sleep(1000);
          printf(".");
          Sleep(1000);
          printf(".\n");
          Sleep(1000);

          exit(0);
          break;

        default:
          printf("\nOpção inválida\n");
          Sleep(2000);
          break;
        }
      } while (op_sub != '5');

      break;

    case '2':
      do
      {
        system("cls");
        printf("Menu Veículos\n");
        printf("[1] Cadastrar veículo\n");
        printf("[2] Pesquisar veículo\n");
        printf("[3] Menu anterior\n");
        printf("[4] Sair do sistema\n\n");
        printf("Escolha uma opção: ");
        op_sub = getch();

        switch (op_sub)
        {
        case '1':
          system("cls");
          cadastrar_veiculo(arqVeiculo);
          break;

        case '2':
          system("cls");
          informacoes_veiculo(arqVeiculo);
          break;

        case '3':
          system("cls");
          break;

        case '4':
          printf("\n\nEncerrando o programa");
          printf(".");
          Sleep(1000);
          printf(".");
          Sleep(1000);
          printf(".\n");
          Sleep(1000);

          exit(0);
          break;

        default:
          printf("\nOpção inválida\n");
          Sleep(2000);
          break;
        }
      } while (op_sub != '3');
      break;

    case '3':
      do
      {
        system("cls");
        printf("Menu Locações\n");
        printf("[1] Cadastrar locação\n");
        printf("[2] Dar baixa em locação\n");
        printf("[3] Menu anterior\n");
        printf("[4] Sair do sistema\n\n");
        printf("Escolha uma opção: ");
        op_sub = getch();

        switch (op_sub)
        {
        case '1':
          system("cls");
          cadastrar_locacao(arqLocacao, arqVeiculo, arqCliente);
          break;

        case '2':
          system("cls");
          baixa_locacao(arqLocacao, arqVeiculo, arqCliente);
          break;

        case '3':
          system("cls");
          break;

        case '4':
          printf("\n\nEncerrando o programa");
          printf(".");
          Sleep(1000);
          printf(".");
          Sleep(1000);
          printf(".\n");
          Sleep(1000);

          exit(0);
          break;

        default:
          printf("\nOpção inválida\n");
          Sleep(2000);
          break;
        }
      } while (op_sub != '3');
      break;

    case '4':
      printf("\n\nEncerrando o programa");
      printf(".");
      Sleep(1000);
      printf(".");
      Sleep(1000);
      printf(".\n");
      Sleep(1000);

      exit(0);
      break;

    default:
      printf("\nOpção inválida\n");
      Sleep(2000);
      break;
    }
  } while (op != '4');

  // Fecha os arquivos gerados
  fclose(arqCliente);
  fclose(arqVeiculo);
  fclose(arqLocacao);
  fclose(arqPremio);

  return 0;
}
