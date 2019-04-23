#include "net/socket/udp.h"
#include <unistd.h>
#include <cstring>
#include "net/utils/utils.h"

UDPSocket::UDPSocket(InetAddress &addr) : Socket(addr, SOCK_DGRAM) {}

UDPSocket::~UDPSocket() {}

void UDPSocket::listen_and_serve(void (*handler)(UDPSocket *socket,
                                                 InetAddress client_addr)) {
  // Wait a client to come
  for (;;) {
    struct sockaddr_storage addr;
    socklen_t addr_len;

    char buf[1024] = {0};

    // Receive some data to initiate the connection
    if (recvfrom(this->socket_fd, buf, sizeof(buf), 0, (struct sockaddr *)&addr,
                 &addr_len) < 0)
      // An error occured
      die(-1, "Failed to receive a client");

    InetAddress client_addr((struct sockaddr *)&addr, addr_len);
    handler(this, client_addr);
  }
}

void UDPSocket::serve(void (*handler)(UDPSocket *socket,
                                      InetAddress server_addr)) {
  handler(this, this->addr);
}

void UDPSocket::send(InetAddress *to_addr, Message *msg) {
  int rc = sendto(this->socket_fd, msg->get_buf(), msg->get_len(), 0,
                  to_addr->get_addr(), to_addr->get_size());

  if (rc < 0) die(rc, "send failed");
}

Message *UDPSocket::recv() {
  // Initialize variables
  struct sockaddr_storage addr;
  memset(&addr, 0, sizeof(addr));
  socklen_t addr_len = 0;
  char buf[64] = {0};
  ssize_t len = 0;

  // Peek the data size
  len = recvfrom(this->socket_fd, buf, sizeof(buf), MSG_PEEK,
                 (struct sockaddr *)&addr, &addr_len);

  // Initialize a big enough buffer to get the whole data
  char *data = new char[len];
  len = recvfrom(this->socket_fd, data, len, 0, (struct sockaddr *)&addr,
                 &addr_len);

  if (len < 0) die(-1, "recv failed");

  // Write the data into the message object to be read after
  Message *msg = new Message();
  msg->write(data, len);

  // Clean up
  delete (data);

  return msg;
}