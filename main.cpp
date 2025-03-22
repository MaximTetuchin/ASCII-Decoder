#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

const std::string ascii85_chars = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz";

// Helper function to convert a value to the corresponding ASCII85 character
char toAscii85Char(int value) {
    if (value < 0 || value >= 85) {
        throw std::out_of_range("Value must be between 0 and 84");
    }
    return ascii85_chars[value];
}

// Encoder function
void Encoder() {
    std::string byte_input;
    std::vector<unsigned char> bytes;
    std::string ascii85_result;
    int extra_bytes = 0;

    // Read bytes from STDIN
    std::cout << "Enter hex bytes (or '/exit' to quit):\n";
    while (std::getline(std::cin, byte_input)) {
        if (byte_input == "/exit") break;  // Exit if user enters "/exit"
        
        std::istringstream stream(byte_input);
        int value;
        while (stream >> std::hex >> value) {
            bytes.push_back(static_cast<unsigned char>(value));
        }

        // Padding to make the byte length a multiple of 4
        while (bytes.size() % 4 != 0) {
            bytes.push_back(static_cast<unsigned char>(0x00));
            extra_bytes++;
        }

        // Convert bytes to ASCII85
        for (size_t i = 0; i < bytes.size() / 4; i++) {
            unsigned char num1_16 = bytes[i * 4 + 0];
            unsigned char num2_16 = bytes[i * 4 + 1];
            unsigned char num3_16 = bytes[i * 4 + 2];
            unsigned char num4_16 = bytes[i * 4 + 3];

            int N = (num1_16 << 24) | (num2_16 << 16) | (num3_16 << 8) | num4_16;
            if (N == 0) {
                ascii85_result += 'z'; // Special case for four zero bytes
            } else {
                int l5 = N % 85; N /= 85;
                int l4 = N % 85; N /= 85;
                int l3 = N % 85; N /= 85;
                int l2 = N % 85; N /= 85;
                int l1 = N % 85;

                ascii85_result += toAscii85Char(l1);
                ascii85_result += toAscii85Char(l2);
                ascii85_result += toAscii85Char(l3);
                ascii85_result += toAscii85Char(l4);
                ascii85_result += toAscii85Char(l5);
            }
        }

        // Trim extra bytes from the result
        ascii85_result.resize(ascii85_result.size() - extra_bytes);
        std::cout << "<~" << ascii85_result << "~>\n";
        ascii85_result.clear();  // Clear the result buffer for next input
        bytes.clear();  // Clear the byte buffer for next input
    }
}

// Decoder function
void Decoder() {
    std::string ASCII_input;
    std::vector<char> all_values_to_decode;
    int extra_symbols = 0;

    // Read ASCII85 input from STDIN
    std::cout << "Enter ASCII85 string (or '/exit' to quit):\n";
    while (std::getline(std::cin, ASCII_input)) {
        if (ASCII_input == "/exit") break;  // Exit if user enters "/exit"
        
        if (ASCII_input.size() >= 2 && ASCII_input.substr(0, 2) == "<~") {
            ASCII_input = ASCII_input.substr(2); // Remove "<~" from the beginning
        }
        if (ASCII_input.size() >= 2 && ASCII_input.substr(ASCII_input.size() - 2) == "~>") {
            ASCII_input = ASCII_input.substr(0, ASCII_input.size() - 2); // Remove "~>" from the end
        }

        for (char c : ASCII_input) {
            all_values_to_decode.push_back(c);
        }

        // Padding to make the input length a multiple of 5
        while (all_values_to_decode.size() % 5 != 0) {
            all_values_to_decode.push_back('u'); // Padding with 'u'
            extra_symbols++;
        }

        // Decoding ASCII85 to bytes
        std::vector<int> decoded_bytes;
        for (size_t i = 0; i < all_values_to_decode.size(); i += 5) {
            int total = 0;
            for (int j = 0; j < 5; ++j) {
                char c = all_values_to_decode[i + j];
                if (c < '!' || (c > 'u' && c != 'z')) { // Handle valid ASCII85 range
                    std::cerr << "Invalid character: " << c << std::endl;
                    return;
                }
                if (c == 'z') {
                    // Special case for 'z' - represents 4 zero bytes
                    total = 0;
                } else {
                    total = total * 85 + (c - '!');
                }
            }

            decoded_bytes.push_back((total >> 24) & 0xFF);
            decoded_bytes.push_back((total >> 16) & 0xFF);
            decoded_bytes.push_back((total >> 8) & 0xFF);
            decoded_bytes.push_back(total & 0xFF);
        }

        // Remove padding
        decoded_bytes.resize(decoded_bytes.size() - extra_symbols);

        // Output the decoded bytes in hexadecimal
        for (int byte : decoded_bytes) {
            std::cout << std::hex << static_cast<int>(byte) << " ";
        }
        std::cout << "\n";

        // Clear buffers for the next round
        all_values_to_decode.clear();
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [-e|-d]" << std::endl;
        return 1;
    }

    if (std::string(argv[1]) == "-e") {
        Encoder();
    } else if (std::string(argv[1]) == "-d") {
        Decoder();
    } else {
        std::cerr << "Invalid option. Use -e for encoding or -d for decoding." << std::endl;
        return 1;
    }

    return 0;
}
