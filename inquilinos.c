// compilar com gcc –pthread
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 5

// assinatura da função da thread
// recebe um argumento - threadid
void *thread_function(void *threadid) {
  long tid;
  tid = (long)threadid;
  printf("Thread inquilinos em execução #%ld!\n", tid);
  printf("Inquilino X aluga imovel: Y");
  // remover o imovel
  sleep(10);
  // adiciona imovel na lista de imovel entregue
  printf("Inquilino X entraga imovel: Y");
  pthread_exit(NULL);
} 


int main (int argc, char *argv[]) {
  // esta dizendo que terá 5 threads do tipo pthread_t
  pthread_t threads[NUM_THREADS];
  // variavel de controle de retorno caso haja uma excessão na criação de um thread
  int rc;
  // variavel para determinar o identificador ou numero da thread
  long t;
  // loop para as 5 threads 
  for(t=0; t<NUM_THREADS; t++){
    printf("In thread main: creating thread %ld\n", t);
    // &threads[t] é a variavel vetor que vai receber a thread
    // NULL nao estamos mudando nada no comportamento da thread
    // thread_function - funcao que a threas vai executar
    // passando a variavel t como argumento para a funcao thread_function
    rc = pthread_create(&threads[t], NULL, thread_function, (void *)t);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }
  // para que o programa principal nao finalize antes de acabar as outras threads 
  // executa-se um join para cada thread criada
  for(t=0; t<NUM_THREADS; t++){
    // vai agurdar pelo termino inicialmente da thread zero 
    // quando a thread zero finalziar, ele volta p loop e vai p join da thread 1
    // e assim por diante até finalizar todas as threads
    pthread_join(threads[t], NULL);
  }
  pthread_exit(NULL);
}

