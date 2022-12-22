#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

void type(int fd){
  struct stat type;
  fstat(fd,&type);
  mode_t m=type.st_mode;
  if(S_ISREG(m)) printf("fichier normal");
  else if(S_ISDIR(m)) printf("c' est un répertoire");
  else if(S_ISCHR(m)) printf("device type char 1 par un.");
  else if(S_ISBLK(m)) printf("device type block.");
  else if(S_ISFIFO(m)) printf("c'est une fifo");
  else if(S_ISLNK(m)) printf("c'est un lien symbolique");
  else if(S_ISSOCK(m)) printf("socket type--.");
  else exit(-1);
}

int main(int argc, char** argv, char** envp){
  //  int fd=1;(sinon tjrs le terminal d'entrée (ecriture apres scanf par exemple) )pas de redirection.
type(STDIN_FILENO);
//et voilà!
  return 0;
}
