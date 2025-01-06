#include "../SocketUtil/socketutil.h"

void startListenPrintThread(int fd);
/* create a new thread for listening to messages from the server*/

void listenPrint(int socketFD);
/* listen for incoming messages and print. if recv() returns 0 it breaks loop */

void readUserInput(int socketFD);
/* This function handles user input by allowing the user to type messages and 
   send them to the server. Message is sent to the server with name appended.*/

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
    char *name = malloc(100 * sizeof(char)); 
    if (name == NULL){
        perror("Failed to allocate memory for name");
        exit(EXIT_FAILURE);
    }
    printf("Please enter your name:\n");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0; // remove newline char

    char line[1024];
    char buffer[1024];
    printf("type here(or exit)...\n");

    while(true)
    {
        fgets(line, sizeof(line), stdin);
        line[strcspn(line, "\n")] = 0;
        sprintf(buffer, "%s: %s", name, line); // appending to buffer
        snprintf(buffer, sizeof(buffer), "%s: %s", name, line);
        send(socketFD, buffer, strlen(buffer), 0);
    }
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