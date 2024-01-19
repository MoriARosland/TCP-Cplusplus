#include <arpa/inet.h> // For inet_addr
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <thread>
#include <unistd.h> // For close

class TcpServerAgent {
private:
  int socket_fd;
  struct sockaddr_in server_address;

  std::thread listening_thread;

  int initialize_socket();
  //   void listening_task();

public:
  TcpServerAgent(int port);
  ~TcpServerAgent();

  //   void listen_for_connections();
};

TcpServerAgent::TcpServerAgent(int port) {
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = INADDR_ANY;
}

int TcpServerAgent::initialize_socket() {
  std::cout << "initializing socket..." << '\n';
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  int opt = 1;

  // Forcefully attaching socket to the port
  if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    std::cout << "failed to attach port to socket" << '\n';
    return EXIT_FAILURE;
  }
  std::cout << "socket attached to port: " << server_address.sin_port << '\n';

  // Bind socket to server_address
  if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
    std::cout << "failed to bind socket" << '\n';
    return EXIT_FAILURE;
  }
  std::cout << "socket initialized" << '\n';

  return 0;
}

// void TcpServerAgent::listen_for_connections() {
//   listening_thread = std::thread(&TcpServerAgent::listening_task);
// }

// void TcpServerAgent::listening_task() {
// }

TcpServerAgent::~TcpServerAgent() {

  if (listening_thread.joinable()) {
    listening_thread.join();
  }

  close(socket_fd);
}