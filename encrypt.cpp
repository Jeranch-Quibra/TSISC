#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <cmath>
#include <cctype>
#include <stdexcept>

// ANSI color codes for styling
const std::string RESET = "\033[0m";
const std::string YELLOW = "\033[33m";
const std::string RED = "\033[91m";
const std::string GREEN = "\033[92m";
const std::string CYAN = "\033[96m";
const std::string WHITE = "\033[97m";
const std::string GRAY = "\033[90m";

// Base64 encoding from previous snippet
static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string base64_encode(const std::string& input) {
    std::string output;
    int i = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    int input_len = input.size();
    int pos = 0;

    while (input_len--) {
        char_array_3[i++] = input[pos++];
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; i <4 ; i++)
                output += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for(int j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (int j = 0; j < i + 1; j++)
            output += base64_chars[char_array_4[j]];

        while(i++ < 3)
            output += '=';
    }

    return output;
}

// Quantum randomness key generator
std::pair<std::string, std::string> electron_key_generator(int length = 32) {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    long long seed = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

    double position = (seed % 97) + ((seed % 10) / 10.0);

    std::string key;
    for (int i = 0; i < length; i++) {
        double energy = (std::sin(position) + std::cos(position * 2) + std::tan(position / 3)) * 100;
        int quantum_shift = static_cast<int>(std::abs(std::pow(std::abs(energy), 1.5))) % 94;
        char c = static_cast<char>(33 + quantum_shift);
        key += c;
        position = std::fmod(std::pow(position, 1.2) + 7, 89);
    }

    std::ostringstream ascii_key_stream;
    for (char c : key) {
        ascii_key_stream << static_cast<int>(c);
    }
    std::string ascii_key = ascii_key_stream.str();

    return {key, ascii_key};
}

// Encrypt message function
std::string encrypt_message(const std::string& message, const std::string& key_ascii, int rounds=10) {
    std::vector<int> message_ascii;
    for (unsigned char c : message) {
        message_ascii.push_back(static_cast<int>(c));
    }

    std::vector<int> key_numbers;
    for (char d : key_ascii) {
        if (!std::isdigit(d)) {
            throw std::invalid_argument("Key ASCII contains non-digit characters");
        }
        key_numbers.push_back(d - '0');
    }

    for (int round_num = 0; round_num < rounds; ++round_num) {
        for (int key : key_numbers) {
            for (int& num : message_ascii) {
                num = (num + key + round_num) % 256;
            }
        }
    }

    std::string encrypted_message;
    for (int num : message_ascii) {
        encrypted_message += static_cast<char>(num);
    }

    return base64_encode(encrypted_message);
}

int main() {
    std::string art = YELLOW + "𓊈𝑾𝑬𝑳𝑪𝑶𝑴𝑬 𝑱۞ 𝑬𝑬𓊉" + RESET;

    std::cout << art << std::endl;
    std::cout << RED << "𓂀 " << RESET << GREEN << "Type" << WHITE << "[" << CYAN << "." << RESET << WHITE << "] " << RESET << GREEN << "in a newline to finish\n" << RESET;
    std::cout << "Type your message ☲ ๛" << RESET << std::endl;
    std::cout << "▬▬ι══════════════════════════ι▬▬" << WHITE << std::endl;

    std::vector<std::string> lines;
    while (true) {
        std::string line;
        std::getline(std::cin, line);
        if (line == ".") break;
        lines.push_back(line);
    }

    std::string message;
    for (size_t i = 0; i < lines.size(); i++) {
        message += lines[i];
        if (i != lines.size() - 1) message += "\n";
    }

    auto [original_key, ascii_representation] = electron_key_generator(32);
    std::string encrypted_message = encrypt_message(message, ascii_representation);

    std::cout << RESET << std::endl;
    std::cout << "࿇ ══━━━━━━━━✥◈✥━━━━━━━━══ ࿇" << std::endl;
    std::cout << std::endl;
    std::cout << GREEN << "۞ 𝑸𝒖𝒂𝒏𝒕𝒖𝒎 𝑬𝒍𝒆𝒄𝒕𝒓𝒐𝒏 𝒌𝒆𝒚" << RESET << std::endl;
    std::cout << RED << "𖠞 ➢ " << RESET << CYAN << original_key << std::endl;
    std::cout << std::endl;
    std::cout << RED << "𓂀  " << RESET << GREEN << "𝑬𝑵𝑪𝑹𝒀𝑷𝑻𝑬𝑫 𝑴𝑬𝑺𝑺𝑨𝑮𝑬    " << RESET << std::endl;
    std::cout << RED << "☲ ➣ " << RESET << CYAN << encrypted_message << std::endl;
    std::cout << RESET << GRAY;

    return 0;
}
