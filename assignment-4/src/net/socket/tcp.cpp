#include "net/socket/tcp.h"
#include "net/utils/utils.h"

#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

unsigned TCPSocket::child_count = 0;

void TCPSocket::child_handler(int signum) {
  pid_t process_id;  // Child process

  while (child_count)  // Clean up all zombies
  {
    process_id = waitpid((pid_t)-1, NULL, WNOHANG); /* Non-blocking wait */
    switch (process_id) {
      case -1:  // Error
        die(errno, "waitpid() failed");
        break;
      case 0:  // Done
        goto stop;
        break;
      default:  // Cleaned up; reduce the child counter
        child_count--;
        break;
    }
  }
stop:
  return;
}

TCPSocket::TCPSocket(InetAddress &addr) : Socket(addr, SOCK_STREAM) {
  struct sigaction cleanup;
  cleanup.sa_handler = TCPSocket::child_handler;
  if (sigfillset(&cleanup.sa_mask) < 0)  // mask all signals
    die(errno, "sigfillset() failed");
  cleanup.sa_flags = SA_RESTART;

  // Set signal disposition for child-termination signals
  if (sigaction(SIGCHLD, &cleanup, 0) < 0) die(errno, "sigaction() failed");
}

TCPSocket::~TCPSocket() {}

void TCPSocket::connect() {
  int return_val = alias_connect(this->socket_fd, this->addr.get_addr(),
                                 this->addr.get_size());
  if (return_val < 0) die(return_val, "Can't connect");
}

void TCPSocket::listen_and_serve(int max_clients,
                                 void (*handler)(TCPSocket *socket,
                                                 int client_socket_fd)) {
  // Open the socket to listen to incoming clients
  int return_val = alias_listen(this->socket_fd, max_clients);
  if (return_val < 0) die(return_val, "Can't listen");

  for (;;) {
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // A blocking I/O to wait a client to be connected first
    int client_socket_fd = alias_accept(
        this->socket_fd, (struct sockaddr *)&client_addr, &client_addr_len);

    if (client_socket_fd == -1)
      die(-1, "Invalid client socket file descriptor");

    // Handle concurrently
    switch (fork()) {
      case -1:
        // Error occured
        die(errno, "Error in creating a child process");
        break;
      case 0:
        // Close parent's server socket fd
        alias_close(this->socket_fd);

        // Handler will take the course
        handler(this, client_socket_fd);
        exit(0);
        break;
    }
    alias_close(client_socket_fd);
  }
}

void TCPSocket::serve(
    std::function<void(TCPSocket *socket, int server_socket_fd)> handler) {
  handler(this, this->socket_fd);
}

void TCPSocket::send(int to_fd, Message *msg) {
  char buf[1024] = {0};
  ssize_t len;

  // Send chunk by chunk
  while ((len = msg->read(buf, sizeof(buf))) > 0)
    alias_send(to_fd, buf, len, 0);
}

Message *TCPSocket::recv() {
  char buf[1024] = {0};
  ssize_t len;
  Message *msg = new Message();

  // Receive chunk by chunk
  while ((len = alias_recv(this->socket_fd, buf, sizeof(buf), 0)) > 0)
    msg->write(buf, len);

  return msg;
}
