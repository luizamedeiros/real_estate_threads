#include <stdio.h>
#include <pthread.h>

void *t_function(void *arg){
    printf("Thread IMÓVEIS DISPONÍVEIS em execução! \n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    pthread_t t;
    printf("Thread IMÓVEIS DISPONÍVEIS sendo criada...\n");
    pthread_create(&t, NULL, t_function, NULL);
    printf("Thread IMÓVEIS DISPONÍVEIS terminando...\n");
    pthread_join(t,NULL);

    return 0;
}