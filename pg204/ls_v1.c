#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>
#include "utils.h"

char mode_to_char(const mode_t m){
  if(S_ISREG(m)) return '-';
  else if(S_ISDIR(m)) return 'd';
  else if(S_ISCHR(m)) return 'c';
  else if(S_ISBLK(m)) return 'b';
  else if(S_ISFIFO(m)) return 'f';
  else if(S_ISLNK(m)) return 's';
  else return '?';
}


void listFiles(const char* dirname){
  DIR* dir=opendir(dirname);
  if(dir==NULL){
    return;
  }
  printf("Reading files in:%s\n",dirname);
  struct dirent* entity;
  entity=readdir(dir);
  while(entity!=NULL){
    struct stat buffer;
    lstat(entity->d_name,&buffer);
    printf("%c %s %s\n",mode_to_char(buffer.st_mode),dirname,entity->d_name);
    if(entity->d_type == DT_DIR && strcmp(entity->d_name,".")!=0 && strcmp(entity->d_name,"..")!=0){//entity->d_type == DT_DIR.(a retenir)
      char path[100]={0};//préparation de la const char* en récursif suivant.
      strcat(path,dirname);
      strcat(path,"/");
      strcat(path,entity->d_name);//pour qu'on ait: dirname/entity->d_name
      //ce qui est cohérent.
      listFiles(path);
    }
    entity=readdir(dir);//on passe au suivant.
    //même si le récursif ne marche pas de ouf, on va l'ignorer pour
    //l'instant.
  }
  closedir(dir);
}

int main(int argc, char** argv){
  if(argc==1) listFiles(".");
  else{
    for(int i=1;i<=argc-1;i++)
      listFiles(argv[i]);
  }
  return 0;//selon l'énoncé on a ajouté le mode récursif seulement.
}
