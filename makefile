# Compiler and flags
CC = gcc
CFLAGS = -Wall -I./SocketUtil -g

# Directories for source code
CLIENT_SRC = SocketClient/main.c
SERVER_SRC = SocketServer/main.c
UTIL_SRC = SocketUtil/socketutil.c

# Object files
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)
UTIL_OBJ = $(UTIL_SRC:.c=.o)

# Executables
CLIENT_EXEC = client
SERVER_EXEC = server

# Default target - build both client and server
all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Rule to create the client executable
$(CLIENT_EXEC): $(CLIENT_OBJ) $(UTIL_OBJ)
	$(CC) $(CLIENT_OBJ) $(UTIL_OBJ) -o $(CLIENT_EXEC)

# Rule to create the server executable
$(SERVER_EXEC): $(SERVER_OBJ) $(UTIL_OBJ)
	$(CC) $(SERVER_OBJ) $(UTIL_OBJ) -o $(SERVER_EXEC)

# Rule to compile client source files
$(CLIENT_OBJ): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -c $(CLIENT_SRC) -o $(CLIENT_OBJ)

# Rule to compile server source files
$(SERVER_OBJ): $(SERVER_SRC)
	$(CC) $(CFLAGS) -c $(SERVER_SRC) -o $(SERVER_OBJ)

# Rule to compile the utility source file
$(UTIL_OBJ): $(UTIL_SRC)
	$(CC) $(CFLAGS) -c $(UTIL_SRC) -o $(UTIL_OBJ)

# Clean up object files and executables
clean:
	rm -f $(CLIENT_OBJ) $(SERVER_OBJ) $(UTIL_OBJ) $(CLIENT_EXEC) $(SERVER_EXEC)

.PHONY: all clean
