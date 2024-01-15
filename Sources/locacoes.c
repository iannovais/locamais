#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>
#include <windows.h>

#include "../Headers/dias.h"
#include "../Headers/structs.h"

// Função para localizar a posição de uma locação no arquivo
int localiza_locacao(FILE *arqLocacao,unsigned long long int codigo){
    fflush(stdin);
    int posicao=-1,achou=0;
    locacao l;

    // Lê o arquivo de locações
    fseek(arqLocacao,0,SEEK_SET);
    fread(&l, sizeof(l),1, arqLocacao);

    // Confere se a locação está no arquivo
    while(!feof(arqLocacao) && !achou){
        posicao++;
        if (l.codigo==codigo){
            achou=1;
        }
        fread(&l, sizeof(l),1, arqLocacao);
    }
    if (achou){
        return posicao;
    } else{
        return -1;
    }
}

// Função para listar veículos disponíveis com a quantidade de ocupantes desejada
int lista_ocupantes(FILE *arqVeiculo, int ocupantes){
    fflush(stdin);
    int disp=0;
    veiculo v;

    // Lê o arquivo dos veículos
    fseek(arqVeiculo,0,SEEK_SET);
    fread(&v, sizeof(v),1, arqVeiculo);

    // Imprime todos os veículos correspondentes à busca
    while (!feof(arqVeiculo)){
        if(ocupantes==v.qtd_ocupante){
            if(strcmp(v.status,"disponivel")==0){
                printf("\nVeículo: %d \n",v.codigoV);
                printf("Descrição: %s \n",v.descricao);
                printf("Modelo: %s\n",v.modelo);
                printf("Cor: %s\n",v.cor);
                printf("Placa: %s\n",v.placa);
                printf("Valor diária: %.2f\n",v.valor_diaria);
                printf("Quantidade de ocupantes: %d\n",v.qtd_ocupante);
                printf("Status: %s\n",v.status);
                disp++;
            }
        }
        fread(&v, sizeof(v),1, arqVeiculo);
    }
    return disp;
}

// Procedimento para cadastrar uma nova locação
void cadastrar_locacao(FILE *arqLocacao, FILE *arqVeiculo, FILE *arqCliente){
    fflush(stdin);
    locacao l;
    veiculo v;
    char status[20];
    int posicao,carro_lote=(rand()%9000)+1000,ocupantes,disp=0,cod_cliente;

    printf("Digite o código do cliente: ");
    fflush(stdin);
    scanf("%3d",&cod_cliente);
    posicao=achar_cliente(arqCliente,cod_cliente);

    if(posicao!=-1){
        printf("Digite a quantia de ocupantes desejada: ");
        fflush(stdin);
        scanf("%d",&ocupantes);
        disp=lista_ocupantes(arqVeiculo,ocupantes);

        if(disp>0){
            printf("\nDigite o código de um veiculo disponível desejado: ");
            fflush(stdin);
            scanf("%3d",&l.cod_veiculo);

            // Procura o veículo solicitado
            posicao=localiza_veiculo(arqVeiculo,l.cod_veiculo);
            fseek(arqVeiculo,sizeof(v)*(posicao),SEEK_SET);
            fread(&v, sizeof(v),1, arqVeiculo);
            strcpy(status,v.status);

            // Gera código para a locação
            if ((posicao!=-1)&&(strcmp(status,"disponivel")==0)){
                l.cod_cliente=cod_cliente;
                l.codigo=(l.cod_cliente*(unsigned long long int)10000000)+(l.cod_veiculo*10000)+(carro_lote);
                posicao=localiza_locacao(arqLocacao,l.codigo);

                // Contagem dos dias da locação
                if (posicao==-1){
                    fflush(stdin);
                    l.qnt_dias=conferir_dias(&l.retirada,&l.entrega);
                    l.qnt_dias=l.qnt_dias+1;

                    do{
                        printf("Você deseja aderir ao seguro? [sim ou nao]: ");
                        fflush(stdin);
                        gets(l.seguro);
                    } while(((strcmp(l.seguro,"nao")!=0)&&((strcmp(l.seguro,"sim")!=0))));

                    printf("\nCodigo locacao: %llu",l.codigo);
                    printf("\nDias de locação: %d",l.qnt_dias);
                    strcpy(l.baixa, "nao");
                    fseek(arqLocacao,0,SEEK_END);
                    fwrite(&l, sizeof(l),1,arqLocacao);
                    fflush(arqLocacao);

                    strcpy(v.status, "alugado"); //atualiza status para alugado

                    posicao=localiza_veiculo(arqVeiculo,l.cod_veiculo);
                    fseek(arqVeiculo,sizeof(v)*(posicao),SEEK_SET);
                    fwrite(&v, sizeof(v),1, arqVeiculo);
                    fflush(arqVeiculo);
                } else{
                    printf("\nEsse código já existe\n");
                }
            } else if(posicao==-1){
                printf("Código de veiculo inválido\n");
            } else{
                printf("Veiculo %s!\n",status);
            }
        } else{
            printf("\nPerdão, não há veículos disponíveis no momento :(\n");
        }
    } else{
        printf("\nCódigo de cliente inválido\n");
    }

    printf("\n\n");
    system("pause");
}

