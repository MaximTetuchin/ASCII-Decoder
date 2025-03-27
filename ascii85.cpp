#include "ascii85.h"
#include <sstream>

const std::string ASCII85::ascii85_chars = 
    "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstu";

// Helper functions
char ASCII85::toAscii85Char(int value) {
    if (value < 0 || value >= 85) throw std::out_of_range("Value must be between 0 and 84");
    return ascii85_chars[value];
}

int ASCII85::fromAscii85Char(char c) {
    if (c < '!' || c > 'u') throw std::out_of_range("Invalid ASCII85 character");
    return c - '!';
}

// Encoding implementation
std::string ASCII85::encode(const std::vector<unsigned char>& bytes) {
    std::string result;
    size_t original_size = bytes.size();
    size_t padding = (4 - (original_size % 4)) % 4;
    std::vector<unsigned char> padded_bytes(bytes);
    
    for (size_t i = 0; i < padding; ++i) {
        padded_bytes.push_back(0x00);
    }

    for (size_t i = 0; i < padded_bytes.size(); i += 4) {
        int N = (padded_bytes[i] << 24) | (padded_bytes[i+1] << 16) | (padded_bytes[i+2] << 8) | padded_bytes[i+3];

        if (N == 0) {
            result += 'z';
            continue;
        }

        char encoded[5];
        for (int j = 4; j >= 0; --j) {
            encoded[j] = toAscii85Char(N % 85);
            N /= 85;
        }
        result.append(encoded, 5);
    }

    if (padding > 0) {
        result.resize(result.size() - padding);
    }
    return "<~" + result + "~>";
}

// Decoding implementation
std::vector<unsigned char> ASCII85::decode(const std::string& input) {
    std::string clean_input = cleanInput(input);
    std::vector<unsigned char> decoded_bytes;

    size_t total_clean_size = clean_input.size();
    size_t pos = 0;

    while (pos < total_clean_size) {
        if (clean_input[pos] == 'z') {
            decoded_bytes.insert(decoded_bytes.end(), 4, 0x00);
            pos++;
            continue;
        }

        size_t original_group_size = std::min<size_t>(5, total_clean_size - pos);
        std::string group;

        if (original_group_size < 5) {
            group = clean_input.substr(pos, original_group_size);
            group.append(5 - original_group_size, 'u');
        } else {
            group = clean_input.substr(pos, 5);
        }

        int N = 0;
        for (size_t i = 0; i < 5; ++i) {
            char c = group[i];
            N = N * 85 + fromAscii85Char(c);
        }

        decoded_bytes.push_back((N >> 24) & 0xFF);
        decoded_bytes.push_back((N >> 16) & 0xFF);
        decoded_bytes.push_back((N >> 8) & 0xFF);
        decoded_bytes.push_back(N & 0xFF);

        if (original_group_size < 5) {
            size_t bytes_to_keep = original_group_size - 1;
            decoded_bytes.resize(decoded_bytes.size() - (4 - bytes_to_keep));
        }

        pos += original_group_size;
    }

    return decoded_bytes;
}

// I/O functions
void ASCII85::runEncoder() {
    std::string byte_input;
    std::cout << "Enter hex bytes (or '/exit' to quit):\n";
    while (std::getline(std::cin, byte_input)) {
        if (byte_input == "/exit") break;
        try {
            auto bytes = parseHexInput(byte_input);
            std::cout << encode(bytes) << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}

void ASCII85::runDecoder() {
    std::string input;
    std::cout << "Enter ASCII85 string (or '/exit' to quit):\n";
    while (std::getline(std::cin, input)) {
        if (input == "/exit") break;
        try {
            auto bytes = decode(input);
            for (unsigned char byte : bytes) {
                std::cout << std::hex << (int)byte << " ";
            }
            std::cout << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}

// Utility functions
std::vector<unsigned char> ASCII85::parseHexInput(const std::string& input) {
    std::vector<unsigned char> bytes;
    std::istringstream stream(input);
    unsigned int value;
    while (stream >> std::hex >> value) {
        if (value > 0xFF) throw std::out_of_range("Byte value out of range");
        bytes.push_back(static_cast<unsigned char>(value));
    }
    return bytes;
}

std::string ASCII85::cleanInput(const std::string& input) {
    std::string clean = input;
    if (clean.substr(0, 2) == "<~") clean = clean.substr(2);
    if (clean.size() >= 2 && clean.substr(clean.size() - 2) == "~>") {
        clean = clean.substr(0, clean.size() - 2);
    }
    return clean;
}