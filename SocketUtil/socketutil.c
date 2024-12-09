#include "socketutil.h"


int createTCPIPv4Socket() { return socket(AF_INET, SOCK_STREAM, 0); }

// when we create a socket address or anything inside a function it will create
// it in this stack and when the function returns all that stuff and fields
// we created will be wiped out of memory. so we have to allocate some memory
// and then return it to the user of this function but we have to warn the user
// to free the memory that we have allocated for them
struct sockaddr_in* createIPv4Address(char *ip, int port) {
    // char* ip = "172.253.63.100"; // google ip
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in)); // ipv4 address 16 bytes allocated
    address->sin_family = AF_INET;
    address->sin_port = htons(port); // Big-endian format
    //address.sin_addr.sin_addr;

    if(strlen(ip) == 0)
        address->sin_addr.s_addr = INADDR_ANY;
    else
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);
    
    printf("Address converted successfully.\n");
    return address;
}