#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/riscv.h"

int main()
  {
    // Obtener la dirección inicial del heap
    char *start = sbrk(0);
    printf("Se define start = %p\n", start);
    // Solicitar una página de memoria
    sbrk(PGSIZE);
    // Obtener la dirección inicial del heap
    printf("Dirección inicial del heap (start): %p\n", start);
    // Proteger la página de memoria
    if (mprotect(start, 1) == -1)
    {
        printf("mprotect failed\n");
        return -1;
    }
    // Intentar escribir en la página de memoria protegida
    printf("Intentando escribir en la dirección protegida: %p\n", start);
    char *end = start;
    *end = 'a';
    printf("Valor al escribir: %p\n", end);
    // Desproteger la página de memoria
    printf("unprotecting the address: %p\n", start);
    if (munprotect(start, 1) == -1)
    {
        printf("munprotect failed\n");
        return -1;
    }
    // Intentar escribir en la página de memoria desprotegida
    printf("Intentando escribir en la dirección desprotegida: %p\n", start);
    *end = 'b';
    printf("Valor al escribir: %s\n", end);
    return 0;
  }