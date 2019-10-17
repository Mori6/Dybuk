#define SA struct sockaddr 
#define CHARDEF 3


char* ret_next_address_11(char* base, int sub) { 
    
}


char* ret_next_address_24(char* base, int sub) { 
    char* baseptr = strrchr(base, '.');
    int sizeafter = strlen(base) - sizeof(baseptr) + 1;
    baseptr++;
    char sub24findr[CHARDEF];
    strncpy(sub24findr, baseptr, sizeafter);
    printf("final string: %s\n", sub24findr);


}

void range_scanner(char* base, int subnet, int port) { 
    int sockfd;
    ret_next_address_24(base, subnet);
    struct  sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) { 
        printf("coudld'nt make a socket for scanning...\n");
        exit(0);
    }
    else { 
        bzero(&servaddr, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("");
        servaddr.sin_port = htons(port);
    }


 if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 


    else { 
        printf("port open\n");
    }


    }
