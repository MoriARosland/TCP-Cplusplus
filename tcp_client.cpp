#include <arpa/inet.h> // For inet_addr
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h> // For close

int main() {
  int sock = 0;
  struct sockaddr_in serv_addr;
  char buffer[1024] = {0};
  const char *message = "Hello from client";

  // Creating socket file descriptor
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cout << "Socket creation error" << std::endl;
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(8080);

  // Convert IPv4 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    std::cout << "Invalid address / Address not supported" << std::endl;
    return -1;
  }

  // Connect to the server
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    std::cout << "Connection Failed" << std::endl;
    return -1;
  }

  send(sock, message, strlen(message), 0);
  std::cout << "Message sent" << std::endl;
  int valread = recv(sock, buffer, 1024, 0);
  std::cout << "Server: " << buffer << std::endl;

  close(sock);
  return 0;
}
