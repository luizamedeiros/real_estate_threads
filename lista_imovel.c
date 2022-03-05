#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lista_imovel.h"

int comprimento(struct nodo *head) {
    int comprimento = 0;
    struct nodo *atual;

    for(atual = head; atual != NULL; atual = atual->proximo) {
        comprimento++;
    }

    return comprimento;
}

void print_lista_imoveis(struct nodo *head) {
   struct nodo *ponteiro = head;
   printf("\n[ ");
	
    while(ponteiro != NULL) {
        printf("(%d,%s,%.2f,%s) ", ponteiro->imovel.codigo, ponteiro->imovel.endereco, ponteiro->imovel.preco, ponteiro->imovel.bairro);
        ponteiro = ponteiro->proximo;
    }

    printf(" ]\n");
}

void insere_imovel(struct nodo **head, Imovel imovel) {
   struct nodo *novo_nodo = (struct nodo*) malloc(sizeof(struct nodo));
	
   novo_nodo->imovel = imovel;
   novo_nodo->proximo = *head;
   *head = novo_nodo;
}

struct nodo* deleta_primeiro_lista(struct nodo **head) {

   struct nodo *temp = *head;
   *head = (*head)->proximo;
	
   return temp;
}

struct nodo* deleta_imovel(struct nodo **head, int pos) {

    struct nodo *atual = *head;
    struct nodo *anterior = NULL;

    if (*head == NULL) {
        return NULL;
    }

    int i = 0;
    while(i != pos) {

        if(atual->proximo == NULL) {
            return NULL;
        }
        else {
            anterior = atual;
            atual = atual->proximo;
        }
        i++;
    }

    if(atual == *head) {
        *head = atual->proximo;

    } else {
        anterior->proximo = atual->proximo;
    }

    return atual;
}
