#pragma once
#include <vector>
#include <string>
#include <stdexcept>

class ASCII85 {
public:
    static void runEncoder(bool adobe_mode);
    static void runDecoder(bool adobe_mode);
    
    static std::string encode(const std::vector<unsigned char>& bytes, bool adobe_mode);
    static std::vector<unsigned char> decode(const std::string& input, bool adobe_mode);

private:
    static const std::string ascii85_chars;
    static char toAscii85Char(int value);
    static int fromAscii85Char(char c);
    static std::vector<unsigned char> parseHexInput(const std::string& input);
    static std::string cleanInput(const std::string& input, bool adobe_mode);
};