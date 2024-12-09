#include "../SocketUtil/socketutil.h"

int main() {
    int serverSocketFD = createTCPIPv4Socket();
    struct sockaddr_in *serverAddress = createIPv4Address("", 2000);

    int result = bind(serverSocketFD, (struct sockaddr*) serverAddress, sizeof(*serverAddress));
    if (result == 0)
        printf("socket was bound successfully\n");

    int listenResult = listen(serverSocketFD, 10);
    // 10 is the backlog amount for incoming sockets connecting to server
    // 0 = success

    struct sockaddr_in clientAddress;
    int clientAddressSize = sizeof(struct sockaddr_in);
    int clientSocketFD = accept(serverSocketFD, (struct sockaddr*) &clientAddress, &clientAddressSize) ;
    // accept() returns a new socket file descriptor for the accepted connection
    // the new socket is used to send and receive data to/from the client
    // the original socket is used to listen for new connections
    //requires the pointer of the size rather than just size
    //FD of client will be returned

    char buffer[1024];
    recv(clientSocketFD, buffer, 1024, 0);
    printf("Received message from client: %s\n", buffer);

    return 0;
}