// Função para efetuar a baixa de uma locação
void baixa_locacao(FILE *arqLocacao, FILE *arqVeiculo, FILE *arqCliente){
    fflush(stdin);
    unsigned long long int codigo, salva_codigo;
    int posicao1, posicao2, posicao3, dias_de_juros, qnt_dias, dia, mes, ano;
    float valor_final = 0.0;
    locacao l;
    veiculo v;
    cliente c;
    printf("Digite o código da locação: ");
    scanf("%10llu", &codigo);
    fflush(stdin);

    //  Procura a locação do veículo
    posicao1 = localiza_locacao(arqLocacao, codigo);

    if (posicao1 != -1){
        salva_codigo = codigo;
        codigo = ((codigo % 10000000) / 10000);

        // Procura o veículo alugado
        posicao2 = localiza_veiculo(arqVeiculo, codigo);

        if(posicao2 != -1){
            fseek(arqLocacao, sizeof(l) * (posicao1), SEEK_SET);
            fread(&l, sizeof(l), 1, arqLocacao);

            // Confere se ainda não deu baixa e inicia o processo de baixa
            if(strcmp(l.baixa, "nao") == 0){
                dia = l.retirada.dia;
                mes = l.retirada.mes;
                ano = l.retirada.ano;
                fseek(arqVeiculo, sizeof(v) * (posicao2), SEEK_SET);
                fread(&v, sizeof(v), 1, arqVeiculo);
                l.retirada.dia = l.entrega.dia;
                l.retirada.mes = l.entrega.mes;
                l.retirada.ano = l.entrega.ano;
                fscanf(arqVeiculo, "%f", &v.valor_diaria);
                qnt_dias = l.qnt_dias;

                // Calcula valor do alguel
                do{
                    valor_final = 0.0;
                    printf("Data de Entrega: ");
                    fflush(stdin);
                    scanf("%d/%d/%d", &l.entrega.dia, &l.entrega.mes, &l.entrega.ano);
                    dias_de_juros = dias_juros(l.retirada, l.entrega);

                    if (dias_de_juros <= 0){
                        l.qnt_dias = (float)l.qnt_dias + dias_de_juros;
                    }

                    printf("\nTotal de dias de locação: %d\n", l.qnt_dias);

                    if(dias_de_juros > 0){
                        printf("Dias de atraso: %d\n", dias_de_juros);
                    }

                    valor_final = v.valor_diaria * l.qnt_dias;
                    printf("\nValor das diárias: R$%.2f", valor_final);

                    if(strcmp(l.seguro, "sim") == 0){
                        valor_final = (float)valor_final + 50.0;
                        printf("\nSeguro: R$50,00\n");
                    }

                    if(dias_de_juros >= 1){
                        float juros = (valor_final * 0.05) + (30 * dias_de_juros);
                        valor_final = (float)valor_final + juros;
                        printf("Juros por atraso: R$%.2f\n", juros);
                    }

                    if(valor_final <= 0){
                        printf("\nData inválida. Insira uma data válida\n");
                    }
                } while(valor_final <= 0);

                printf("Preço total da sua locação: R$%.2f", valor_final);

                // Procura o cliente
                codigo = salva_codigo;
                codigo = codigo / 10000000;
                posicao3 = achar_cliente(arqCliente, codigo);
                fseek(arqCliente, sizeof(c) * posicao3, SEEK_SET);
                fread(&c, sizeof(c), 1, arqCliente);
                fseek(arqCliente, sizeof(c) * posicao3, SEEK_SET);

                rank_cliente(&c);

                float desconto = 0.0;

                // Desconto para clientes baseados no rank
                if(strcmp(c.rank, "Loca+") == 0){
                    desconto = valor_final * 0.02;
                    printf("\nDesconto de 2%% pelo rank Loca+: -R$%.2f", desconto);
                    valor_final -= desconto;
                    printf("\nValor final com desconto: R$%.2f", valor_final);
                } else if(strcmp(c.rank, "Loca++") == 0){
                    desconto = valor_final * 0.05;
                    printf("\nDesconto de 5%% pelo rank Loca++: -R$%.2f", desconto);
                    valor_final -= desconto;
                    printf("\nValor final com desconto: R$%.2f", valor_final);
                }

                strcpy(v.status, "disponivel"); //atualiza status para disponível

                fseek(arqVeiculo, sizeof(v) * (posicao2), SEEK_SET);
                fwrite(&v, sizeof(v), 1, arqVeiculo);
                fflush(arqVeiculo);

                strcpy(l.baixa, "sim"); // atualiza a baixa para sim
                l.retirada.dia = dia;
                l.retirada.mes = mes;
                l.retirada.ano = ano;
                fseek(arqLocacao, sizeof(l) * (posicao1), SEEK_SET);
                fwrite(&l, sizeof(l), 1, arqLocacao);
                fflush(arqLocacao);

                // Atualiza pontos fidelidade
                int pt = c.ponto_Fidelidade;
                c.ponto_Fidelidade = (pt + (qnt_dias * 10));
                fwrite(&c, sizeof(c), 1, arqCliente);
                fflush(arqCliente);

                printf("\n\nVocê ganhou %i pontos com essa locação!", qnt_dias*10);
                printf("\nSeus pontos de fidelidade são: %d\n", c.ponto_Fidelidade);
            } else{
                printf("Essa locação já teve baixa");
            }
        } else{
            printf("Erro ao localizar veiculo da locação");
        }
    } else{
        printf("Não há locação com esse código");
    }
    printf("\n");
    system("pause");
}

