#include <bits/types/sigset_t.h>
#define _POSIX_C_SOURCE 199309L
#include <bits/time.h>
#include <bits/types/siginfo_t.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/ucontext.h>
#include <time.h>
#include <ucontext.h>
#include <unistd.h>

#define MAX_THREADS 2

// https://elixir.bootlin.com/glibc/latest/source/sysdeps/unix/sysv/linux/x86/sys/ucontext.h#L139

static ucontext_t ctx_request;
static ucontext_t ctx_check_request;

static sigset_t block_alarm;

extern int m_setcontext (ucontext_t *);

void
timer_handler (int signum, siginfo_t *si, void *ucontext)
{
  memcpy (&ctx_request, ucontext, sizeof (ucontext_t));
  // ctx_request = ucontext;
  setcontext (&ctx_check_request);
}

void
setup_timer ()
{
  struct sigaction sa
      = { .sa_sigaction = timer_handler, .sa_flags = SA_SIGINFO };
  struct itimerval timer;

  // Install timer_handler as the signal handler for SIGVTALRM
  if (sigaction (SIGPROF, &sa, NULL) == -1)
    {
      perror ("sigaction");
      exit (1);
    }

  // Configure the timer to expire after 1 second
  timer.it_value.tv_sec = 1;
  timer.it_value.tv_usec = 0;
  timer.it_interval.tv_sec = 1;
  timer.it_interval.tv_usec = 0;

  // Start the timer
  if (setitimer (ITIMER_PROF, &timer, NULL) == -1)
    {
      perror ("setitimer");
      exit (1);
    }
}

void
thread_1 (void)
{
  register int i = 0;
  while (1)
    {
      printf ("Runinng long request %d\n", ++i);
      volatile unsigned int t = -1;
      while (t--)
        asm ("pause");

      if (i == 10)
        break;
    }
}

void
check_request (void)
{
  sigprocmask (SIG_BLOCK, &block_alarm, NULL);
  int i = 0;
  printf ("Cheking request %d\n", ++i);

  sigprocmask (SIG_UNBLOCK, &block_alarm, NULL);
  m_setcontext (&ctx_request);
  // setcontext (ctx_request);
  puts ("here");
}

#define STACK_SIZE 8 * 1024 * 1024

int
main ()
{

  ucontext_t ctx_main;

  sigemptyset (&block_alarm);
  sigaddset (&block_alarm, SIGPROF);

  getcontext (&ctx_check_request);
  ctx_check_request.uc_stack.ss_sp = malloc (STACK_SIZE);
  ctx_check_request.uc_stack.ss_size = STACK_SIZE;
  ctx_check_request.uc_link = NULL;
  makecontext (&ctx_check_request, check_request, 0);

  getcontext (&ctx_request);
  ctx_request.uc_stack.ss_sp = malloc (STACK_SIZE);
  ctx_request.uc_stack.ss_size = STACK_SIZE;
  ctx_request.uc_link = &ctx_main;
  makecontext (&ctx_request, thread_1, 0);

  // Set up the timer
  // time_init ();
  setup_timer ();

  // setcontext (&ctx_request);
  swapcontext (&ctx_main, &ctx_request);
  // thread_1 ();

  puts ("End");

  return 0;
}
