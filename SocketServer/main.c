#include "../SocketUtil/socketutil.h"

struct AcceptedSocket
{
    int acceptedSocketFD;
    struct sockaddr_in address;
    int error;
    bool acceptedSucess;
};

struct AcceptedSocket * acceptIncomingConnection(int serverSocketFD);

void receivePrintIncomingData(int socketFD);

void startAcceptingIncomingConnections(int serverSocketFD);

void receivePrintIncomingDataonSeparateThread(struct AcceptedSocket *pSocket);

struct AcceptedSocket acceptedSockets[10];
int acceptedSocketsCount = 0;

void startAcceptingIncomingConnections(int serverSocketFD){
    while(true)
    {
        struct AcceptedSocket* clientSocket = acceptIncomingConnection(serverSocketFD);
        acceptedSockets[acceptedSocketsCount++] = *clientSocket;
        
        receivePrintIncomingDataonSeparateThread(clientSocket);
    }
}

void receivePrintIncomingDataonSeparateThread(struct AcceptedSocket *pSocket){
    pthread_t id;
    pthread_create(&id, NULL, receivePrintIncomingData, pSocket->acceptedSocketFD);
}

void receivePrintIncomingData(int socketFD){
    char buffer[1024];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        ssize_t amountReceived = recv(socketFD, buffer, sizeof(buffer) - 1, 0);
        
        if(amountReceived > 0){
            buffer[amountReceived] = 0;
            printf("%s\n", buffer);

            sendToOtherClients(buffer, socketFD);
        }
        if(amountReceived == 0){
            break;
        }
    }
    close(socketFD);
}

void sendToOtherClients(char *buffer, int socketFD){
    for(int i = 0; i < acceptedSocketsCount; i++)
        if(acceptedSockets[i].acceptedSocketFD != socketFD)
        {
            send(acceptedSockets[i].acceptedSocketFD, buffer, strlen(buffer), 0);
        }
}

struct AcceptedSocket * acceptIncomingConnection(int serverSocketFD){
    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    // socklen_t clientAddressSize = sizeof(clientAddress);
    int clientSocketFD = accept(serverSocketFD, (struct sockaddr*) &clientAddress, &clientAddressSize) ;
    // accept() returns a new socket file descriptor for the accepted connection
    // the new socket is used to send and receive data to/from the client
    // the original socket is used to listen for new connections
    //requires the pointer of the size rather than just size
    //FD of client will be returned

    struct AcceptedSocket* acceptedSocket = malloc(sizeof(struct AcceptedSocket));
    acceptedSocket->address = clientAddress;
    acceptedSocket->acceptedSocketFD = clientSocketFD;
    acceptedSocket->acceptedSucess = clientSocketFD > 0;

    if(!acceptedSocket->acceptedSucess)
        acceptedSocket->error = clientSocketFD; 
    return acceptedSocket;
}

int main() {
    int serverSocketFD = createTCPIPv4Socket();
    struct sockaddr_in *serverAddress = createIPv4Address("", 2000);

    int result = bind(serverSocketFD, (struct sockaddr*) serverAddress, sizeof(*serverAddress));
    if (result == 0)
        printf("socket was bound successfully\n");

    int listenResult = listen(serverSocketFD, 10);
    // 10 is the backlog amount for incoming sockets connecting to server
    // 0 = success

    startAcceptingIncomingConnections(serverSocketFD);

    shutdown(serverSocketFD, SHUT_RDWR);
    return 0;
}