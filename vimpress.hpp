#pragma once
// #pragma clang ignore "-Wformat-security"

#include <ncurses.h>
#include <memory>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include "huffman.hpp"

class VimPress {
  size_t x, y;
  char mode;
  std::string filename, status, section, openWithDecompress;
  std::vector<std::string> lines;
  HuffmanCoding& huffman;
  const std::string CUSTOM_EXTENSION = ".caju";


  // Remove character
  void m_remove(int);
  // Manipulate m_tabs to remove tabs
  std::string m_tabs(std::string&);
  // Insert in the middle
  void m_insert(std::string, int);
  // Add to end of line
  void m_append(std::string&);

  public:
    VimPress(const std::string &, HuffmanCoding&, const std::string& = "");
    ~VimPress();
    void run();

  protected:
    // Update the screen (handle status)
    void update();
    void statusline();
    
    // Handle modes and input
    void input(int);
    
    // Print the lines
    void print();
    
    // Move the cursor
    void up();
    void left();
    void right();
    void down();

    // File operations
    void open();
    void saveWithCompress();
    void saveWithoutCompress();
};
