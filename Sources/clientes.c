#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

#include "../Headers/structs.h"
#include "../Headers/clientes.h"

// Procedimento para atribuir o rank do cliente com base nos pontos de fidelidade
void rank_cliente(cliente *c) {
    if (c->ponto_Fidelidade < 500) {
        strcpy(c->rank, "Loca");
    } else if (c->ponto_Fidelidade >= 500 && c->ponto_Fidelidade <= 1000) {
        strcpy(c->rank, "Loca+");
    } else{
        strcpy(c->rank, "Loca++");
    }
}

// Função para encontrar a posição de um cliente no arquivo
int achar_cliente(FILE *arqCliente,int codigoC){
    fflush(stdin);
    int posicao=-1, achou=0;
    cliente c;
    fseek(arqCliente,0,SEEK_SET);
    fread(&c, sizeof(c), 1, arqCliente);

    // Procura o cliente no arquivo
    while(!feof(arqCliente) && !achou){
         posicao++;
        if (c.codigoC==codigoC){
            achou=1;
        }
        fread(&c, sizeof(c),1, arqCliente);
    }

    if(achou){
        return posicao;
    } else{
        return -1;
    }
}

// Procedimento para cadastrar um novo cliente
void cadastrar_cliente(FILE *arqCliente) {
    fflush(stdin);
    cliente c;
    int posicao;

    // Verifica se o arquivo não está vazio para atribuir um novo código ao cliente
    fseek(arqCliente, 0, SEEK_END);
    if (ftell(arqCliente) > 0) {
        fseek(arqCliente, -sizeof(cliente), SEEK_END);
        fread(&c, sizeof(cliente), 1, arqCliente);
        c.codigoC++;
    } else {
        c.codigoC = 1;
    }

    posicao = achar_cliente(arqCliente, c.codigoC);

    printf("Nome: ");
    fflush(stdin);
    gets(c.nome);

    printf("Telefone: ");
    fflush(stdin);
    gets(c.telefone);

    printf("Rua: ");
    fflush(stdin);
    gets(c.endereco.rua);

    printf("Bairro: ");
    fflush(stdin);
    gets(c.endereco.bairro);

    printf("Numero: ");
    scanf("%i", &c.endereco.num);

    printf("Cep: ");
    fflush(stdin);
    gets(c.endereco.cep);

    printf("Cidade: ");
    fflush(stdin);
    gets(c.endereco.cidade);

    printf("Estado: ");
    fflush(stdin);
    gets(c.endereco.estado);

    c.ponto_Fidelidade = 0;

    // Adiciona o cliente no final do arquivo
    fseek(arqCliente, 0, SEEK_END);
    fwrite(&c, sizeof(c), 1, arqCliente);
    fflush(arqCliente);

    printf("\nCliente cadastrado com sucesso!\nCódigo: %i", c.codigoC);

    printf("\n\n");
    system("pause");
}

// Procedimento para exibir informações de um cliente
void informacoes_cliente(FILE *arqCliente){
    fflush(stdin);
    cliente c;
    int posicao, codDigitado;
    printf("Digite o código do cliente que deseja procurar: ");
    fflush(stdin);
    scanf("%3i", &codDigitado);
    posicao = achar_cliente(arqCliente, codDigitado);

    // Lê o arquivo de clientes
    if (posicao != -1){
        fseek(arqCliente, 0, SEEK_SET);
        fread(&c, sizeof(c), 1, arqCliente);

        // Percorre o arquivo até encontrar o cliente desejado
        while(!feof(arqCliente)){
            if(codDigitado == c.codigoC){
                printf("\nNome: %s \n", c.nome);
                printf("Telefone: %s\n", c.telefone);
                printf("Rua: %s\n", c.endereco.rua);
                printf("Bairro: %s\n", c.endereco.bairro);
                printf("Nº: %i\n", c.endereco.num);
                printf("CEP: %s\n", c.endereco.cep);
                printf("Cidade: %s\n", c.endereco.cidade);
                printf("Estado: %s\n", c.endereco.estado);

                // atribui rank ao cliente
                rank_cliente(&c);

                printf("\nRank: %s\n", c.rank);
                printf("Pontos Fidelidade: %i\n", c.ponto_Fidelidade);

                if(c.ponto_Fidelidade >= 500){
                    printf("O cliente é elegível para o KIT LocaMais!\n");
                }

                // Salvar as alterações no arquivo
                fseek(arqCliente, posicao * sizeof(c), SEEK_SET);
                fwrite(&c, sizeof(c), 1, arqCliente);
                fflush(arqCliente);
            }
            fread(&c, sizeof(c), 1, arqCliente);
        }
    } else{
        printf("\nCódigo do cliente não encontrado\n");
    }

    printf("\n");
    system("pause");
}

// Procedimento para exibir locações de um cliente
void locacoes_cliente(FILE *arqLocacao, FILE *arqCliente){
    fflush(stdin);
    locacao l;
    int posicao, codDigitado, achou = 0;
    printf("Digite o código do cliente para procurar suas locações: ");
    fflush(stdin);
    scanf("%3i",&codDigitado);
    posicao=achar_cliente(arqCliente, codDigitado);

    // Lê as locações do cliente
    if (posicao != -1){
        fseek(arqLocacao,0,SEEK_SET);
        fread(&l, sizeof(l),1,arqLocacao);

        // Imprime as locações do cliente
        while(!feof (arqLocacao) ){
            if(codDigitado == l.cod_cliente){
                achou = 1;
                printf("\nLocação código: %llu \n",l.codigo);
                printf("Dias de locacao: %d \n",l.qnt_dias);
                printf("Data da retirada: %d/%d/%d\n",l.retirada.dia,l.retirada.mes,l.retirada.ano);
                printf("Data da entrega: %d/%d/%d\n",l.entrega.dia,l.entrega.mes,l.entrega.ano);
                printf("Seguro? %s\n",l.seguro);
                printf("Código do veiculo: %d\n",l.cod_veiculo);
                printf("Concluida? %s\n",l.baixa);
                printf("\n\n");
            }
            fread(&l, sizeof(l),1, arqLocacao);
        }
        if(achou == 0){
            printf("\nNão foi encontrada nenhuma locação no nome do cliente!\n");
        }
    } else{
        printf("\nCódigo do cliente não encontrado\n");
    }

    printf("\n");
    system("pause");
}

// Procedimento para listar clientes com direito a um kit LocaMais
void kit_locaMais(FILE *arqCliente){
    fflush(stdin);
    cliente c;
    int achou = 0;

    // Lê o arquivo de locações
    fseek(arqCliente,0,SEEK_SET);
    fread(&c, sizeof(c),1, arqCliente);

    printf("Clientes com direito à um kit LocaMais:\n");

    // Imprime os clientes que possuem 500 ou mais pontos
    while (!feof(arqCliente)){
        if(c.ponto_Fidelidade>=500){
            printf("\nCódigo: %d \n",c.codigoC);
            printf("Nome: %s \n",c.nome);
            printf("Telefone: %s\n",c.telefone);
            printf("\nPontos Fidelidade: %i\n",c.ponto_Fidelidade);
            achou = 1;
        }
        fread(&c, sizeof(c),1, arqCliente);
    }
    if(achou == 0){
        printf("\nNenhum cliente tem pontos de fidelidade suficiente!\n");
    }
    printf("\n");
    system("pause");
}
