#include <iostream>
#include <string_view>
#include <vector>

namespace base64 {

ssize_t decode(std::string_view input, char * output) {
    static constexpr unsigned char kDecodingTable[] = {
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
            64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
            64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64
    };

    size_t in_len = input.size();
    if (in_len % 4 != 0) return -1;

    size_t out_len = in_len / 4 * 3;
    if (input[in_len - 1] == '=') out_len--;
    if (input[in_len - 2] == '=') out_len--;

    for (size_t i = 0, j = 0; i < in_len;) {
        uint32_t a = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
        uint32_t b = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
        uint32_t c = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];
        uint32_t d = input[i] == '=' ? 0 & i++ : kDecodingTable[static_cast<int>(input[i++])];

        uint32_t triple = (a << 3 * 6) + (b << 2 * 6) + (c << 1 * 6) + (d << 0 * 6);

        if (j < out_len) output[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < out_len) output[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < out_len) output[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return out_len;
}

}

void debug_exit() {
    std::cerr << "debug" << std::endl;
    exit(0);
}

void unused_function() {
    asm ("mov    0xc3e0(%ecx),%ecx\n");
}

std::string_view prefix = "Дорогой Дедушка Мороз,\n"
                          "В этом году я вёл себя х";
std::string_view shapka_end = "Подари мне, пожалуйста, шапку";
std::string_view mashinka_end = "Подари мне, пожалуйста, машинку";

std::string find_present(std::string_view letter_encoded) {
    char buf[4096] = {};
    base64::decode(letter_encoded, buf);

    std::string_view text(buf);
    if (text.substr(0, prefix.size()) != prefix) {
        return "";  // Спам, однако
    }

    if (text.substr(text.size() - shapka_end.size()) == shapka_end) {
        return "Шапка";
    }

    if (text.substr(text.size() - mashinka_end.size()) == mashinka_end) {
        return "Машинка";
    }

    return "Ручная проверка";
}


int main() {
    std::string name, second_name, letter_encoded;
    char safe_zone[4096];
    while (std::cin >> name >> second_name >> letter_encoded) {
        auto present = find_present(letter_encoded);
        if (!present.empty()) {
            std::cout << name << ' ' << second_name << ' ' << present << std::endl;
        }
    }
}
