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
    print();
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
        case 127:
        case KEY_BACKSPACE:
          if( x == 0 && y > 0){
            x = lines[y - 1].length();
            lines[y - 1] += lines[y];
            m_remove(y);
            // up();
          }else if(x > 0){
            lines[y].erase(--x, 1);
          }
          break;
        default:
          lines[y].insert(x, 1, c);
          x++;
          break;
      }
      break;
  }
}

void VimPress::print(){
  for (size_t i {}; i < (size_t)LINES - 1; ++i) {
    if(i >= lines.size()){
      move(i, 0);
      clrtoeol();
    } else {
      mvprintw(i, 0, lines[i].c_str());
    }
    clrtoeol();
  }
  // Move the cursor to the current position
  // move(row, column)
  move(y, x);
}

void VimPress::m_remove(int number){
  lines.erase(lines.begin() + number);
}

std::string VimPress::m_tabs(std::string& line){
  std::size_t tab = line.find('\t');
  return tab == line.npos ? line : m_tabs(line.replace(tab, 1, "  "));
}

void VimPress::m_insert(std::string line, int number){
  line = m_tabs(line);
  lines.insert(lines.begin() + number, line);
}

void VimPress::m_append(std::string& line){
  line = m_tabs(line);
  lines.push_back(line);
}
