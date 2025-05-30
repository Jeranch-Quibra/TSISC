#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype>
#include <stdexcept>

// ANSI color codes
const std::string RESET = "\033[0m";
const std::string YELLOW = "\033[33m";
const std::string RED = "\033[91m";
const std::string GREEN = "\033[92m";
const std::string CYAN = "\033[96m";
const std::string WHITE = "\033[97m";
const std::string GRAY = "\033[90m";

// Base64 decoding function (same as before)
static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(const std::string &encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;

        for (j = 0; j <4; j++)
            char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }

    return ret;
}

// Decrypt function matching the Python logic
std::string decrypt_message(const std::string& encrypted_base64, const std::string& key_ascii, int rounds=10) {
    std::string encrypted_message = base64_decode(encrypted_base64);

    std::vector<int> message_ascii;
    for (unsigned char c : encrypted_message) {
        message_ascii.push_back(static_cast<int>(c));
    }

    std::vector<int> key_numbers;
    for (char d : key_ascii) {
        if (!std::isdigit(d)) {
            throw std::invalid_argument("Key ASCII contains non-digit characters");
        }
        key_numbers.push_back(d - '0');
    }

    for (int round_num = rounds - 1; round_num >= 0; --round_num) {
        for (auto it = key_numbers.rbegin(); it != key_numbers.rend(); ++it) {
            int key = *it;
            for (int& num : message_ascii) {
                num = (num - key - round_num + 256) % 256;
            }
        }
    }

    std::string decrypted_message;
    for (int num : message_ascii) {
        decrypted_message += static_cast<char>(num);
    }

    return decrypted_message;
}

int main() {
    const std::string art = YELLOW + "𓊈𝑫𝑬𝑪𝑹𝒀𝑷𝑻 𝑱۞ 𝑬𝑬𓊉" + RESET;

    while (true) {
        std::cout << art << std::endl;
        std::cout << RED << "𓂀 " << RESET << GREEN << "Encrypted message ☲ ๛" << RESET << std::endl;
        std::cout << RED << "➣ " << CYAN;

        std::string encrypted_message;
        std::getline(std::cin, encrypted_message);
        std::cout << RESET;

        std::cout << "࿇ ══━━━━━━━━✥◈✥━━━━━━━━══ ࿇" << std::endl;
        std::cout << GREEN << RED << "𖠞 " << GREEN << "Key " << RESET << std::endl;
        std::cout << RED << "➢ " << RESET << CYAN;

        std::string decryption_key;
        std::getline(std::cin, decryption_key);
        std::cout << RESET;

        // Convert key to ascii representation as string of concatenated ordinals
        std::ostringstream ascii_key_stream;
        for (char c : decryption_key) {
            ascii_key_stream << static_cast<int>(c);
        }
        std::string ascii_representation = ascii_key_stream.str();

        try {
            std::string decrypted_message = decrypt_message(encrypted_message, ascii_representation);
            // Remove leading whitespace if any (like Python's lstrip)
            size_t first_non_space = decrypted_message.find_first_not_of(" \t\n\r");
            if (first_non_space != std::string::npos) {
                decrypted_message = decrypted_message.substr(first_non_space);
            }

            std::cout << std::endl;
            std::cout << RESET << GREEN << "Decrypted Message ☲ ✔" << RESET << std::endl;
            std::cout << "▬▬ι══════════════════════════ι▬▬" << std::endl;
            std::cout << decrypted_message << std::endl;
            std::cout << RESET << "࿇ ══━━━━━━━━✥◈✥━━━━━━━━══ ࿇" << std::endl << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << RED << "Error during decryption: " << e.what() << RESET << std::endl;
        }
    }
    return 0;
}