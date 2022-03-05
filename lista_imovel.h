#ifndef LISTA_IMOVEL_H
#define LISTA_IMOVEL_H

typedef struct {
    int     codigo;
    char    endereco[128];
    float   preco;
    char    bairro[32];
} Imovel;

struct nodo {
    Imovel imovel;
    struct nodo *proximo;
};

//funcao que retorna o numero de elementos de uma lista
int comprimento(struct nodo *head);

//funcao que printa todos os imoveis em uma lista
void print_lista_imoveis(struct nodo *head);

//funcao que insere um novo imovel em uma lista
void insere_imovel(struct nodo **head, Imovel imovel);

//funcao que deleta o primeiro elemento ([0]) de uma lista
struct nodo* deleta_primeiro_lista(struct nodo **head);

//funcao que deleta um imovel da posicao X de uma lista
struct nodo* deleta_imovel(struct nodo **head, int pos);

#endif