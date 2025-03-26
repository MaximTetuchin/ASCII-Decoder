#ifndef ASCII85_H
#define ASCII85_H

#include <vector>
#include <string>
#include <iostream>

class ASCII85 {
public:
    // Encoding
    static std::string encode(const std::vector<unsigned char>& bytes);
    static void runEncoder();

    // Decoding
    static std::vector<unsigned char> decode(const std::string& input);
    static void runDecoder();

private:
    static const std::string ascii85_chars;
    
    static char toAscii85Char(int value);
    static int fromAscii85Char(char c);
    
    static std::vector<unsigned char> parseHexInput(const std::string& input);
    static std::string cleanInput(const std::string& input);
};

#endif // ASCII85_H