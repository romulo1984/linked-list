#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lista.h"

extern Bairro* listaBairro;
extern FILE* saida;

//Find

Bairro* findBairro(Bairro *listaBairro, uint id) {
    Bairro* bairro = listaBairro;
    if (bairro) {
        while (bairro->id != id) {
            if (bairro->prox != NULL) {
                bairro = bairro->prox;
            } else {
                //Bairro inexistente
                return NULL;
            }
        }

        return bairro;
    } else
        return NULL;
}

Rua* findRua(Bairro *bairro, uint id) {
    Rua* rua = bairro->rua;
    if (rua) {
        while (rua->id != id) {
            if (rua->prox != NULL) {
                rua = rua->prox;
            } else {
                //Rua inexistente
                return NULL;
            }
        }

        return rua;
    } else
        return NULL;
}

boolean ruaExists(Bairro *bairro, uint id) {
    Rua* rua = bairro->rua;
    if (rua) {
        while (rua->id != id) {
            if (rua->prox != NULL) {
                rua = rua->prox;
            } else {
                return false;
            }
        }
    } else {
        return false;
    }

    return (rua) ? true : false;
}

Casa* findCasa(Rua *rua, uint id) {
    Casa* casa = rua->casa;
    if (rua && casa) {
        while (casa->id != id) {
            if (casa->prox != NULL) {
                casa = casa->prox;
            } else {
                //Casa inexistente
                return NULL;
            }
        }

        return casa;
    } else
        return NULL;
}

boolean casaExists(Rua *rua, uint id) {
    Casa* casa = rua->casa;
    if (casa) {
        if (casa->prox != NULL) {
            casa = casa->prox;
        } else {
            return false;
        }
    } else {
        return false;
    }

    return (casa) ? true : false;
}

//Cidade

float medirCidade(Bairro* cidade) {
    Bairro* bairro = cidade;
    if (!bairro) {
        return 0;
    }

    Casa *casa = NULL;
    float consumoTotal = 0;

    while (bairro != NULL) {
        Rua *rua = bairro->rua;
        while (rua != NULL) {
            casa = rua->casa;
            while (casa != NULL) {
                consumoTotal = consumoTotal + casa->consumo;
                casa = casa->prox;
            }
            rua = rua->prox;
        }
        bairro = bairro->prox;
    }

    return consumoTotal;
}

//Bairro

float medirBairro(uint id) {
    Bairro *bairro = findBairro(listaBairro, id);
    if (!bairro) {
        return 0;
    }
    Rua *rua = bairro->rua;
    if (!rua) {
        return 0;
    }

    Casa *casa = NULL;
    float consumoTotal = 0;

    while (rua != NULL) {
        casa = rua->casa;
        while (casa != NULL) {
            consumoTotal = consumoTotal + casa->consumo;
            casa = casa->prox;
        }
        rua = rua->prox;
    }

    return consumoTotal;
}

Bairro* InsereBairro(Bairro* bairro, Bairro* lista) {
    bairro->prox = listaBairro;
    return bairro;
}

Bairro* LiberaBairro(Bairro* listaBairro) {
    Bairro* p = listaBairro;
    Bairro* ant = NULL;

    while (p != NULL) {
        ant = p;
        p = p->prox;
        LiberaRua(ant->rua);
        FREE(ant->nome);
        FREE(ant);
    }
    listaBairro = NULL;
    return listaBairro;
}

Bairro* InicializaBairro(int id, char* nome) {
    Bairro* bairro = (Bairro*) malloc(sizeof (Bairro));

    bairro->id = id;
    bairro->nome = (char*) malloc(strlen(nome) + 1 * sizeof (char));
    strcpy(bairro->nome, nome);

    bairro->rua = NULL;

    return bairro;
}


//RUA

float medirRua(uint id, uint bairroId) {
    Bairro *bairro = findBairro(listaBairro, bairroId);
    if (!bairro) {
        return 0;
    }
    Rua *rua = findRua(bairro, id);
    if (!rua) {
        return 0;
    }

    Casa *casa = rua->casa;
    float consumoTotal = 0;

    while (casa != NULL) {
        consumoTotal = consumoTotal + casa->consumo;
        casa = casa->prox;
    }

    return consumoTotal;
}

Rua* InsereRua(Rua* rua, int bairroId) {
    Bairro* bairro = findBairro(listaBairro, bairroId);
    if (!bairro)
        return NULL;

    boolean ruaExiste = false;
    ruaExiste = ruaExists(bairro, rua->id);

    if (ruaExiste) {
        FREE(rua->nome);
        FREE(rua);
        fprintf(saida, "Rua inserida já existe.\n");
        return NULL;
    }

    Rua *l = bairro->rua;

    Rua* ant = NULL; /* ponteiro p/ elemento anterior */
    Rua* p = l; /* ponteiro p/ percorrer lista*/
    while (p != NULL) {
        ant = p;
        p = p->prox;
    }

    if (ant == NULL) { /* insere no início */
        rua->prox = l;
        l = rua;
    } else { /* insere no meio */
        rua->prox = ant->prox;
        ant->prox = rua;
    }

    bairro->rua = l;

    return rua;
}

Casa* LiberaCasa(Casa* casa) {
    Casa* p = casa;
    Casa* ant = NULL;
    
    while (p != NULL) {
        ant = p;
        p = p->prox;
        FREE(ant->nomeConsumidor);
        FREE(ant);
    }
    casa = NULL;
    return casa;
}

Rua* LiberaRua(Rua* rua) {
    Rua* p = rua;
    Rua* ant = NULL;

    while (p != NULL) {
        ant = p;
        p = p->prox;
        LiberaCasa(ant->casa);
        FREE(ant->nome);
        FREE(ant);
    }
    rua = NULL;
    return rua;
}

