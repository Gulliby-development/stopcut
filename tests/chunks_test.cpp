#include "harness.hpp"

static const std::vector<Row> rows = {
    {{"cut", "--stop", "zz"}, "", 0, "\n", ""},
    {{"cut", "--stop", "zz"}, "\n", 0, "\n", ""},
    {{"cut", "--stop", "zz"}, "ab\ncd", 0, "abcd\n", ""},
    {{"cut", "--stop", "zz"}, "ab\r\ncd\r\n", 0, "abcd\n", ""},
    {{"cut", "--stop", "zz"}, "ab\r", 0, "ab\n", ""},
    {{"cut", "--stop", "zz"}, "a\\nb\\tc\\\\d\n", 0, "a\\nb\\tc\\\\d\n", ""},
    {{"cut", "--stop", "zz"}, "A\\x41\\X7A\\xe9\n", 1, "", "stopcut: line 1: bad escape\n"},
    {{"cut", "--stop", "zz"}, "ab\na\\q\n", 1, "", "stopcut: line 2: bad escape\n"},
    {{"cut", "--stop", "zz"}, "ab\nc\\\n", 1, "", "stopcut: line 2: bad escape\n"},
    {{"cut", "--stop", "zz"}, "ab\nc\\x4\n", 1, "", "stopcut: line 2: bad escape\n"},
    {{"cut", "--stop", "zz"}, "nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n", 0, "nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n", ""},
    {{"cut", "--stop", "zz"}, "ab\nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn\n", 1, "", "stopcut: line 2: chunk is too long (max 64 bytes)\n"},
    {{"cut", "--stop", "zz"}, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\n", 0, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxAAAAAAAAAAAAAAAAAAAAAAAA\n", ""},
    {{"cut", "--stop", "zz"}, "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\\x41\n", 1, "", "stopcut: line 1: chunk is too long (max 64 bytes)\n"},
    {{"cut", "--stop", "ab"}, "xab\nq\\q\n", 0, "x\n", ""},
    {{"cut", "--stop", "zz"}, "a\n\nb\n", 0, "ab\n", ""},
    {{"check", "--stop", "ab"}, "ab\nx\\q\n", 1, "", "stopcut: line 2: bad escape\n"},
};

int main() {
    return check_rows(rows);
}
