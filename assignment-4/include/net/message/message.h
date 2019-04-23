#pragma once

class Message {
 private:
  const static int DEFAULT_INIT_CAP = 1024;
  int len;
  int cap;
  int ptr;
  char *buf;
  void grow();
  void shrink();

 public:
  Message();
  Message(int init_cap);
  ~Message();

  int write(const void *buf, int size);
  int read(void *buf, int size);
  char *get_buf();
  int get_len();
};