int sd;
    struct hostent *host;
    struct sockaddr_in addr;
    BIO *outbio = NULL;
    SSL_METHOD *method;
    SSL_CTX *ctx;
    SSL *ssl;
    char *req;
    int req_len;
    //char hostname[] = "127.0.0.1";
    //char certs[] = "/etc/ssl/certs/ca-certificates.crt";
    int port = 443;
    int bytes;
    char buf[128];

    // added this to test
       char           dest_url[] = "127.0.0.1";
       BIO              *certbio = NULL;
       X509                *cert = NULL;
       X509_NAME       *certname = NULL;
    BIO *outbio2 = NULL;


void init_SSL_params() { 
    
    OpenSSL_add_all_algorithms();
    ERR_load_BIO_strings();
    ERR_load_crypto_strings();
    SSL_load_error_strings();

}