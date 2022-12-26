#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#include"utils.h"

#define HALF_DUPLEX "./fifo_v1.fifo"
#define MAX_BUF_SIZE 255

int transfert(int fd_in, int fd_out){
  char buf[MAX_BUF_SIZE];
  ssize_t numread=1;
  while(0!=numread){
    numread=read(fd_in,buf,MAX_BUF_SIZE);
    if(numread>0){
      write(fd_out,buf,numread);
    }else if(numread<0){
      perror("Error reading");
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}


int main(int argc, char** argv){
  int fd, ret_val;
  fprintf(stderr,"Le tube vers lequel se connecter est: %s\n",HALF_DUPLEX);

  umask(0);//rwx.(avoir tous les droits depuis toutes les sessions)
  ret_val=mknod(HALF_DUPLEX,S_IFIFO|0666,0);//création d'un i-node
  //l'identificateur "propre" au fichier.

  if((-1==ret_val) && (EEXIST!=errno)){
    perror("Error creating the named pipe");
    return EXIT_FAILURE;
  }

  fd=open(HALF_DUPLEX,O_RDONLY);//descripteur maintenant(se rappeler de dup).
  exit_if(fd<0,"error opening the named pipe");

  ret_val=transfert(fd,STDOUT_FILENO);
  close(fd);

  unlink(HALF_DUPLEX);//pour que la prochaine execution ait un sens.
  //un autre file descriptor pourrait pointer dessus apres.
  //soft link => fait comme si il y avait pas de contenu.
  //prévoir les suites.(BUT)
  return ret_val;
}



  
