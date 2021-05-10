#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>

#define NUM_ITERATIONS 100

void *mythread(void *);

int main(int argc, char **argv)
{
  int numThreads;
  int i;
  pthread_t *tids;
  pthread_attr_t attr;

  numThreads = atoi(argv[1]);

  tids = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);

  /* Limita o tamanho das pilhas das threads em 256 kB.
     Se não fizermos isso pthread_create() usará o
     tamanho padrão (8 MB, no Debian, 10 MB no RHEL).
     Não precisamos dessa pilha toda! */
  pthread_attr_init(&attr);
  pthread_attr_setstacksize(&attr, 128 * 1024);
  // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  /* NOTA: O tempo total cairá abaixo do previsto se
           pthread_create() falhar... */
  for (i = 0; i < numThreads; i++)
    if (pthread_create(tids + i, &attr, mythread, NULL) != 0)
      *(tids + i) = 0;

  /* NOTA: Ok, eu poderia ter criado threads 'detached' aqui.
           É que fiquei com preguiça de difinir o método de
           verificação de término. */
  for (i = 0; i < numThreads; i++)
    if (*(tids + i) != 0)
      pthread_join(*(tids + i), NULL);
      
  free(tids);
  char crap;
  scanf("%c", &crap);

  return 0;
}

/* Conta de 1 até NUM_ITERATIONS, sem interrupções */
void *mythread(void *data)
{
  int i;

  for (i = 0; i < NUM_ITERATIONS; i++);
  pthread_exit(NULL);
}
