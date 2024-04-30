#include "vimpress.hpp"

int main(int argc, char **argv) {
    if (argc > 1) {
        auto vimpress = std::make_shared<VimPress>(argv[1]);
        vimpress -> run();
    } else {
        auto vimpress = std::make_shared<VimPress>("");
        vimpress -> run();
    }
    return 0;
}
