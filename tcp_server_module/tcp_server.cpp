// 1. using create(), Create TCP socket.
// 2. using bind(), Bind the socket to server address.
// 3. using listen(), put the server socket in a passive mode, where it waits for the client to approach the server to make a connection
// 5. using accept(), At this point, connection is established between client and server, and they are ready to transfer data.
// 5. Go back to Step 3.

#include <arpa/inet.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {

  int socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;

  // Forcefully attaching socket to the port 8080
  if (setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  printf("Created socket\n");

  struct sockaddr_in server_address;

  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(8080);

  // Forcefully attaching socket to the port 8090
  if (bind(socket_descriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  printf("bind success\n");

  // Listen for incomming connections
  printf("listening for connections...\n");
  if (listen(socket_descriptor, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // printf("Accepted message.\n");
  int acc_socket;
  if ((acc_socket = accept(socket_descriptor, (struct sockaddr *)&server_address, (socklen_t *)&server_address)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  char buffer[1024] = {0};
  char server_message[] = "This in an echo from the server.";

  recv(acc_socket, buffer, 1024, 0);
  std::cout << "Message from client: " << buffer << std::endl;
  send(acc_socket, server_message, strlen(server_message), 0);
  std::cout << "Echo sendt to client.\n";

  std::string userInput;

  std::cout << "Enter a string to exit.\n";
  std::getline(std::cin, userInput);

  printf("Closing connection.");

  close(acc_socket);
  close(socket_descriptor);

  return 0;
}