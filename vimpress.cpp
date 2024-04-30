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

  // Initialize the section for the status line
  section = {};

  // Open the file
  open();

  // Initialize ncurses
  initscr();
  noecho();
  cbreak();
  keypad(stdscr, true);
  use_default_colors();
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
  section = " COLS: " + std::to_string(x) + " | ROWS: " + std::to_string(y) + " | FILE: " + filename + " ";
  status.insert(0, " "); // initial SPACE for `status`
  // status.insert(status.length(), " "); // final SPACE for `status`
}

void VimPress::statusline() {

  start_color();
  init_color(COLOR_BLACK, 0, 0, 0);
  init_color(COLOR_GREEN, 0, 500, 0);
  if (mode == 'n') {
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
  } else {
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
  }

  attron(A_REVERSE);
  attron(A_BOLD);
  attron(COLOR_PAIR(1));

  for (int i {}; i < COLS; ++i) {
    mvprintw(LINES - 1, i, " "); 
  }
  mvprintw( LINES - 1, COLS - section.length(), &section[0]);
  mvprintw(LINES - 1, 0, status.c_str());

  attroff(COLOR_PAIR(1));
  attroff(A_BOLD);
  attroff(A_REVERSE);
}

void VimPress::input(int c) {

  switch (c){
    case KEY_UP:
      up();
      return;
    case KEY_LEFT:
      left();
      return;
    case KEY_RIGHT:
      right();
      return;
    case KEY_DOWN:
      down();
      return;
  }

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
          if ( x == 0 && y > 0) {
            x = lines[y - 1].length();
            lines[y - 1] += lines[y];
            m_remove(y);
            up();
          } else if (x > 0) {
            lines[y].erase(--x, 1);
          }
          break;
        case KEY_DC:
          if ( x == lines[y].length() && y != lines.size() - 1) {
            lines[y] += lines[y + 1];
          } else {
            lines[y].erase(x, 1);
          }
          break;
        case KEY_ENTER:
        case 10:
          if (x < lines[y].length()) {
            m_insert(lines[y].substr(x, lines[y].length() - x), y + 1); 
            lines[y].erase(x, lines[y].length() - x);
          } else {
            m_insert("", y + 1);
          }
          x = 0;
          down();
          break;
        case KEY_BTAB:
        case KEY_CTAB:
        case KEY_STAB:
        case KEY_CATAB:
        case 9:
          lines[y].insert(x, 2, ' ');
          x += 2;
          break;
        default:
          lines[y].insert(x, 1, c);
          x++;
          break;
      }
      break;
  }
}

void VimPress::print() {
  for (size_t i {}; i < (size_t)LINES - 1; ++i) {
    if (i >= lines.size()) {
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

void VimPress::m_remove(int number) {
  lines.erase(lines.begin() + number);
}

std::string VimPress::m_tabs(std::string& line) {
  std::size_t tab = line.find('\t');
  return tab == line.npos ? line : m_tabs(line.replace(tab, 1, "  "));
}

void VimPress::m_insert(std::string line, int number) {
  line = m_tabs(line);
  lines.insert(lines.begin() + number, line);
}

void VimPress::m_append(std::string& line) {
  line = m_tabs(line);
  lines.push_back(line);
}

void VimPress::up() {
  if (y > 0) {
    --y;
  }
  if ( x >= lines[y].length()) {
    x = lines[y].length();
  }
  move(y, x);
}

void VimPress::left() {
  if (x > 0) {
    --x;
    move(y, x);
  }
}

void VimPress::right() {
  if ( (int)x <= COLS && x <= lines[y].length() - 1) {
    ++x;
    move(y, x);
  }
}

void VimPress::down() {
  if ( (int)y < LINES && y < lines.size() - 1) {
    ++y;
  }
  if ( x >= lines[y].length()) {
    x = lines[y].length();
  }
  move(y, x);
}

void VimPress::open() {
  if (std::filesystem::exists(filename)) {
    std::ifstream ifile(filename);
    if (ifile.is_open()) {
      while (!ifile.eof()) {
        std::string buffer;
        std::getline(ifile, buffer);
        m_append(buffer);
      }
    } else {
      throw std::runtime_error("Cannot open file. Permission denied.");
    }
  } else {
    std::string str {};
    m_append(str);
  }
}
