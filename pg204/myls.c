#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define CHK_SYSCALL(scall, verr, msg) do {      \
        if ((verr) == (scall)) {                \
            perror(msg);                        \
            return EXIT_FAILURE;                \
        }                                       \
    } while (0)

extern int optind, opterr, optopt;

void usage(const char *name, FILE *outstream) {
    fprintf(outstream, "Usage: %s [-l] [-h]\n", name);
}

char mode_to_char(const mode_t m)
{
    if (S_ISREG(m)) return '-';
    if (S_ISDIR(m)) return 'd';
    if (S_ISLNK(m)) return 'l';
    if (S_ISCHR(m)) return 'c';
    if (S_ISBLK(m)) return 'b';
    if (S_ISFIFO(m)) return 'f';
    if (S_ISSOCK(m)) return 's';
    return '?';
}
        
int ls_directory(const char *dname, const bool type)
{
    DIR *dir;
    struct dirent *entry;
    char *filename = NULL;
    long mp;
    struct stat infos;

    if (type) {
        /* Compute the complete path length */
        CHK_SYSCALL(mp = pathconf(dname, _PC_PATH_MAX), -1, "path max");
        mp += 1; /*< For the '\0' character */
        CHK_SYSCALL(filename = (char *)malloc(sizeof(char[mp])), NULL, "malloc");
    }

    CHK_SYSCALL(dir = opendir(dname), NULL, dname);

    errno = 0;
    while (NULL != (entry = readdir(dir))) {
        if (true == type) {
            /* Build complete path */
            snprintf(filename, mp, "%s/%s", dname, entry->d_name);
            CHK_SYSCALL(lstat(filename, &infos), -1, filename);
            printf("%c ", mode_to_char(infos.st_mode));
        }
        printf("%s%c", entry->d_name, type ? '\n' : ' ');
    }
    if (!type)
        printf("\n");
    if (0 != errno) {
        perror("readdir");
        return EXIT_FAILURE;
    }
    if (filename)
        free(filename);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    int i, ret = EXIT_SUCCESS, option;
    bool type = false;

    /* Option management */
    while (-1 != (option = getopt(argc, argv, "lh"))) {
        switch(option) {
        case 'l':
            /* Long list style */
            type = true;
            break;
     /* Ajouter ici les nouveaux cas:
        case 'R':
            rec = true;
            break;
        case...:
            ...
            break;
     */
        case 'h':
            /* Display help */
            usage(argv[0], stdout);
            return EXIT_SUCCESS;
            break;
        case '?':
            fprintf(stderr, "Unrecognized option: -%c\n", optopt);
            usage(argv[0], stderr);
            return EXIT_FAILURE;
            break;
        }
    }

    /* If no option provided, list current directory. */
    if (optind == argc)
        ret = ls_directory(".", type);
    else {
        for (i = optind; EXIT_SUCCESS == ret && i < argc; ++i)
            ret = ls_directory(argv[i], type);
    }
    return ret;
}
