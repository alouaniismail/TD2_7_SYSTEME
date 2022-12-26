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

//ce fichier lit l'entrée standard comme les exos sur read(2)
//et le met dans le tube.

int main(int argc, char** argv){
  //lire ce qu'il y a dans STDIN et le mettre dans la fifo.
  int out;
  ssize_t n;
  char buf[MAX_BUF_SIZE];

  out=open(HALF_DUPLEX,O_WRONLY);
  exit_if(out==-1,"probleme ouverture de la fifo");
  n=1;
  while(n>0){
    n=read(STDIN_FILENO,buf,MAX_BUF_SIZE);
    exit_if(n<0,"probleme de lecture depuis l'entrée standard");
    exit_if((write(out,buf,n))==-1,"probleme de report sur la fifo");
  }
  close(out);
  return EXIT_SUCCESS;
}
    
