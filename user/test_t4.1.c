#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/riscv.h"
#include "kernel/fcntl.h"

int
main(int argc, char *argv[])
{
  char *filename = "testfile";
  int fd;

  // 1. Creación del Archivo: O_CREATE | O_RDWR
  fd = open(filename, O_CREATE | O_RDWR);
  if (fd < 0) {
    printf("Error: No se pudo crear el archivo.\n");
    exit(1);
  }
  printf("Archivo '%s' creado con éxito.\n", filename);

  // 2. Escritura Inicial
  char *initial_content = "Contenido inicial.\n";
  if (write(fd, initial_content, strlen(initial_content)) < 0) {
    printf("Error: No se pudo escribir en el archivo.\n");
    close(fd);
    exit(1);
  }
  printf("Escritura inicial realizada con éxito.\n");
  close(fd);

  // 3. Cambio de Permisos a Solo Lectura
  if (chmod(filename, 1) < 0) {
    printf("Error: No se pudo cambiar los permisos a solo lectura.\n");
    exit(1);
  }
  printf("Permisos cambiados a solo lectura.\n");

  // 4. Prueba de Escritura con Solo Lectura
  fd = open(filename, O_WRONLY);
  if (fd >= 0) {
    printf("Error: Se abrió el archivo en modo escritura cuando debería fallar.\n");
    close(fd);
    exit(1);
  } else {
    printf("Correcto: No se pudo abrir el archivo en modo escritura (solo lectura).\n");
  }

  // 5. Cambio de Permisos de Vuelta a Lectura/Escritura
  if (chmod(filename, 3) < 0) {
    printf("Error: No se pudo cambiar los permisos a lectura/escritura.\n");
    exit(1);
  }
  printf("Permisos cambiados a lectura/escritura.\n");

  // 6. Escritura Final
  fd = open(filename, O_RDWR);
  if (fd < 0) {
    printf("Error: No se pudo abrir el archivo en modo lectura/escritura.\n");
    exit(1);
  }
  char *final_content = "Contenido final.\n";
  if (write(fd, final_content, strlen(final_content)) < 0) {
    printf("Error: No se pudo escribir en el archivo en modo lectura/escritura.\n");
    close(fd);
    exit(1);
  }
  printf("Escritura final realizada con éxito.\n");
  
  close(fd);

  // 7. Mostrar contenido del archivo
  char buf[100];
  fd = open(filename, O_RDONLY);
  if (fd < 0) {
    printf("Error: No se pudo abrir el archivo para lectura.\n");
    exit(1);
  }
  if (read(fd, buf, sizeof(buf)) < 0) {
    printf("Error: No se pudo leer el archivo.\n");
    close(fd);
    exit(1);
  }
  printf("\nContenido del archivo: %s", buf);

  // Fin del programa
  printf("Pruebas completadas exitosamente.\n");
  exit(0);
}