#include "stopcut.hpp"

namespace stopcut {

static int hex_value(unsigned char byte) {
    if (byte >= '0' && byte <= '9') {
        return byte - '0';
    }
    if (byte >= 'a' && byte <= 'f') {
        return byte - 'a' + 10;
    }
    if (byte >= 'A' && byte <= 'F') {
        return byte - 'A' + 10;
    }
    return -1;
}

std::string unescape(const std::string &text, bool &ok) {
    std::string bytes;
    bytes.reserve(text.size());
    ok = true;

    for (std::size_t index = 0; index < text.size(); ++index) {
        const unsigned char byte = static_cast<unsigned char>(text[index]);
        if (byte != '\\') {
            bytes.push_back(static_cast<char>(byte));
            continue;
        }

        if (++index >= text.size()) {
            ok = false;
            break;
        }

        switch (text[index]) {
        case 'n':
            bytes.push_back('\n');
            break;
        case 't':
            bytes.push_back('\t');
            break;
        case '\\':
            bytes.push_back('\\');
            break;
        case 'x': {
            if (index + 2 >= text.size()) {
                ok = false;
                break;
            }
            const int high = hex_value(static_cast<unsigned char>(text[index + 1]));
            const int low = hex_value(static_cast<unsigned char>(text[index + 2]));
            if (high < 0 || low < 0) {
                ok = false;
                break;
            }
            bytes.push_back(static_cast<char>((high << 4) | low));
            index += 2;
            break;
        }
        default:
            ok = false;
            break;
        }

        if (!ok) {
            break;
        }
    }

    return bytes;
}

std::string escape(const std::string &bytes) {
    static const char hex[] = "0123456789abcdef";
    std::string text;
    text.reserve(bytes.size());

    for (const char character : bytes) {
        const unsigned char byte = static_cast<unsigned char>(character);
        if (byte == '\\') {
            text += "\\\\";
        } else if (byte == '\n') {
            text += "\\n";
        } else if (byte == '\t') {
            text += "\\t";
        } else if (byte < 0x20 || byte > 0x7e) {
            text += "\\x";
            text.push_back(hex[byte >> 4]);
            text.push_back(hex[byte & 0x0f]);
        } else {
            text.push_back(character);
        }
    }

    return text;
}

} // namespace stopcut
