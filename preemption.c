// from chartGPT
// example glibc
// https://elixir.bootlin.com/glibc/latest/source/manual/examples/swapcontext.c#L59
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#define STACK_SIZE 16384

typedef struct thread
{
  int id;
  ucontext_t context;
  char *stack;
  struct thread *next;
} thread_t;

thread_t *current_thread = NULL;
thread_t *ready_queue = NULL;

void
thread_yield ()
{
  thread_t *prev_thread = current_thread;
  current_thread = ready_queue;
  ready_queue = ready_queue->next;
  prev_thread->next = ready_queue;
  ready_queue = prev_thread;
  swapcontext (&(prev_thread->context), &(current_thread->context));
}

void
thread_create (thread_t *thread, void (*func) (void))
{
  thread->stack = malloc (STACK_SIZE);
  getcontext (&(thread->context));
  thread->context.uc_stack.ss_sp = thread->stack;
  thread->context.uc_stack.ss_size = STACK_SIZE;
  thread->context.uc_link = NULL;
  makecontext (&(thread->context), func, 0);
  thread->next = ready_queue;
  ready_queue = thread;
}

void
thread_1 ()
{
  int i;
  for (i = 0; i < 5; i++)
    {
      printf ("Thread 1: %d\n", i);
      thread_yield ();
    }
}

void
thread_2 ()
{
  int i;
  for (i = 0; i < 5; i++)
    {
      printf ("Thread 2: %d\n", i);
      thread_yield ();
    }
}

int
main ()
{
  static thread_t thread1, thread2;

  thread_create (&thread1, thread_1);
  thread_create (&thread2, thread_2);

  current_thread = &thread1;

  while (ready_queue != NULL)
    {
      thread_yield ();
    }

  return 0;
}
