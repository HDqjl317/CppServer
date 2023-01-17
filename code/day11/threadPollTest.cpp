#include <iostream>
#include <string>
#include "src/threadPoll.h"

void print(int a, double b, const char *c, std::string d) {
    std::cout << a << b << c << d << std::endl;
}

void test() {
    std::cout << "help" << std::endl;
}

int main() {
    ThreadPoll * poll = new ThreadPoll();
    std::function<void()> func = std::bind(print, 1, 3.14, "hello", std::string("world"));
    poll->add(func);
    func = test;
    poll->add(func);
    delete poll;
    return 0;
}