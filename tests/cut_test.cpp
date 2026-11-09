#include "harness.hpp"

static const std::vector<Row> rows = {
    {{"cut", "--stop", "ab"}, "xaby\n", 0, "x\n", ""},
    {{"cut", "--stop", "ab"}, "xa\nby\n", 0, "x\n", ""},
    {{"cut", "--stop", "bcd", "--stop", "cx"}, "ab\ncd\n", 0, "a\n", ""},
    {{"cut", "--stop", "abcd", "--stop", "bc"}, "ab\ncd\n", 0, "\n", ""},
    {{"cut", "--stop", "abcd", "--stop", "bc"}, "abc\nd\n", 0, "a\n", ""},
    {{"cut", "--stop", "ab", "--keep"}, "xaby\n", 0, "xab\n", ""},
    {{"cut", "--stop", "ab", "--stop", "abc", "--keep"}, "xabcy\n", 0, "xabc\n", ""},
    {{"cut", "--stop", "ab", "--stop", "abc"}, "xabcy\n", 0, "x\n", ""},
    {{"cut", "--stop", "ab"}, "a\na\nb\n", 0, "a\n", ""},
    {{"cut", "--stop", "ab"}, "xa\n", 0, "xa\n", ""},
    {{"cut", "--stop", "abc"}, "ab\n", 0, "ab\n", ""},
    {{"cut", "--stop", "aab"}, "aa\na\nb\n", 0, "a\n", ""},
    {{"cut", "--stop", "ab"}, "\n", 0, "\n", ""},
    {{"cut", "--stop", "ab"}, "ab\n", 0, "\n", ""},
    {{"cut", "--stop", "ab", "--keep"}, "ab\n", 0, "ab\n", ""},
    {{"cut", "--stop", "x\\n"}, "ax\n\nb\n", 0, "axb\n", ""},
    {{"cut", "--stop", "zz"}, "a\\x01b\\xe9\n", 0, "a\\x01b\\xe9\n", ""},
    {{"cut", "--stop", "b", "--stop", "a"}, "xyab\n", 0, "xy\n", ""},
};

int main() {
    return check_rows(rows);
}
