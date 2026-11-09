#include "harness.hpp"

static const std::vector<Row> rows = {
    {{"trace", "--stop", "ab"}, "xaby\n", 0, "1 emit \"x\" stop\nstopped 1\n", ""},
    {{"trace", "--stop", "ab"}, "xa\nby\n", 0, "1 emit \"x\" hold \"a\"\n2 emit \"\" stop\nstopped 1\n", ""},
    {{"trace", "--stop", "bcd", "--stop", "cx"}, "ab\ncd\nef\n", 0, "1 emit \"a\" hold \"b\"\n2 emit \"\" stop\nstopped 1\n", ""},
    {{"trace", "--stop", "abcd", "--stop", "bc"}, "abc\nd\n", 0, "1 emit \"a\" stop\nstopped 1\n", ""},
    {{"trace", "--stop", "ab", "--keep"}, "xa\nby\n", 0, "1 emit \"x\" hold \"a\"\n2 emit \"ab\" stop\nstopped 3\n", ""},
    {{"trace", "--stop", "abc"}, "ab\nab\nx\n", 0, "1 emit \"\" hold \"ab\"\n2 emit \"ab\" hold \"ab\"\n3 emit \"abx\" hold \"\"\nflushed 5 \"\"\n", ""},
    {{"trace", "--stop", "ab"}, "", 0, "flushed 0 \"\"\n", ""},
    {{"trace", "--stop", "aa"}, "a\na\n", 0, "1 emit \"\" hold \"a\"\n2 emit \"\" stop\nstopped 0\n", ""},
    {{"trace", "--stop", "aab"}, "aa\naa\nb\n", 0, "1 emit \"\" hold \"aa\"\n2 emit \"aa\" hold \"aa\"\n3 emit \"\" stop\nstopped 2\n", ""},
    {{"trace", "--stop", "ab", "--stop", "ba"}, "a\n\nb\na\n", 0, "1 emit \"\" hold \"a\"\n2 emit \"\" hold \"a\"\n3 emit \"\" stop\nstopped 0\n", ""},
    {{"trace", "--stop", "ab"}, "a\\x09\n", 0, "1 emit \"a\\t\" hold \"\"\nflushed 2 \"\"\n", ""},
};

int main() {
    return check_rows(rows);
}
