#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#define N 100

Bairro* listaBairro = NULL;
FILE* saida;

int main(int argc, char** argv) {

    float cons = 0;
    int idbairro = 0, idrua = 0, idcasa = 0, numcasa = 0;
    char *classe, *funcao, *nome;
    Rua* rua;
    Bairro* bairro;
    Casa* casa;

    //Bairros
    Bairro* Hell = InicializaBairro(1, "Hell’s Kitchen");
    Bairro* Governors = InicializaBairro(2, "Governors Island");
    Bairro* Harlem = InicializaBairro(3, "Harlem");
    Bairro* Meatpacking = InicializaBairro(13, "Meatpacking District");
    Bairro* Little = InicializaBairro(17, "Little Italy ");
    Bairro* Midtown = InicializaBairro(42, "Midtown Manhattan");
    listaBairro = InsereBairro(Hell, listaBairro);
    listaBairro = InsereBairro(Governors, listaBairro);
    listaBairro = InsereBairro(Harlem, listaBairro);
    listaBairro = InsereBairro(Meatpacking, listaBairro);
    listaBairro = InsereBairro(Little, listaBairro);
    listaBairro = InsereBairro(Midtown, listaBairro);

    Bairro* cidade = listaBairro;

    classe = (char*) (malloc(N * sizeof (char)));
    funcao = (char*) (malloc(N * sizeof (char)));
    nome = (char*) (malloc(N * sizeof (char)));

    FILE* arquivo;

    arquivo = fopen("teste1.txt", "r");
    saida = fopen("saida.txt", "w");
    if (arquivo == NULL) {
        printf("Erro: arquivo inexistente\n");
        return EXIT_FAILURE;
    }

    while (!feof(arquivo)) {
        fscanf(arquivo, "%s", classe);

        //Corrige linha final duplicada
        if (feof(arquivo))
            break;

        if (strcmp(classe, "rua") == 0) {
            fscanf(arquivo, "%s", funcao);

            if (strcmp(funcao, "incluir") == 0) {
                fscanf(arquivo, "%d %d %[^\n]", &idbairro, &idrua, nome);
                bairro = findBairro(listaBairro, idbairro);
                if (bairro) {
                    rua = InicializaRua(idrua, nome);
                    rua = InsereRua(rua, idbairro);
                    if (rua) {
                        fprintf(saida, "Rua incluida com sucesso. Bairro id: %d Rua id: %d Rua nome %s\n", idbairro, idrua, rua->nome);
                    }
                }

            } else {
                if (strcmp(funcao, "eliminar") == 0) {
                    fscanf(arquivo, "%d %d", &idbairro, &idrua);
                    bairro = findBairro(listaBairro, idbairro);

                    if (bairro) {
                        rua = findRua(bairro, idrua);
                        if (rua) {
                            fprintf(saida, "Rua removida com sucesso. Bairro id: %d Rua id: %d\n", idbairro, idrua);
                            RetiraRua(idrua, idbairro);
                        } else
                            fprintf(saida, "ERRO: rua id %d não existe \n", idrua);
                    }
                } else {
                    if (strcmp(funcao, "medir") == 0) {
                        fscanf(arquivo, "%d %d", &idbairro, &idrua);
                        float consumototal = 0;
                        consumototal = medirRua(idrua, idbairro);
                        fprintf(saida, "Consumo total da rua %d: %.3f\n", idrua, consumototal);

                    }
                }
            }
        } else {
            if (strcmp(classe, "casa") == 0) {
                fscanf(arquivo, "%s", funcao);
                if (strcmp(funcao, "incluir") == 0) {

                    fscanf(arquivo, "%d %d %d %d %[^\n]", &idbairro, &idrua, &idcasa, &numcasa, nome);

                    bairro = findBairro(listaBairro, idbairro);
                    if (bairro) {
                        rua = findRua(bairro, idrua);
                        if (rua) {
                            casa = InicializaCasa(idcasa, nome, numcasa);
                            InsereCasa(casa, idrua, idbairro);
                            fprintf(saida, "Casa incluida com sucesso. Bairro id: %d Rua id: %d Casa id: %d Casa nr: %d Consumidor nome %s\n", idbairro, idrua, idcasa, numcasa, nome);
                        } else
                            fprintf(arquivo, "ERRO: Rua inexistente\n");
                    } else
                        fprintf(saida, "ERRO; Bairro inexistente\n");
                } else {
                    if (strcmp(funcao, "eliminar") == 0) {

                        fscanf(arquivo, "%d %d %d", &idbairro, &idrua, &idcasa);

                        bairro = findBairro(listaBairro, idbairro);
                        if (bairro) {
                            rua = findRua(bairro, idrua);
                            if (rua) {
                                casa = findCasa(rua, idcasa);
                                if (casa) {
                                    fprintf(saida, "Casa removida com sucesso. Bairro id: %d Rua id: %d Casa id: %d\n", idbairro, idrua, idcasa);
                                    RetiraCasa(idcasa, idrua, idbairro);
                                } else
                                    fprintf(saida, "ERRO: Casa inexistente\n");
                            } else
                                fprintf(saida, "ERRO: Rua inexistente\n");
                        } else
                            fprintf(saida, "ERRO Bairro inexistente\n");
                    } else {
                        if (strcmp(funcao, "consumir") == 0) {
                            fscanf(arquivo, "%d %d %d %f", &idbairro, &idrua, &idcasa, &cons);

                            bairro = findBairro(listaBairro, idbairro);
                            if (bairro) {
                                rua = findRua(bairro, idrua);
                                if (rua) {
                                    casa = findCasa(rua, idcasa);
                                    if (casa) {
                                        fprintf(saida, "Casa ID %d recebeu consumo %.3f\n", idcasa, cons);
                                        ConsumirCasa(idcasa, idrua, idbairro, cons);
                                    } else
                                        fprintf(saida, "ERRO: Casa inexistente. Não pode receber consumo\n");
                                } else
                                    fprintf(saida, "ERRO: Rua inexistente\n");
                            } else
                                fprintf(saida, "ERRO: Bairro inexistente\n");
                        } else {
                            if (strcmp(funcao, "medir") == 0) {
                                fscanf(arquivo, "%d %d %d", &idbairro, &idrua, &idcasa);
                                float consumototal = 0;
                                consumototal = medirCasa(idcasa, idrua, idbairro);
                                fprintf(saida, "Consumo total da casa: %.3f\n", consumototal);
                            }
                        }
                    }
                }
            } else {
                if (strcmp(classe, "bairro") == 0) {
                    fscanf(arquivo, "%s", funcao);
                    if (strcmp(funcao, "medir") == 0) {
                        fscanf(arquivo, "%d", &idbairro);
                        bairro = findBairro(listaBairro, idbairro);
                        if (bairro) {
                            fprintf(saida, "Consumo total do bairro %d: %.3f\n", bairro->id, medirBairro(idbairro));
                        }
                    }
                } else {
                    if (strcmp(classe, "cidade") == 0) {
                        fscanf(arquivo, "%s", funcao);
                        if (strcmp(funcao, "medir") == 0) {
                            fprintf(saida, "Consumo total da cidade: %.3f\n", medirCidade(cidade));
                        }
                    }
                }
            }
        }
    }

    //Libera dados concatenadamente.
    LiberaBairro(listaBairro);
    cidade = NULL;

    FREE(classe);
    FREE(funcao);
    FREE(nome);
    fclose(arquivo);
    fclose(saida);

    return (0);
}
