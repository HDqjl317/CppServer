#include "buffer.h"
#include <string.h>
#include <iostream>

Buffer::Buffer() {}

Buffer::~Buffer() {}


const std::string &Buffer::buf() const { return buf_; }

const char *Buffer::c_str() const { return buf_.c_str(); }

void Buffer::set_buf(const char *buf) {
  std::string new_buf(buf);
  buf_.swap(new_buf);
}

size_t Buffer::size() const { return buf_.size(); }

void Buffer::append(const char *str, int size) {
  for (int i = 0; i < size; ++i) {
    if (str[i] == '\0') {
      break;
    }
    buf_.push_back(str[i]);
  }
}

void Buffer::clear() { buf_.clear(); }