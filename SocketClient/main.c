#include "../SocketUtil/socketutil.h"

int main() {
    int socketFD = createTCPIPv4Socket();
    //address family i net (ipv4)
    // by specifying the protocol layer 0. we want the ip layer to be under
    // neath the transportation layer
    // if socketFileDescriptor is not negatve then it was created succesfully

    struct sockaddr_in *address = createIPv4Address("127.0.0.1", 2000);

    int result = connect(socketFD, (struct sockaddr*)address, sizeof(*address));
    printf("Result: %d", result);
    if (result < 0) {
        perror("Connection failed");
        close(socketFD);
        return EXIT_FAILURE;
    }
    if(result == 0)
        printf("connection is sucessful\n");

    char* message;
    message = "GET \\ HTTP/1.1\r\nHost:google.com\r\n\r\n";
    send(socketFD, message, strlen(message), 0);

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(socketFD, buffer, 1024, 0);
    
    printf("response was %s \n", buffer);

    return 0;
}