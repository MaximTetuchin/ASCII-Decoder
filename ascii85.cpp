#include "ascii85.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

const std::string ASCII85::ascii85_chars = 
    "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstu";

char ASCII85::toAscii85Char(int value) {
    if (value < 0 || value >= 85) throw std::out_of_range("Value must be between 0 and 84");
    return ascii85_chars[value];
}

int ASCII85::fromAscii85Char(char c) {
    if (c < '!' || c > 'u') throw std::out_of_range("Invalid ASCII85 character");
    return c - '!';
}

std::string ASCII85::encode(const std::vector<unsigned char>& bytes, bool adobe_mode) {
    std::string result;
    size_t original_size = bytes.size();
    size_t padding = (4 - (original_size % 4)) % 4;
    std::vector<unsigned char> padded_bytes(bytes);
    
    for (size_t i = 0; i < padding; ++i) {
        padded_bytes.push_back(0x00);
    }

    for (size_t i = 0; i < padded_bytes.size(); i += 4) {
        int N = (padded_bytes[i] << 24) | 
                    (padded_bytes[i+1] << 16) | 
                    (padded_bytes[i+2] << 8) | 
                    padded_bytes[i+3];

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
    
    return adobe_mode ? "<~" + result + "~>" : result;
}

std::vector<unsigned char> ASCII85::decode(const std::string& input, bool adobe_mode) {
    std::string clean_input = cleanInput(input, adobe_mode);
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

std::string ASCII85::cleanInput(const std::string& input, bool adobe_mode) {
    if (!adobe_mode) return input;
    
    std::string clean = input;
    size_t start = clean.find("<~");
    if (start != std::string::npos) {
        clean = clean.substr(start + 2);
    }
    size_t end = clean.find("~>");
    if (end != std::string::npos) {
        clean = clean.substr(0, end);
    }
    return clean;
}

void ASCII85::runEncoder(bool adobe_mode) {
    std::vector<unsigned char> input(
        std::istreambuf_iterator<char>(std::cin),
        std::istreambuf_iterator<char>()
    );
    std::cout << encode(input, adobe_mode);
}

void ASCII85::runDecoder(bool adobe_mode) {
    std::string input(
        std::istreambuf_iterator<char>(std::cin),
        std::istreambuf_iterator<char>()
    );
    auto bytes = decode(input, adobe_mode);
    std::cout.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
}