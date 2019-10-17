int can_connect(int socket_descriptor, char* host) { 
    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family = AF_INET;
    server.sin_port = htons(443);

    if (connect(socket_descriptor, (struct sockaddr *)&server, 
        sizeof(server)) < 0) {
            printf("couldnt connect...\n");
            return 0;
    }
    return 1;
}

