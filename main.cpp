#include <iostream>
#include <string>
#include <vector>
#include <sstream>

// ASCII85 character set from '!' (33) to 'u' (117)
const std::string ascii85_chars = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstu";

// Convert an integer value to an ASCII85 character
char toAscii85Char(int value) {
    if (value < 0 || value >= 85) {
        throw std::out_of_range("Value must be between 0 and 84");
    }
    return ascii85_chars[value];
}

// Convert an ASCII85 character to its integer value
int fromAscii85Char(char c) {
    if (c < '!' || c > 'u') {
        throw std::out_of_range("Invalid ASCII85 character");
    }
    return c - '!';
}

// Encoding function
void Encoder() {
    std::string byte_input;
    std::vector<unsigned char> bytes;
    std::string ascii85_result;

    std::cout << "Enter hex bytes (or '/exit' to quit):\n";
    while (std::getline(std::cin, byte_input)) {
        if (byte_input == "/exit") break;

        // Parse hex bytes
        std::istringstream stream(byte_input);
        unsigned int value;
        while (stream >> std::hex >> value) {
            if (value > 0xFF) {
                std::cerr << "Invalid byte value: " << value << std::endl;
                continue;
            }
            bytes.push_back(static_cast<unsigned char>(value));
        }

        // Process in 4-byte groups
        size_t original_size = bytes.size();
        size_t padding = (4 - (original_size % 4)) % 4;
        for (size_t i = 0; i < padding; ++i) {
            bytes.push_back(0x00);
        }

        for (size_t i = 0; i < bytes.size(); i += 4) {
            int N = (bytes[i] << 24) | (bytes[i+1] << 16) | (bytes[i+2] << 8) | bytes[i+3];

            // Special case: four zero bytes
            if (N == 0) {
                ascii85_result += 'z';
                continue;
            }

            // Convert to base85
            char encoded[5];
            for (int j = 4; j >= 0; --j) {
                encoded[j] = toAscii85Char(N % 85);
                N /= 85;
            }
            ascii85_result.append(encoded, 5);
        }

        // Remove padding characters
        if (padding > 0) {
            ascii85_result.resize(ascii85_result.size() - padding);
        }

        std::cout << "<~" << ascii85_result << "~>\n";

        // Clear buffers for next input
        ascii85_result.clear();
        bytes.clear();
    }
}

// Decoding function
void Decoder() {
    std::string ASCII_input;
    std::vector<unsigned char> decoded_bytes;

    std::cout << "Enter ASCII85 string (or '/exit' to quit):\n";
    while (std::getline(std::cin, ASCII_input)) {
        if (ASCII_input == "/exit") break;

        // Remove whitespace and delimiters
        if (ASCII_input.substr(0, 2) == "<~") {
            ASCII_input = ASCII_input.substr(2);
        }
        if (ASCII_input.size() >= 2 && ASCII_input.substr(ASCII_input.size() - 2) == "~>") {
            ASCII_input = ASCII_input.substr(0, ASCII_input.size() - 2);
        }

        // Process each group
        size_t pos = 0;
        while (pos < ASCII_input.size()) {
            if (ASCII_input[pos] == 'z') {
                // Special case: four zero bytes
                decoded_bytes.insert(decoded_bytes.end(), 4, 0x00);
                pos++;
                continue;
            }

            // Read 5 characters (or remaining)
            size_t group_size = std::min<size_t>(5, ASCII_input.size() - pos);
            if (group_size < 5) {
                std::cerr << "Warning: Incomplete group at end (padding with 'u')\n";
                ASCII_input.append(5 - group_size, 'u');
                group_size = 5;
            }

            // Convert to 32-bit value
            int N = 0;
            for (size_t i = 0; i < 5; ++i) {
                char c = ASCII_input[pos + i];
                if (c < '!' || c > 'u') {
                    std::cerr << "Invalid ASCII85 character: " << c << std::endl;
                    return;
                }
                N = N * 85 + fromAscii85Char(c);
            }

            // Extract 4 bytes
            decoded_bytes.push_back((N >> 24) & 0xFF);
            decoded_bytes.push_back((N >> 16) & 0xFF);
            decoded_bytes.push_back((N >> 8) & 0xFF);
            decoded_bytes.push_back(N & 0xFF);

            pos += 5;
        }

        // Remove padding bytes (if any)
        size_t padding = (4 - (decoded_bytes.size() % 4)) % 4;
        if (padding > 0 && !decoded_bytes.empty()) {
            decoded_bytes.resize(decoded_bytes.size() - padding);
        }

        // Output as hex
        for (unsigned char byte : decoded_bytes) {
            std::cout << std::hex << (byte & 0xFF) << " ";
        }
        std::cout << "\n";

        decoded_bytes.clear();
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [-e|-d]\n";
        std::cerr << "  -e : Encode to ASCII85\n";
        std::cerr << "  -d : Decode from ASCII85\n";
        return 1;
    }

    std::string mode(argv[1]);
    if (mode == "-e") {
        Encoder();
    } else if (mode == "-d") {
        Decoder();
    } else {
        std::cerr << "Invalid option. Use -e for encoding or -d for decoding.\n";
        return 1;
    }

    return 0;
}