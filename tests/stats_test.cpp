#include "harness.hpp"

static const std::vector<Row> rows = {
    {{"stats", "--stop", "ab"}, "", 0, "chunks=0 bytes=0 emitted=0 held=0 stopped=no\n", ""},
    {{"stats", "--stop", "ab"}, "xaby\n", 0, "chunks=1 bytes=4 emitted=1 held=0 stopped=yes\n", ""},
    {{"stats", "--stop", "ab"}, "xa\nby\nnever\\q\n", 0, "chunks=2 bytes=4 emitted=1 held=1 stopped=yes\n", ""},
    {{"stats", "--stop", "abc"}, "ab\nab\nx\n", 0, "chunks=3 bytes=5 emitted=5 held=2 stopped=no\n", ""},
    {{"stats", "--stop", "abc", "--stop", "xyz"}, "abxy\nq\n", 0, "chunks=2 bytes=5 emitted=5 held=2 stopped=no\n", ""},
    {{"stats", "--stop", "ab", "--keep"}, "a\nb\n", 0, "chunks=2 bytes=2 emitted=2 held=1 stopped=yes\n", ""},
};

int main() {
    return check_rows(rows);
}
