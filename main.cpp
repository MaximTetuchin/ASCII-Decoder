#include "ascii85.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    bool adobe_mode = false;
    std::string mode;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-e" || arg == "-d") {
            mode = arg;
        } else if (arg == "--adobe") {
            adobe_mode = true;
        } else {
            std::cerr << "Invalid option: " << arg << "\n";
            return 1;
        }
    }

    if (mode.empty()) {
        std::cerr << "Usage: " << argv[0] << " [-e|-d] [--adobe]\n";
        return 1;
    }

    if (mode == "-e") {
        ASCII85::runEncoder(adobe_mode);
    } else if (mode == "-d") {
        ASCII85::runDecoder(adobe_mode);
    }

    return 0;
}