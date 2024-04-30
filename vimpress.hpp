#pragma once
// #pragma clang ignore "-Wformat-security"

#include <ncurses.h>
#include <memory>
#include <string>
#include <vector>

class VimPress {
  std::string filename;
  std::vector<std::string> lines;

  public:
    VimPress(const std::string &);
    ~VimPress();
    void run();
};
