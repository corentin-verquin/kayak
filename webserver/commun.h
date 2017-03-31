/*Ensemble des include utiles dans les fichiers ainsi que
 *les structures et differentes fonctions utiles
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h> 
#include <sys/stat.h>
#include <fcntl.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define PORT 8080

enum http_method {
  HTTP_GET ,
  HTTP_UNSUPPORTED ,
};
typedef struct
{
  enum http_method method ;
  int major_version ;
  int minor_version ;
  char * target ;
} http_request ;

typedef int SOCKET;
typedef struct sockaddr_in SDDR_IN;
typedef struct sockaddr SDDR;

SOCKET creer(void);
int configurer(SOCKET sock);
void initialiser_signaux(void);
void actionClient(SOCKET client,char *dossier);
void fermerClient(SOCKET client, FILE *request);
int verifier_ligneVide(char* c);
char * fgets_or_exit ( char * buffer , int size , FILE * stream );
int parse_http_request(const char *c, http_request *request);
void skip_headers ( FILE * client );
void send_status ( FILE * client , int code , const char * reason_phrase );
void send_response ( FILE * client , int code , const char * reason_phrase ,const char * message_body );
int check_and_open ( const char * target , const char * document_root );
int get_file_size(int fd);
void send_file( FILE * client ,const int file);
int copy(int in, FILE * out);
