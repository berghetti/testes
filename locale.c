#include <stdio.h>
#include <wchar.h>
#include <locale.h>

int main(void) {

  setlocale(LC_ALL, "C.UTF-8");

  wchar_t  *p = L"Ação" ;

  printf("%ls\n", p);
}
