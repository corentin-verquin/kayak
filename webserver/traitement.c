/*ensemble des traitements sur la premiere ligne de l'entete*/
#include "commun.h"

/*return la taille d'un mot*/
int tailleMot(const char *ligne, int num){
  int res = 0;
  int numMot = 0;
  int i = 0;
  while(ligne[i]){
    if (ligne[i] == ' '){
      numMot++;
    }else if (num == numMot){
      res++;
    }
    i++;
  }
  return res;
}

/*fonction qui compte le nombre de mots*/
int nbMots(const char *ch){
  int res = 1;
  int i = 0;
  while(ch[i] != '\n'){
    if (ch[i] == ' ' && (ch[i-1] != ' ' || ch[i+1] != ' ' )){
      res++;
    }
    i++;
  }
  return res;
}

/*fonction qui coupe une chaine en plusieurs sous chaines*/
char **split(const char *ligne, char c){
  int nb = nbMots(ligne);
  char **res = (char **)malloc(sizeof(char *) * nb + 1);
  int cur = 0;
  char *tmp = (char *)malloc(tailleMot(ligne, cur));
  int j = 0;
  int i = 0;
  int taille = (int)strlen(ligne);
  while(i < taille + 1){
    if (ligne[i] == c || ligne[i] == '\0'){
      res[cur] = strdup(tmp);
      free(tmp);
      cur++;
      tmp = (char *)malloc(tailleMot(ligne, cur));
      j = 0;
    }else{
      tmp[j] = ligne[i];
      j++;
    }
    i++;
  }
  return res;
}

/*fonction qui reecrit la cible*/
char * rewrite_target ( char * target ){
 
  if (strcmp(target, "/") == 0){
    target = malloc(strlen("/index.html")+1);
    strcpy(target, "/index.html");
    return target;
  }

  char * tmp=strchr(target,'?');
  if(tmp!=NULL){
    *tmp='\0';
  }
  return target;  
}

/*fonction qui verifie la premiere ligne de l'entete et en extrait les infos*/
int parse_http_request(const char *c, http_request *request){
  //verifie que la ligne contient seulement 3mots
  int i = 0;
  int cpt = 0;
  int res=0;
  for(;i<(int)strlen(c);i++){
    if(c[i] == ' ')
      cpt++;
  }
  if(cpt != 2){
    res= 1;
  }
  //verifie que le premier mot est egal a GET
  char **tmp=split(c,' ');
  if(strcmp(tmp[0],"GET") != 0){
    res= 1;
  }

  //verifie que le troisieme est de la bonne forme
  if(strncmp(tmp[2],"HTTP/1.1", 8) !=0 && strncmp(tmp[2],"HTTP/1.0",8) !=0){
    res= 1;
  }

  if(res==0){
    request->method=HTTP_GET;
  }else{
    request->method=HTTP_UNSUPPORTED;
  }

  request->major_version = 1;
  request->minor_version=1;
  request->target = rewrite_target(tmp[1]);
  if (request->target != tmp[1])
    free(tmp[1]);
  
  return res;
}


/*fonction qui verifie si une ligne est vide*/
int verifier_ligneVide(char* c){
  if((int)strlen(c) >2)
    return 0;

  if((int)strncmp(c,"\r\n",2) == 0)
    return 1;
  else if(strncmp(c,"\n",2) == 0)
    return 1;
  else
    return 0;
}
