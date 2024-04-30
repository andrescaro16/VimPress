#include "vimpress.hpp"

VimPress::VimPress(const std::string &file) {
  // We avoid a segmentation fault by adding an empty string to the vector
  lines.push_back("");

  // Initialize the cursor position and mode
  x = y = 0;
  mode = 'n';
  status = "NORMAL";

  // Default filename if no file is provided
  if (file.empty()) {
    filename = "untitled";
  } else {
    filename = file;
  }

  // Initialize ncurses
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
  while(mode != 'q'){
    update();
    statusline();
    int c = getch();
    input(c);
  }
}

void VimPress::update() {
  switch (mode){
    case 27:
    case 'n':
      status = "NORMAL";
      break;
    case 'i':
      status = "INSERT";
      break;
    case 'q':
      break;
  }
}

void VimPress::statusline() {
  attron(A_REVERSE);
  mvprintw(LINES - 1, 0, status.c_str());
  // move(LINES - 1, 0);
  // addstr(status.c_str());
  attroff(A_REVERSE);
}

void VimPress::input(int c){
  switch (mode){
    case 27:
    case 'n':
      switch (c){
        case 'q':
          mode = 'q';
          break;
        case 'i':
          mode = 'i';
          break;
        case 'w':
          mode = 'w';
          break;
      }
      break;
    case 'i':
      switch (c){
        case 27:
          mode = 'n';
          break;
        default:
          std::string s(1, c);
          lines.push_back(s);
          break;
      }
      break;
  }
  for (size_t i {}; i < lines.size(); ++i) {
    mvprintw(0, i, lines[i].c_str()); 
    // move(i, 0);
    // addstr(lines[i].c_str());
  }
}
