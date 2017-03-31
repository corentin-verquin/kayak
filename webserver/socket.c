/*ensemble des methodes relatives a la socket*/
#include "commun.h"

/*fonction qui cree une socket et la return*/
SOCKET creer(void){
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET){
		perror("socket");
		return INVALID_SOCKET;
	}
	return sock;
}

/*fonction qui permet d'initialiser une socket */
int configurer(SOCKET sock){
	int res;
	SDDR_IN sin;
	socklen_t recsize = sizeof(sin);

	sin.sin_addr.s_addr = htonl(INADDR_ANY);  /* Adresse IP automatique */
	sin.sin_family = AF_INET;                 /* Protocole familial (IP) */
        sin.sin_port = htons(PORT);               /* Listage du port */

	int optval = 1;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) == -1){
	  perror("Option SO_REUSEADDR non autorisee.\n ");
	}

	res = bind(sock, (SDDR*)&sin, recsize);
	if(res == SOCKET_ERROR){
		perror("bind");
		return SOCKET_ERROR;
	}
	return res;
}
