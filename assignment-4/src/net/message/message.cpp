#include "net/message/message.h"
#include <algorithm>
#include <cstring>

Message::Message() {
  this->len = 0;
  this->cap = DEFAULT_INIT_CAP;
  this->ptr = 0;
  this->buf = new char[this->cap];
}

Message::Message(int init_cap) {
  this->len = 0;
  this->cap = init_cap;
  this->ptr = 0;
  this->buf = new char[this->cap];
}

Message::~Message() { delete[] this->buf; }

void Message::grow() {
  // Make twice size of the old buf and copy the old data from the old buf
  char *new_buf = new char[this->cap << 1];
  memcpy(new_buf, this->buf, this->cap);

  this->cap <<= 1;
  delete[] this->buf;
  this->buf = new_buf;
}

void Message::shrink() {
  // Assumed that ptr >= half size of the cap. This function will NOT actually
  // shrink the array. But, instead, move the second half elements to the first
  // half elements. Essentially, the previous first half elements will be
  // replaced.

  int half_size = this->cap >> 1;
  memcpy(this->buf, this->buf + half_size, half_size);
  this->ptr -= half_size;
}

int Message::write(const void *buf, int size) {
  int space = this->cap - this->len;
  // Make sure there is enough space to write
  while (space < (size << 1)) {
    grow();
    space = this->cap - this->len;
  }

  memcpy(this->buf + this->len, buf, size);
  this->len += size;
  return size;
}

int Message::read(void *buf, int size) {
  // Make sure if the size is not over the len
  size = std::min(this->len, size);
  memcpy(buf, this->buf + ptr, size);

  // Update properties accordingly
  this->ptr += size;
  this->len -= size;

  // Save memory by reusing the used block of bytes
  while (this->ptr >= (this->cap >> 1)) shrink();

  // Return zero if there is nothing left to read
  return size;
}

char *Message::get_buf() { return this->buf + this->ptr; }

int Message::get_len() { return this->len; }