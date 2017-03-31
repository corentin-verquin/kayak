/*ensemble des methodes utilisees par le processus client*/
#include "commun.h"
#include "reponse.h"

/*gere toutes les actions que le client doit effectuer*/
void actionClient(SOCKET client,char * dossier){

  FILE *request = fdopen(client, "w+");
  char buf[256];
  http_request req;

  fgets_or_exit(buf,sizeof(buf),request);
  int bad_request=parse_http_request(buf,&req);

  skip_headers(request);

  if ( bad_request==1 )
    send_response ( request , 400 , "Bad Request" , "Bad request\r\n" );
  else if ( req.method == HTTP_UNSUPPORTED )
    send_response ( request , 405 , "Method Not Allowed" , "Method Not Allowed\r\n" );
  else{
    int fd = check_and_open(req.target,dossier);
    if(fd==-1)
      send_response ( request , 404 , "Not Found" , p404 );
    else if(fd==-2)
      send_response (request, 403, "Forbidden", p403);
    else
      send_file(request,fd);
  }
  fermerClient(client,request);
}

/*fonction qui ferme un client proprement*/
void fermerClient(SOCKET client, FILE *request){
  printf("le client avec la socket %d vient de se deconnecter\n", client);
  fclose(request);
  close(client);
  exit(0);
}

/*fonction qui lit dans un stream ou exit si le stream est ferme*/
char * fgets_or_exit ( char * buffer , int size , FILE * stream ){
  if(fgets(buffer, size, stream) == NULL)
    exit(0);

  return buffer;
}

/*fonction qui va passer toutes les lignes du header*/
void skip_headers ( FILE * client ){
 int boolean = 0;
 char buf[256];
  while(boolean == 0){
    fgets_or_exit(buf,sizeof(buf),client);
    if(verifier_ligneVide(buf) == 1)
      boolean = 1;
  }
}

/*fonction qui permet d'envoyer le statut d'une entete*/
void send_status ( FILE * client , int code , const char * reason_phrase ){
  fprintf(client,"HTTP/1.1 %d %s",code,reason_phrase);
}

/*fonction qui permet d'envoyer un message text*/
void send_response ( FILE * client , int code , const char * reason_phrase ,const char * message_body ){
  send_status(client,code,reason_phrase);
  fprintf(client,"\nConnection: close\nContent-Length: %d\n\r\n",(int)strlen(message_body));
  fprintf(client,"%s",message_body);
}

/*fonction qui permet d'envoyer le contenu d'un fichier*/
void send_file( FILE * client ,const int file){
  send_status(client,200,"OK");
  fprintf(client,"\nConnection: close\nContent-Length: %d\n\r\n",get_file_size(file));
  copy(file,client);
}

/*fonction qui copie le contenu d'un fichier*/
int copy(int in, FILE * out){
  char buff[255];
  int tmp =0;
  while((tmp = read(in,buff,255*sizeof(char)))>0){
    fprintf(out,"%s",buff);
  }
  return tmp;
}

/*fonction qui verifie la cible et retourne un descripteur de fichier
 *si l'ouverture a reussi sinon -1 si le fichier n'existe pas ou -2
 *si les droits d'acces ne sont pas respectes*/
int check_and_open (const char * target , const char * document_root ){
  struct stat sb;
  int taille = (int)strlen(target)+(int)strlen(document_root)+1;
  char * str = (char *) malloc(sizeof (char) * taille);
  snprintf(str,taille,"%s%s",document_root,target);
  
  if(access(str,F_OK)==-1){
    return -1;
  }

  if(access(str,R_OK)==-1){
    return -2;
  }

  if(strncmp(target,"/..",3)==0){
    printf("test");
    return -2;
  }

  if (stat(str, &sb) == -1) {
    return -1;
  }
  
  if(S_ISREG(sb.st_mode) == -1){
    return -1;
  }

  int res = open(str,O_RDONLY);
  free(str);
  return res;
}

/*fonction qui retourne la taille d'un fichier*/
int get_file_size(int fd){

  struct stat sb;

  fstat(fd,&sb);

  return (int)sb.st_size;
  
}
