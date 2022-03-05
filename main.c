#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "lista_imovel.h"

#define NUM_CORRETORES      5
#define NUM_INQUILINOS      5

#define MAX_CODIGO          1000
#define MAX_PRECO           3000


struct nodo *imoveis_disponiveis = NULL;
struct nodo *imoveis_entregues = NULL;

pthread_mutex_t mutex_disponiveis;
pthread_mutex_t mutex_entregues;

void *inquilino (void *arg) {
    
    int inquilinoid = *((int*)arg);

    int tempo_sleep = rand() % 5 + 1; // dormir entre 1 e 5 segundos
    sleep(tempo_sleep);
    
    int acao = rand() % 2;

    if (acao == 1) { //inquilino aluga imovel (tira da lista de imoveis disponiveis)
    
        pthread_mutex_lock(&mutex_disponiveis);
        int comp_lista = comprimento(imoveis_disponiveis);
        int imovel_idx = rand() % comp_lista; // cria indice randomico para remover da lista
        struct nodo *temp = deleta_imovel(&imoveis_disponiveis, imovel_idx);
        pthread_mutex_unlock(&mutex_disponiveis);

        printf("Inquilino %d removeu o imovel %d (%s, R$%.2f, bairro %s) da lista de imoveis disponiveis\n", inquilinoid,
                                                                                                             temp->imovel.codigo,
                                                                                                             temp->imovel.endereco,
                                                                                                             temp->imovel.preco,
                                                                                                             temp->imovel.bairro);
        free(temp);

    }

    else { //inquilino entrega um imovel (adiciona na lista de imoveis entregues)

        Imovel imovel = { rand() % MAX_CODIGO, "Rua Aleatória", (float)(rand() % MAX_PRECO), "Bairro Aleatório" };
        
        // nao pode mudar a lista de imoveis disponiveis durante uma insercao
        pthread_mutex_lock(&mutex_entregues);
        insere_imovel(&imoveis_entregues, imovel);
        pthread_mutex_unlock(&mutex_entregues);

        printf("Inquilino %d adicionou o imovel %d (%s, R$%.2f, bairro %s) na lista de imoveis entregues\n", inquilinoid,
                                                                                                             imovel.codigo,
                                                                                                             imovel.endereco,
                                                                                                             imovel.preco,
                                                                                                             imovel.bairro);

    }

    pthread_exit(NULL);
}

void *corretor (void *arg) {

    int corretorid = *((int*)arg);

    int tempo_sleep = rand() % 5 + 1; // dormir entre 1 e 5 segundos
    sleep(tempo_sleep);
    
    // seleciona aleatoriamente uma das tres acoes de um corretor
    int acao = rand() % 3;

    if (acao == 1) { // move todos os imoveis da lista de entregues para a lista de disponiveis

        // nao pode haver modificacoes em nenhuma das listas durante essa operacao
        pthread_mutex_lock(&mutex_entregues);
        while (imoveis_entregues != NULL) {
            struct nodo *temp = NULL;
            temp = deleta_primeiro_lista(&imoveis_entregues);
            insere_imovel(&imoveis_disponiveis, temp->imovel);
            free(temp);
        }
        pthread_mutex_unlock(&mutex_entregues);

        printf("Corretor %d moveu todos os imoveis da lista de entregues para a lista de disponiveis\n", corretorid);
    }

    else if (acao == 2) { // adiciona um novo imovel a lista de disponiveis
        Imovel imovel = { rand() % MAX_CODIGO, "Rua Generica", (float)(rand() % MAX_PRECO), "Bairro Generico" };
        
        // nao pode mudar a lista de imoveis disponiveis durante uma insercao
        pthread_mutex_lock(&mutex_entregues);
        insere_imovel(&imoveis_disponiveis, imovel);
        pthread_mutex_unlock(&mutex_entregues);

        printf("Corretor %d adicionou o imovel %d (%s, R$%.2f, bairro %s) na lista de imoveis disponiveis\n", corretorid,
                                                                                                              imovel.codigo,
                                                                                                              imovel.endereco,
                                                                                                              imovel.preco,
                                                                                                              imovel.bairro);
    }

    else { // remove um imovel da lista de disponiveis

        // ninguem pode editar essa lista enquanto é feita a contagem
        // dos elementos e posteriorment a delecao
        pthread_mutex_lock(&mutex_entregues);
        int l = comprimento(imoveis_disponiveis);
        int imovel_idx = rand() % l; // cria indice randomico para remover da lista
        struct nodo *temp = deleta_imovel(&imoveis_disponiveis, imovel_idx);
        pthread_mutex_unlock(&mutex_entregues);

        printf("Corretor %d removeu o imovel %d (%s, R$%.2f, bairro %s } da lista de imoveis disponiveis\n", corretorid,
                                                                                                             temp->imovel.codigo,
                                                                                                             temp->imovel.endereco,
                                                                                                             temp->imovel.preco,
                                                                                                             temp->imovel.bairro);
        free(temp);
    }

    pthread_exit(NULL);
}

int main () {

    // precisa usar um seed diferente para mudar o resultado do rand()
    srand(time(NULL));

    pthread_t threads[NUM_CORRETORES+NUM_INQUILINOS];
    
    // inicializa mutex
    pthread_mutex_init(&mutex_disponiveis, NULL);
    pthread_mutex_init(&mutex_entregues, NULL);

    // popular imoveis disponiveis e entregues
    Imovel im1 = {11, "Rua X", 123, "Trindade"};
    Imovel im2 = {22, "Rua Y", 456, "Carvoeira"};
    Imovel im3 = {33, "Rua Z", 789, "Centro"};
    Imovel im4 = {44, "Rua A", 910, "Itacorubi"};
    Imovel im5 = {55, "Rua B", 111, "Campeche"};
    Imovel im6 = {66, "Rua C", 213, "Ingleses"};

    insere_imovel(&imoveis_disponiveis, im1);
    insere_imovel(&imoveis_disponiveis, im2);
    insere_imovel(&imoveis_disponiveis, im3);
    insere_imovel(&imoveis_entregues, im4);
    insere_imovel(&imoveis_entregues, im5);
    insere_imovel(&imoveis_entregues, im6);

    printf("Imoveis disponiveis: ");
    print_lista_imoveis(imoveis_disponiveis);
    printf("Imoveis entregues: ");
    print_lista_imoveis(imoveis_entregues);

    int rc;
    int i;
    for (i = 0; i < NUM_CORRETORES; i++) {
        rc = pthread_create(&threads[i], NULL, corretor, (void *)&i);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for (i = 0; i < NUM_INQUILINOS; i++) {
        rc = pthread_create(&threads[i+NUM_CORRETORES], NULL, inquilino, (void *)&i);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(i = 0;  i < NUM_CORRETORES+NUM_INQUILINOS; i++)
        pthread_join(threads[i], NULL);
    
    pthread_mutex_destroy(&mutex_disponiveis);
    pthread_mutex_destroy(&mutex_entregues);

    printf("Imoveis disponiveis: ");
    print_lista_imoveis(imoveis_disponiveis);
    printf("Imoveis entregues: ");
    print_lista_imoveis(imoveis_entregues);

    return 0;
}