void RetiraRua(uint ruaId, uint bairroId) {
    Bairro* bairro = findBairro(listaBairro, bairroId);
    if (!bairro)
        return;
    Rua *rua = findRua(bairro, ruaId);
    if (!rua)
        return;

    Rua* ruaAnt = NULL;
    Rua* r = bairro->rua;

    //Libera todas as casas.

    if (rua->casa) {
        Casa* casa = rua->casa;
        while (casa->prox != NULL) {
            RetiraCasa(casa->id, rua->id, bairro->id);
            casa = casa->prox;
        }
    }

    //Apaga referência e libera rua.

    while (r != NULL && r->id != rua->id) {
        ruaAnt = r;
        r = r->prox;
    }

    if (ruaAnt == NULL) {
        /* retira do inicio */
        bairro->rua = r->prox;
    } else {
        /* retira do meio */
        ruaAnt->prox = r->prox;
    }

    FREE(rua->nome);
    FREE(rua);
}

Rua* InicializaRua(int id, char* nome) {
    Rua* rua = (Rua*) malloc(sizeof (struct rua));

    rua->id = id;
    rua->nome = (char*) malloc(strlen(nome) + 1 * sizeof (char));
    strcpy(rua->nome, nome);

    rua->casa = NULL;

    return rua;
}

//CASA

float medirCasa(uint id, uint ruaId, uint bairroId) {
    Bairro *bairro = findBairro(listaBairro, bairroId);
    if (!bairro)
        return 0;
    Rua *rua = findRua(bairro, ruaId);
    if (!rua)
        return 0;
    Casa *casa = findCasa(rua, id);
    if (!casa)
        return 0;
    return casa->consumo;
}

void ConsumirCasa(uint id, uint ruaId, uint bairroId, float consumo) {
    //Checagem
    Bairro *bairro = findBairro(listaBairro, bairroId);
    if (!bairro)
        return;
    Rua *rua = findRua(bairro, ruaId);
    if (!rua)
        return;
    Casa *casa = findCasa(rua, id);
    if (!casa)
        return;
    casa->consumo = casa->consumo + consumo;
}

Casa* InsereCasa(Casa* casa, int ruaId, int bairroId) {

    Bairro* bairro = findBairro(listaBairro, bairroId);
    if (!bairro)
        return NULL;
    Rua *rua = findRua(bairro, ruaId);
    if (!rua)
        return NULL;

    boolean casaExiste = false;
    casaExiste = casaExists(rua, casa->id);

    if (casaExiste) {
        fprintf(saida, "Casa inserida já existe.\n");
        FREE(casa->nomeConsumidor);
        FREE(casa);
        return NULL;
    }

    //opção 1: insere em sequência crescente.

    Casa *l = rua->casa;

    Casa* ant = NULL;
    Casa* p = l;

    int numAnt = 0, num = 0;

    while (p != NULL) {
        num = casa->numcasa - p->numcasa;
        if (num > 0 && (num < numAnt || numAnt == 0)) {
            numAnt = num;
            ant = p;
        }
        p = p->prox;
    }

    if (ant == NULL) { /* insere no início */
        casa->prox = l;
        l = casa;
    } else { /* insere no meio */
        casa->prox = ant->prox;
        ant->prox = casa;
    }

    rua->casa = l;

    return casa; //retorna casa inserida


    //opção 2: insere em sequência decrescente

    //    Casa* ant = NULL;
    //    Casa* p = rua->casa;
    //    int numAnt = 0, num = 0;
    //
    //    for (p = rua->casa; p != NULL; p = p->prox) {
    //        num = p->numcasa - casa->numcasa;
    //        if (num > 0 && (num < numAnt || numAnt == 0)) {
    //            numAnt = num;
    //            ant = p;
    //        }
    //    }
    //
    //    //Insere casas (em sequência).
    //
    //    if (!ant) {
    //        if (!rua->casa)
    //            rua->casa = casa;
    //        else {
    //            casa->prox = rua->casa;
    //            rua->casa = casa;
    //        }
    //    } else {
    //        p = ant->prox;
    //
    //        casa->prox = p;
    //        ant->prox = casa;
    //    }
}

void RetiraCasa(uint id, uint ruaId, uint bairroId) {

    Bairro* bairro = findBairro(listaBairro, bairroId);
    Rua *rua = bairro->rua;
    rua = findRua(bairro, ruaId);

    Casa* l = rua->casa;
    if (!l)
        return; //rua não tem casas;
    Casa* ant = NULL; /* ponteiro p/ anterior */
    Casa* p = rua->casa; /* ponteiro p/ percorrer */

    while (p != NULL && p->id != id) {
        ant = p;
        p = p->prox;
    }

    if (!p)
        return; //Casa não encontrada

    /* retira elemento */
    if (ant == NULL) {
        /* retira do inicio */
        l = p->prox;
    } else {
        /* retira do meio */
        ant->prox = p->prox;
    }
    FREE(p->nomeConsumidor);
    FREE(p);

    rua->casa = l;
}

Casa * InicializaCasa(int idconsumidor, char* nomeConsumidor, int numcasa) {
    Casa* casa = (Casa*) malloc(sizeof (Casa));

    casa->id = idconsumidor;
    casa->consumo = 0;
    casa->numcasa = numcasa;
    casa->nomeConsumidor = (char*) malloc(strlen(nomeConsumidor) + 1 * sizeof (char));
    strcpy(casa->nomeConsumidor, nomeConsumidor);

    return casa;
}