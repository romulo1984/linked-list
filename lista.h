#define FREE(p) if(p!=NULL){free(p);p=NULL;}
#define PFREE(p) if(p!=NULL){free(p);}
typedef unsigned int uint;
typedef enum {false = 0, true} boolean;

typedef struct casa {
    int id;
    int numcasa;
    float consumo;
    char *nomeConsumidor;
    struct casa *prox;
} Casa;

typedef struct rua {
    int id;
    char *nome;
    Casa *casa;
    struct rua *prox;
} Rua;

typedef struct bairro {
    int id;
    char *nome;
    Rua *rua;
    struct bairro* prox;
} Bairro;

//Cidade

float medirCidade(Bairro* cidade);

//Bairro

Bairro* findBairro(Bairro *listaBairro, uint id);
float medirBairro(uint id);
Bairro* InsereBairro(Bairro* bairro, Bairro* lista);
Bairro* RetiraBairro(Bairro* lista, int id);
Bairro* LiberaBairro(Bairro* lista);
Bairro* InicializaBairro(int id, char *nome);

//Rua

Rua* findRua(Bairro *bairro, uint id);
boolean ruaExists(Bairro *bairro, uint id);
float medirRua(uint id, uint bairroId);
Rua* InsereRua(Rua* rua, int bairroId);
void RetiraRua(uint ruaId, uint bairroId);
Rua* LiberaRua(Rua* lista);
Rua* InicializaRua(int id, char *nome);

//Casa

Casa* findCasa(Rua *rua, uint id);
boolean casaExists(Rua *rua, uint id);
float medirCasa(uint id, uint ruaId, uint bairroId);
void ConsumirCasa(uint id, uint ruaId, uint bairroId, float consumo);
Casa* InicializaCasa(int idconsumidor, char* nomeconsumidor,int numcasa);
Casa* InsereCasa(Casa* casa, int ruaId, int bairroId);
void RetiraCasa(uint id, uint ruaId, uint bairroId);
Casa* LiberaCasa(Casa* lista);