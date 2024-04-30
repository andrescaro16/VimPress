#include "vimpress.hpp"

VimPress::VimPress(const std::string &file) {

  lines.push_back("");

  if (file.empty()) {
    filename = "untitled";
  } else {
    filename = file;
  }

  initscr();
  noecho();
  cbreak();
  keypad(stdscr, true);
}

VimPress::~VimPress() {
  refresh();
  endwin();
}

void VimPress::run() {
  // printw(&filename[0]);
  // getch();
  while(getmaxyx(stdscr, LINES, COLS)){
    int ch = getch();
    std::string s(1, (char)ch);
    lines.push_back(s);
    for (size_t i {}; i < lines.size(); ++i) {
      // mvprintw(0, i, lines[i].c_str());
      move(0, i);
      addstr(lines[i].c_str());
    }
  }
}
