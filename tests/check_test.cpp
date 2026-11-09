#include "harness.hpp"

static const std::vector<Row> rows = {
    {{"check"}, "", 0, "chunks 0 bytes 0\n", ""},
    {{"check"}, "ab\ncd\n", 0, "chunks 2 bytes 4\n", ""},
    {{"check", "--stop", "bc"}, "abc\nbcd\n", 0, "stop 1 \"bc\": inside 1\nchunks 2 bytes 6\n", ""},
    {{"check", "--stop", "bc", "--stop", "cb", "--stop", "abcb"}, "abc\nbcd\n", 0, "stop 1 \"bc\": inside 1\nstop 2 \"cb\": split 2\nstop 3 \"abcb\": split 0\nchunks 2 bytes 6\n", ""},
    {{"check", "--stop", "zz"}, "ab\n", 0, "stop 1 \"zz\": absent\nchunks 1 bytes 2\n", ""},
    {{"check", "--stop", "ab"}, "a\n\nb\n", 0, "stop 1 \"ab\": split 0\nchunks 3 bytes 2\n", ""},
    {{"check", "--stop", "b\\n"}, "ab\nx\n", 0, "stop 1 \"b\\n\": absent\nchunks 2 bytes 3\n", ""},
    {{"check", "--stop", "a\\x62"}, "a\nb\n", 0, "stop 1 \"ab\": split 0\nchunks 2 bytes 2\n", ""},
    {{"check", "--stop", "ab"}, "a\\x41\nab\n\\xe9\n", 0, "stop 1 \"ab\": inside 2\nchunks 3 bytes 5\n", ""},
};

int main() {
    return check_rows(rows);
}
