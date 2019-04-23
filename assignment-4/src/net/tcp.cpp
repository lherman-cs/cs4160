#include "net/tcp.h"
#include "net/message/message.h"

static const int MAXBYTES = 1024;

TCP::TCP() { sock.connect(); }
TCP::~TCP() { sock.close(); }

void read_handler(TCPSocket *socket, int server_socket_fd) {}
void write_handler(TCPSocket *socket, int server_socket_fd) {}

char *TCP::read() {
  char buf[MAXBYTES] = {0};
  char *bufPtr = buf;
  auto handler = [bufPtr](TCPSocket *socket, int server_socket_fd) {
    Message *msg = socket->recv();
    while (msg->read(bufPtr, MAXBYTES) > 0) std::cerr << bufPtr;
    delete msg;
  };
  sock.serve(handler);
  return buf;
}

void TCP::write(const char *msg) {
  if (strlen(msg) > (MAXBYTES - 1)) {
    std::cerr << "Message is too long to send" << std::endl;
    return;
  }
  auto handler = [&msg](TCPSocket *socket, int server_socket_fd) {
    Message m;
    m.write((void *)msg, strlen(msg) + 1);
    socket->send(server_socket_fd, &m);
  };
  sock.serve(handler);
}