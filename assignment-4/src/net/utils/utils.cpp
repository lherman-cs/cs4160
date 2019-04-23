#include "net/utils/utils.h"

#include <errno.h>
#include <sys/time.h>
#include <cstdlib>
#include <iostream>

void die(int err_code, std::string err_msg) {
  std::cerr << "Error(" << err_code << "): " << err_msg << std::endl;
  exit(err_code);
}

void log_msg(std::string name, std::string msg) {
  std::cerr << name << ": " << msg << std::endl;
}

double timestamp() {
  struct timeval tv;
  if (gettimeofday(&tv, NULL) < 0) {
    die(errno, "utils:timestampt: gettimeofday failed");
  }
  return ((double)tv.tv_sec + ((double)tv.tv_usec / 1000000));
}