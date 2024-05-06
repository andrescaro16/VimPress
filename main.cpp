#include "vimpress.hpp"

int main(int argc, char **argv) {
    HuffmanCoding huffman;
    if (argc > 1) {
        const char* secondArg = argv[2];
        std::string secondArgValue = (secondArg != nullptr) ? secondArg : "";
        auto vimpress = std::make_shared<VimPress>(argv[1], huffman, secondArgValue);
        vimpress -> run();
    } else {
        auto vimpress = std::make_shared<VimPress>("", huffman);
        vimpress -> run();
    }
    return 0;
}
