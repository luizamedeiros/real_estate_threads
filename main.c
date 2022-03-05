#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "lista_imovel.h"

#define NUM_CORRETORES      4
#define NUM_INQUILINOS      8

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

        printf("Corretor %d dormiu %d segundos e moveu os imoveis da lista de entregues para a de disponiveis!\n", corretorid, tempo_sleep);
        
        // a sessao de prints abaixo nao esta lockada, o que pode ocasionar bugs
        printf("Imoveis disponiveis: ");
        print_lista_imoveis(imoveis_disponiveis);
        printf("Imoveis entregues: ");
        print_lista_imoveis(imoveis_entregues);
    }
    else if (acao == 2) { // adiciona um novo imovel a lista de disponiveis
        Imovel imovel = { rand() % MAX_CODIGO, "Rua Generico da Silva", (float)(rand() % MAX_PRECO), "Bairro Generico" };
        
        // nao pode mudar a lista de imoveis disponiveis durante uma insercao
        pthread_mutex_lock(&mutex_entregues);
        insere_imovel(&imoveis_disponiveis, imovel);
        pthread_mutex_unlock(&mutex_entregues);

        printf("Corretor %d dormiu %d segundos e adicionou o imovel { %d, %s, %.2f, %s } a lista de disponiveis\n", corretorid,
                                                                                                                    tempo_sleep,
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

        printf("Corretor %d dormiu %d segundos e removeu o imovel { %d, %s, %.2f, %s } do indice %d\n", corretorid,
                                                                                                        tempo_sleep,
                                                                                                        temp->imovel.codigo,
                                                                                                        temp->imovel.endereco,
                                                                                                        temp->imovel.preco,
                                                                                                        temp->imovel.bairro,
                                                                                                        imovel_idx);
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
    Imovel im1 = {11, "paula", 123, "liliana"};
    Imovel im2 = {22, "victor", 456, "preuss"};
    Imovel im3 = {33, "eduarda", 789, "bueno"};
    Imovel im4 = {44, "luiza", 910, "hugo"};
    Imovel im5 = {55, "medeiros", 111, "duda"};
    Imovel im6 = {66, "zomigani", 213, "oliveira"};

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

    // printf("Imoveis disponiveis: ");
    // print_lista_imoveis(imoveis_disponiveis);
    // printf("Imoveis entregues: ");
    // print_lista_imoveis(imoveis_entregues);

    return 0;
}