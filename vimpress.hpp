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

  // Remove character
  void m_remove(int);
  // Manipulate m_tabs to remove tabs
  std::string m_tabs(std::string&);
  // Insert in the middle
  void m_insert(std::string, int);
  // Add to end of line
  void m_append(std::string&);

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
