#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include "utils.h"

int main(int argc, char** argv){
  int fd=open("./donnees.txt",O_RDONLY);
  exit_if(fd==-1,"erreur d'ouverture");
  char buffer[5];
  ssize_t n=read(fd,buffer,4);
  exit_if(n<0,"erreur de lecture");
  ssize_t n2=write(0,buffer,n);
  exit_if(n2<0,"erreur d'ecriture");
  close(fd);
  return 0;
}
			      
