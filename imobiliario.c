// compilar com gcc –pthread
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS 5


void *thread_inquilino_function(void *threadid) {
  long tid;
  tid = (long)threadid;
  printf("Thread inquilinos em execução #%ld!\n", tid);
  printf("Inquilino X aluga imovel: Y\n");
  // remover o imovel
  sleep(10);
  // adiciona imovel na lista de imovel entregue
  printf("Inquilino X entraga imovel: Y\n");
  pthread_exit(NULL);
} 

void *thread_corretor_function(void *arg){
  printf("Corretor X adiciona imovel: Y\n");
  pthread_exit(NULL);
} 

void *thread_imoveisDisp_function(void *arg){
  printf("Corretor X disponibiliza imovel: Y\n");
  pthread_exit(NULL);
} 

void *thread_imoveisIndisp_function(void *arg){
   printf("Corretor X indisponibiliza imovel: Y\n");
  pthread_exit(NULL);
} 



int main (int argc, char *argv[]) {

  pthread_t threads[NUM_THREADS];

  int rc;

  long t;

  for(t=0; t<NUM_THREADS; t++){
    printf("In thread main: creating thread %ld\n", t);

    rc = pthread_create(&threads[t], NULL, thread_inquilino_function, (void *)t);
    if (rc){
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

  for(t=0; t<NUM_THREADS; t++){
    pthread_join(threads[t], NULL);
  }
  pthread_exit(NULL);
}

