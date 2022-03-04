#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> 
#define NUM_THREADS 2

char *imoveis_disponiveis[NUM_THREADS];

typedef struct rental_t
{
    int id;
    char address;
    float price;
    char neighborhood;
} rental;

void *t_function(void *arg)
{   
    rental *r;
    r = (rental *)arg;
    printf("RENTAL: \n ---- \n id: %i \n address: %s \n price: %f \n neighborhood: %s \n", r->id, r->address, r->price, r->neighborhood);
    sleep(3);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t t[NUM_THREADS];
    rental *r1, *r2;
    long i;

    r1 = malloc(sizeof(r1));
    r1->id = 1;
    r1->address = "Harvey Street 123";
    r1->price = 1800.00;
    r1->neighborhood = "Downtown";

    r2 = malloc(sizeof(r2));
    r2->id = 2;
    r2->address = "Cartwright Crescent 99";
    r2->price = 2400.00;
    r2->neighborhood = "Erin Mills";

    printf("Thread 1 sendo criada \n");
    pthread_create(&t[0], NULL, t_function, (void *)r1);

    printf("Thread 2 sendo criada \n");
    pthread_create(&t[1], NULL, t_function, (void *)r2);

    printf("Aguardando término das threads \n");

    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(t[i], NULL);
    return 0;
}