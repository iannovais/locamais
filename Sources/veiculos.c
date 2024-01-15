#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

#include "../Headers/structs.h"
#include "../Headers/clientes.h"
#include "../Headers/veiculos.h"

// Função para localizar a posição de um veículo no arquivo
int localiza_veiculo(FILE *arqVeiculo,int codigoV){
    fflush(stdin);
    int posicao=-1,achou=0;
    veiculo v;

    // Lê o arquivo de veículos
    fseek(arqVeiculo,0,SEEK_SET);
    fread(&v, sizeof(v), 1, arqVeiculo);

    // Confere se o veículo está no arquivo
    while(!feof(arqVeiculo) && !achou){
         posicao++;
        if (v.codigoV==codigoV){
            achou=1;
        }
        fread(&v, sizeof(v),1, arqVeiculo);
    }

    if(achou){
        return posicao;
    } else{
        return -1;
    }
}

// Procedimento para cadastrar um novo veículo
void cadastrar_veiculo(FILE *arqVeiculo) {
    fflush(stdin);
    veiculo v;
    int posicao;

    // Verifica se o arquivo não está vazio para atribuir um novo código ao veiculo
    fseek(arqVeiculo, 0, SEEK_END);
    if (ftell(arqVeiculo) > 0) {
        fseek(arqVeiculo, -sizeof(veiculo), SEEK_END);
        fread(&v, sizeof(veiculo), 1, arqVeiculo);
        v.codigoV++;
    } else {
        v.codigoV = 1;
    }

    posicao = localiza_veiculo(arqVeiculo, v.codigoV);

    printf("Descricao do veiculo: ");
    fflush(stdin);
    gets(v.descricao);

    printf("Modelo do veículo: ");
    fflush(stdin);
    gets(v.modelo);

    printf("Cor do veículo: ");
    fflush(stdin);
    gets(v.cor);

    printf("Placa: ");
    fflush(stdin);
    gets(v.placa);

    v.qtd_ocupante = -1;
    do {
        printf("Quantidade de ocupantes: ");
        fflush(stdin);
        scanf("%i", &v.qtd_ocupante);
    } while ((v.qtd_ocupante <= 0) || (v.qtd_ocupante >= 15));

    strcpy(v.status, "disponivel"); // atualiza status do veículo

    printf("Preço da diária: ");
    fflush(stdin);
    scanf("%f", &v.valor_diaria);

    // Salva as informações no arquivo de veículos
    fseek(arqVeiculo, 0, SEEK_END);
    fwrite(&v, sizeof(v), 1, arqVeiculo);
    fflush(arqVeiculo);

    printf("\nVeículo cadastrado com sucesso!\nCódigo: %i", v.codigoV);

    printf("\n\n");
    system("pause");
}

// Procedimento para exibir informações de um veículo
void informacoes_veiculo(FILE *arqVeiculo){
    fflush(stdin);
    veiculo v;
    int posicao, codDigitado;
    printf("Digite o código do veiculo que deseja procurar: ");
    fflush(stdin);
    scanf("%3i",&codDigitado);
    posicao=localiza_veiculo(arqVeiculo, codDigitado);

    // Lê o arquivo de veículos
    if (posicao != -1){
        fseek(arqVeiculo,0,SEEK_SET);
        fread(&v, sizeof(v),1,arqVeiculo);

        // Imprime informações sobre o veículo
        while(!feof (arqVeiculo) ){
            if(codDigitado == v.codigoV){
                printf("\nModelo: %s\n",v.modelo);
                printf("Descrição: %s \n",v.descricao);
                printf("Cor: %s\n",v.cor);
                printf("Placa: %s\n",v.placa);
                printf("Qtd de ocupantes: %i\n",v.qtd_ocupante);
                printf("Status: %s\n",v.status);
                printf("Valor diária: %.2f\n",v.valor_diaria);
            }
            fread(&v, sizeof(v),1, arqVeiculo);
        }
    } else{
        printf("\nCódigo do veículo não encontrado\n");
    }

    printf("\n");
    system("pause");
}
