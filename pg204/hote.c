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

//ce fichier va ecrire un ensemble d'infos sur la test.fifo
//receuillies depuis la std_in.

int main(int argc, char** argv){
  ssize_t n=1;
  int out;
  char buf[MAX_SIZE];
  out=open(FIFO2,O_WRONLY);
  exit_if(out==-1,"Erreur d'ouverture de la fifo");
  while(n!=0){
    n=read(STDIN_FILENO,buf,MAX_SIZE);
    exit_if(n<0,"Erreur lecture depuis l'entrée standard");
    exit_if((write(out,buf,n))==-1,"Erreur ecriture dans le named pipe");
  }
  return EXIT_SUCCESS;
}
