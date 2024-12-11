#include "../SocketUtil/socketutil.h"

void startListenPrintThread(int fd);

void listenPrint(int socketFD);

int main() {
    int socketFD = createTCPIPv4Socket();
    //address family i net (ipv4)
    // by specifying the protocol layer 0. we want the ip layer to be under
    // neath the transportation layer
    // if socketFileDescriptor is not negatve then it was created succesfully

    struct sockaddr_in *address = createIPv4Address("127.0.0.1", 2000);

    int result = connect(socketFD, (struct sockaddr*)address, sizeof(*address));

    if (result < 0) {
        perror("Connection failed");
        close(socketFD);
        return EXIT_FAILURE;
    }
    if(result == 0)
        printf("connection is sucessful\n");

    startListenPrintThread(socketFD);

    readUserInput(socketFD);

    // char* message;
    // message = "GET \\ HTTP/1.1\r\nHost:google.com\r\n\r\n";
    // send(socketFD, message, strlen(message), 0);

    // char buffer[1024];
    // memset(buffer, 0, sizeof(buffer));
    // recv(socketFD, buffer, 1024, 0);
    
    // printf("response was %s \n", buffer);

    close(socketFD);

    return 0;
}

void readUserInput(int socketFD){
    char *name = NULL;
    size_t nameLen = 0;
    printf("Please enter your name:\n");
    ssize_t nameCount = getline(&name, &nameLen, stdin);
    name[nameCount-1] = 0;

    char *line = NULL;
    size_t len = 0;
    printf("type here(or exit)...\n");

    char buffer[1024];

    while(true)
    {
        ssize_t charCount = getline(&line, &len, stdin);

        sprintf(buffer, "%s: %s", name, line);
        if(charCount > 0)
        {
            if(strcmp(line,"exit\n") == 0)
                break;
            ssize_t amountSent = send(socketFD, buffer, strlen(buffer), 0);
        }
    }
    free(line);
    free(name);
}

void startListenPrintThread(int socketFD){
    pthread_t id;
    pthread_create(&id, NULL, listenPrint, (void*)socketFD);
}

void listenPrint(int socketFD){
    char buffer[1024];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        ssize_t amountReceived = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
        
        if(amountReceived > 0){
            buffer[amountReceived] = '\0';
            printf("%s\n", buffer);
        }
            
        if(amountReceived == 0){
            break;
        }
    }
    close(socketFD);
}