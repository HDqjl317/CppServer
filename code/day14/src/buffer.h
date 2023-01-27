#ifndef BUFFER_H
#define BUFFER_H

#include "common.h"
#include <string>
#include <memory>


class Buffer {

public:
    Buffer();
    ~Buffer();
    DISALLOW_COPY_AND_MOVE(Buffer);

    const std::string &buf() const;
    const char* c_str() const;
    void set_buf(const char*);

    void append(const char* _str, int _size);
    size_t size() const;
    void clear();

private:
    std::string buf_;
};

#endif