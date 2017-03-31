/*ensemble des actions effectuees par le serveur*/
#include "commun.h"

/*fonction qui traite le signal 17*/
void traitement_signal(int sig)
{
  printf("Signal %d recu \n" , sig );
  while(waitpid(-1,NULL,WNOHANG) >0);
  printf("En attente d'un client ...\n");
}

/*fonction qui permet d'initialiser les signaux*/
void initialiser_signaux(void){
  struct sigaction sa ;
  sa.sa_handler = traitement_signal ;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART ;

  if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
      perror ( " sigaction ( SIGCHLD ) " );
    }
   
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR){
    perror("signal");
  }
}

/*fonction qui affiche un message d'erreur si la commande est mal utilisee*/
void message_erreur(int n){
  if(n==0){
    printf("Mauvaise utilisation\n Syntaxe correcte:\n kayak [dossier_serveur]\n");
  }else{
    printf("Impossible d'acceder au dossier.\n");
    if(n==1){
      printf("Le parametre n'est pas un dossier.\n");
    }else{
      printf("Vous n'avez pas les droit d'acces au dossier.\n");
    }
  }
}

/*action du serveur*/
int main(int argc, char * argv[]){
 
  if(argc==1){
    message_erreur(0);
    return 1;
  }

  struct stat sb;

  if(access(argv[1],X_OK)==-1){
    message_erreur(2);
    return 1;
  }

  if (stat(argv[1], &sb) == -1) {
    perror("stat");
    exit(EXIT_SUCCESS);
  }

  if((sb.st_mode & S_IFMT)!=S_IFDIR){
    message_erreur(1);
    return 1;
  }

  initialiser_signaux();
  
  //creation socket addressage client
  SOCKET client;
  SDDR_IN csin;
  socklen_t crecsize = sizeof(csin);
	
  //creation socket serveur
  SOCKET serveur = creer();
  if(serveur == -1){
    exit(1);
  }
  if(configurer(serveur) == -1){
    exit(1);	
  }else{
    //demarage du listage
    int tmp = listen(serveur, 5);
    printf("\nListage du port %d...\n\n",PORT);
    if(tmp != SOCKET_ERROR){
      while(1){
	//attente de connection
	printf("En attente d'un client ...\n");
	client = accept(serveur, (SDDR*)&csin, &crecsize);

	if(client != SOCKET_ERROR){

	  printf("un client se connecte avec la socket %d de %s:%d\n", client, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
	  if(fork()==0){
	    actionClient(client,argv[1]);
	  }
	
	}else{
	  perror("accept");
	}

      }
    }else{
      perror("listen");
      exit(1);
    }
  }
  close(serveur);

  return EXIT_SUCCESS;
}
