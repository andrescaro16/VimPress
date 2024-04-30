#pragma once
// #pragma clang ignore "-Wformat-security"

#include <ncurses.h>
#include <memory>
#include <string>
#include <vector>

class VimPress {
  int x, y;
  char mode;
  std::string filename, status;
  std::vector<std::string> lines;

  public:
    VimPress(const std::string &);
    ~VimPress();
    void run();

  protected:
    void update();
    void statusline();
    void input(int);
    void print();
};
