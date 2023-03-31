#include <bits/time.h>
#define POSIX_C_SOURCE 200112L
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <ucontext.h>
#include <unistd.h>

#define MAX_THREADS 2

int cycles_per_us;
int start_tsc;

typedef struct {
  char *name;
  int count;
  ucontext_t context;
} thread_t;

thread_t threads[MAX_THREADS];
int current_thread = 0;

void timer_handler(int signum) {
  thread_t *prev_thread = &threads[current_thread];
  current_thread = (current_thread + 1) % MAX_THREADS;
  thread_t *next_thread = &threads[current_thread];

  // printf("Handler\n");
  //  Save the current context and switch to the next thread's context
  if (swapcontext(&prev_thread->context, &next_thread->context) == -1) {
    perror("swapcontext");
    exit(1);
  }
}

void setup_timer() {
  struct sigaction sa = {.sa_handler = timer_handler};
  struct itimerval timer;

  // Install timer_handler as the signal handler for SIGVTALRM
  if (sigaction(SIGPROF, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  // Configure the timer to expire after 1 second
  timer.it_value.tv_sec = 1;
  timer.it_value.tv_usec = 0;
  timer.it_interval.tv_sec = 1;
  timer.it_interval.tv_usec = 0;

  // Start the timer
  if (setitimer(ITIMER_PROF, &timer, NULL) == -1) {
    perror("setitimer");
    exit(1);
  }
}

static inline uint64_t rdtsc(void) {
  uint32_t a, d;
  asm volatile("rdtsc" : "=a"(a), "=d"(d));
  return ((uint64_t)a) | (((uint64_t)d) << 32);
}

static inline uint64_t rdtscp(uint32_t *auxp) {
  uint32_t a, d, c;
  asm volatile("rdtscp" : "=a"(a), "=d"(d), "=c"(c));
  if (auxp)
    *auxp = c;
  return ((uint64_t)a) | (((uint64_t)d) << 32);
}

static inline void cpu_relax(void) { asm volatile("pause"); }

static inline void cpu_serialize(void) {
  asm volatile("cpuid" : : : "%rax", "%rbx", "%rcx", "%rdx");
}

void __time_delay_us(uint64_t us) {
  uint64_t cycles = us * cycles_per_us;
  unsigned long start = rdtsc();

  while (rdtsc() - start < cycles)
    cpu_relax();
}

/* derived from DPDK */
static int time_calibrate_tsc(void) {
  /* TODO: New Intel CPUs report this value in CPUID */
  struct timespec sleeptime = {.tv_nsec = 5E8}; /* 1/2 second */
  struct timespec t_start, t_end;

  cpu_serialize();
  if (clock_gettime(CLOCK_MONOTONIC_RAW, &t_start) == 0) {
    uint64_t ns, end, start;
    double secs;

    start = rdtsc();
    nanosleep(&sleeptime, NULL);
    clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);
    end = rdtscp(NULL);
    ns = ((t_end.tv_sec - t_start.tv_sec) * 1E9);
    ns += (t_end.tv_nsec - t_start.tv_nsec);

    secs = (double)ns / 1000;
    cycles_per_us = (uint64_t)((end - start) / secs);
    // printf("time: detected %d ticks / us", cycles_per_us);

    /* record the start time of the binary */
    start_tsc = rdtsc();
    return 0;
  }

  return -1;
}

/**
 * time_init - global time initialization
 *
 * Returns 0 if successful, otherwise fail.
 */
int time_init(void) { return time_calibrate_tsc(); }

void m_sleep(void) { __time_delay_us(1005 * 1000); }

int g_count;

void thread_1() {
  thread_t *thread = &threads[0];
  while (1) {
    printf("%s local %d global %d\n", thread->name, ++thread->count, ++g_count);
    // asm("mov %rsi, %rdi");
    asm("mov $0x10, %r15\n\t");
    // m_sleep();
  }
}

void thread_2() {
  thread_t *thread = &threads[1];
  while (1) {
    printf("%s local %d global %d\n", thread->name, ++thread->count, ++g_count);
    // m_sleep();
  }
}

#define STACK_SIZE 8 * 1024 * 1024

int main() {

  // Initialize the threads
  threads[0].name = "Thread 1";
  threads[0].count = 0;
  getcontext(&threads[0].context);
  threads[0].context.uc_stack.ss_sp = malloc(STACK_SIZE);
  threads[0].context.uc_stack.ss_size = STACK_SIZE;
  threads[0].context.uc_link = NULL;
  makecontext(&threads[0].context, thread_1, 0);

  threads[1].name = "Thread 2";
  threads[1].count = 0;
  getcontext(&threads[1].context);
  threads[1].context.uc_stack.ss_sp = malloc(STACK_SIZE);
  threads[1].context.uc_stack.ss_size = STACK_SIZE;
  threads[1].context.uc_link = NULL;
  makecontext(&threads[1].context, thread_2, 0);

  // Set up the timer
  time_init();
  setup_timer();

  // while (1)
  //   ;

  // Start running the first thread
  if (setcontext(&threads[0].context) == -1) {
    perror("setcontext");
    exit(1);
  }

  return 0;
}
