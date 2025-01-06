#include "../SocketUtil/socketutil.h"

struct AcceptedSocket
{
    int acceptedSocketFD;
    struct sockaddr_in address;
    int error;
    bool acceptedSucess;
};

struct AcceptedSocket * acceptIncomingConnection(int serverSocketFD);
/* accepts an incoming client connection and returns an AcceptedSocket struct 
   that contains information about the client. accept() system that is used to 
   accept an incoming connection request and returns a new socket file 
   descriptor that is used to communicate with the client. if connection fails
   then set the error value.*/

void receivePrintIncomingData(int socketFD);
/* listens for incoming data from a client and prints it to the console. recv()
   is used to receive data from the client reading up to sizeof(buffer) - 1
   bytes. When recv() returns 0, it means the client disconnected and the loop 
   breaks and will close the socket. */

void startAcceptingIncomingConnections(int serverSocketFD);
/* continueously accept any incoming connections from new clients and creates a 
   new thread for each new connection. Adding a new client to the list. */

void receivePrintIncomingDataonSeparateThread(struct AcceptedSocket *pSocket);
/* create a new thread for each client to handle incoming data concurrently. 
   the function calls receivePrintIncomingData() which will receive and process
   data from the client asynchronously */

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
/* sends the received message stored in buffer to all clients but the one that 
   sent the message. Iterating through acceptedSockets array and check each 
   client's socket file descriptor. */

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