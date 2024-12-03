#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MSG_SIZE 128

void simple_strcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void simple_strcat(char *dest, const char *src) {
    while (*dest) {
        dest++;
    }
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void int_to_str(int num, char *str) {
    char temp[12];
    int i = 0;

    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    while (num > 0) {
        temp[i++] = '0' + (num % 10);
        num /= 10;
    }

    int j = 0;
    while (i > 0) {
        str[j++] = temp[--i];
    }
    str[j] = '\0';
}

int main(int argc, char *argv[]) {
    if (fork() == 0) { // Proceso hijo: envía mensajes
        sleep(5);
        for (int i = 0; i < 10; i++) {

            char msg[MSG_SIZE];
            char num_str[12];

            int_to_str(i, num_str);

            simple_strcpy(msg, "mensaje ");
            simple_strcat(msg, num_str);

            if (send(getpid(), msg) < 0) {
                printf("Error enviando mensaje %d\n", i);
            } else {
                printf("Mensaje %d enviado correctamente\n", i);
            }
            sleep(i+9);
        }
        exit(0);
    } else { // Proceso padre: recibe mensajes
        for (int i = 0; i < 10; i++) {
            sleep(i+8);
            char msg[MSG_SIZE];

            if (receive(msg) < 0) {
                printf("Error recibiendo mensaje %d\n", i);
            } else {
                printf("Mensaje recibido: %s\n", msg);
            }
        }
        wait(0);
    }
    exit(0);
}