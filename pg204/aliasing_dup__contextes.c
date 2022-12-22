#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define FILENAME "donnees.txt"
#define BLOCK_SIZE 4

int read_and_print_block(int fd) { /*fonction de transfert de base*/
    char data[BLOCK_SIZE+1];
    ssize_t n = read(fd, data, BLOCK_SIZE);
    //lecture et mise de 4 octets dans le char* qui mesure 5 en total.
    if (-1 == n) {
        perror("read");
        return EXIT_FAILURE;
    } else if (n < BLOCK_SIZE) {
        fprintf(stderr, "Unexpected end of file.\n");
        return EXIT_FAILURE;
    }
    data[BLOCK_SIZE] = '\0';
    printf("read: \"%4s\"\n", data);
    return EXIT_SUCCESS;
}

int main(int argc, char ** argv)
{
    int fd1, fd2;
    fd1 = open(FILENAME, O_RDONLY);
    if (-1 == fd1) {
        perror("open(fd1)");
        return EXIT_FAILURE;
    }

#ifdef USE_DUP
    fd2 = dup(fd1);
    if (-1 == fd2) {
        perror("dup(fd1)");
        return EXIT_FAILURE;
    }
#elif defined(USE_OPEN)
    fd2 = open(FILENAME, O_RDONLY);
    if (-1 == fd1) {
        perror("open(fd1)");
        return EXIT_FAILURE;
    }
#else
#error You should request to use whether dup(2) or open(2)
#endif

    read_and_print_block(fd1);
    read_and_print_block(fd2);
    //dans le cas de l'alias_open.c, on ré-ouvre le fichier de nouveau donc
    //un nouveau descripteur ainsi le curseur se situe au début les 2 affichages
    //sont alors pareils.

    //dans le cas de l'alias-dup.c(l'aliasing avec dup()), on duplique le fd
    //soit on re-crée un nouveau pointeur vers le fichier qui est le même que
    //l'avant, ce qui veut dire que le curseur a été déplacé, soit les 2
    //affichages se suivent.

    //voilà le résumé de l'exo3 avec dup2 comme exec. de alias_open
    //et dup3 de alias_dup.
    return EXIT_SUCCESS;
}
