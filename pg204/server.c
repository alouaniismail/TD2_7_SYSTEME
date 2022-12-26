#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include"utils.h"

#define FIFO2 "./test.fifo"
#define MAX_SIZE 255

int main(int argc, char** argv){
  umask(0);//rwx.

  //on dispose déjà de la fifo crée, faire un mknod
  //avec S_IFIFO|0666,0 puis un unlink(fd) après le open et les
  //instructions, c'est lourd.
  int fd=open(FIFO2,O_RDONLY);
  char buf[MAX_SIZE];

  ssize_t n=1;
  while(n!=0){
    n=read(fd,buf,MAX_SIZE);
    exit_if(n<0,"Erreur lecture depuis le pipe");
    exit_if((write(STDOUT_FILENO,buf,n))==-1,"Erreur reportage->la sortie0");
  }
  close(fd);
  //cela reste en continue pour les deux
  //c'est pour cela que ça s'arrête pas, c'est le principe.
  return EXIT_SUCCESS;
}
