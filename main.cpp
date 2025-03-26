#include "ascii85.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [-e|-d]\n";
        std::cerr << "  -e : Encode to ASCII85\n";
        std::cerr << "  -d : Decode from ASCII85\n";
        return 1;
    }

    std::string mode(argv[1]);
    if (mode == "-e") {
        ASCII85::runEncoder();
    } else if (mode == "-d") {
        ASCII85::runDecoder();
    } else {
        std::cerr << "Invalid option. Use -e for encoding or -d for decoding.\n";
        return 1;
    }

    return 0;
}