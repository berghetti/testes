
#include <stdio.h>
#include <stdlib.h>

#define lambda(_fret, _fbody) ({ _fret __fname_ _fbody __fname_; })

int
main (void)
{
  char a[] = { 10, 5, 8, 2, 1, 7 };
  int size = sizeof (a);
  char *p, *end;

  end = a + size;

  p = a;
  while (p < end)
    printf ("%d ", *p++);
  putchar ('\n');

  qsort (a, size, sizeof (*a),
         lambda (
             int, (const void *a, const void *b) {
               return *(char *)a - *(char *)b;
             }));

  p = a;
  while (p < end)
    printf ("%d ", *p++);
  putchar ('\n');

  return 0;
}
