
#define _GNU_SOURCE
#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <immintrin.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

extern void init_errno (uintptr_t);

#define TLS_SIZE (1024 * 4)

char tls_area1[TLS_SIZE], tls_area2[TLS_SIZE];

unsigned long long tls_addr1 = (unsigned long long)tls_area1 + TLS_SIZE - 1;
unsigned long long tls_addr2 = (unsigned long long)tls_area2 + TLS_SIZE - 1;

static __thread volatile int x;

void
test_malloc (size_t size)
{
  void *p;
  printf ("TID: %u\n", gettid ());
  p = malloc (size);
  assert (p);
  memset (p, 0, size);
  free (p);
}

void
test_errno ()
{
  printf ("Errno address: %p\n", &errno);

  int fd = open ("non_existent_file.txt", O_RDONLY);
  assert (fd == -1);
  fprintf (stderr, "Errno %d\n", errno);
  errno = 0;
  // perror ("Error");
}

typedef struct
{
  void *tcb; /* Pointer to the TCB.  Not necessarily the
                thread descriptor used by libpthread.  */
  void *dtv;
  void *self; /* Pointer to the thread descriptor.  */
  int multiple_threads;
  int gscope_flag;
  uintptr_t sysinfo;
  uintptr_t stack_guard;
  uintptr_t pointer_guard;
  unsigned long int unused_vgetcpu_cache[2];
  /* Bit 0: X86_FEATURE_1_IBT.
     Bit 1: X86_FEATURE_1_SHSTK.
   */
  unsigned int feature_1;
  int __glibc_unused1;
  /* Reservation of some values for the TM ABI.  */
  void *__private_tm[4];
  /* GCC split stack support.  */
  void *__private_ss;
  /* The marker for the current shadow stack.  */
  unsigned long long int ssp_base;
  /* Must be kept even if it is no longer used by glibc since programs,
     like AddressSanitizer, depend on the size of tcbhead_t.  */
  __int128 __glibc_unused2[8][4] __attribute__ ((aligned (32)));

  void *__padding[8];
} tcbhead_t;

void
set_tls (unsigned long long new_tp)
{
  printf ("New tls: %p\n", (void *)new_tp);
  tcbhead_t *current, *new;
  current = (tcbhead_t *)__builtin_ia32_rdfsbase64 ();
  new = (tcbhead_t *)new_tp;

  //*new = *current;

  new->tcb = (void *)new;
  new->self = (void *)new;
  //   new->stack_guard = current->stack_guard;
  //   new->pointer_guard = current->pointer_guard;
  //   new->feature_1 = current->feature_1;

  _writefsbase_u64 (new_tp);
}

int
main (void)
{
  unsigned long long tls_default = __builtin_ia32_rdfsbase64 ();
  printf ("default tls address: %p\n", (void *)tls_default);

  // tcbhead_t *tcb = (tcbhead_t *)tls_default;

  // printf ("tcb: %p\ndtv: %p\nself: %p\n", tcb->tcb, tcb->dtv, tcb->self);

  // test on default tls area
  x = 1;
  // test_malloc (4192);
  test_errno ();

  // test on tls area
  set_tls (tls_addr1);
  x = 2;
  // test_malloc (4192);
  test_errno ();

  // test on tls area 2
  //_writefsbase_u64 (tls_addr2);
  // init_errno (tls_addr2);
  // x = 3;
  // test_malloc (8193);
  // test_errno ();

  set_tls (tls_default);
  //_writefsbase_u64 (tls_default);
  printf ("TLS default %p:%d\n", &x, x);

  set_tls (tls_addr1);
  //_writefsbase_u64 (tls_addr1);
  printf ("TLS area 1 %p:%d\n", &x, x);

  ////_writefsbase_u64 (tls_addr2);
  //// printf ("TLS area 2 %p:%d\n", &x, x);

  //// restore original tls
  set_tls (tls_default);
  //_writefsbase_u64 (tls_default);
  return 0;
}
