#include "harness.hpp"

static const std::vector<Row> rows = {
    {{"cut"}, "a\n", 2, "", "stopcut: missing --stop\n"},
    {{"cut", "x"}, "", 2, "", "stopcut: unexpected argument x\n"},
    {{"cut", "-"}, "", 2, "", "stopcut: unexpected argument -\n"},
    {{"cut", "--bogus"}, "", 2, "", "stopcut: unknown option --bogus\n"},
    {{"cut", "--stop"}, "", 2, "", "stopcut: missing value for --stop\n"},
    {{"cut", "--stop", ""}, "", 2, "", "stopcut: empty stop sequence\n"},
    {{"cut", "--stop", "ab", "--stop", "ab"}, "", 2, "", "stopcut: duplicate stop sequence\n"},
    {{"cut", "--stop", "abcdefghijklmnop"}, "", 0, "\n", ""},
    {{"cut", "--stop", "abcdefghijklmnopq"}, "", 2, "", "stopcut: stop sequence is too long (max 16 bytes)\n"},
    {{"cut", "--stop", "a\\q"}, "", 2, "", "stopcut: bad escape in stop sequence\n"},
    {{"cut", "--stop", "a\\"}, "", 2, "", "stopcut: bad escape in stop sequence\n"},
    {{"cut", "--stop", "\\x4"}, "", 2, "", "stopcut: bad escape in stop sequence\n"},
    {{"cut", "--stop", "a", "--keep", "--keep"}, "", 2, "", "stopcut: given twice: --keep\n"},
    {{"trace", "--stop", "--keep"}, "", 0, "flushed 0 \"\"\n", ""},
    {{"stats", "--keep"}, "", 2, "", "stopcut: missing --stop\n"},
    {{"cut", "--stop", "a", "--stop", "b", "--stop", "c", "--stop", "d", "--stop", "e", "--stop", "f", "--stop", "g", "--stop", "h"}, "", 0, "\n", ""},
    {{"cut", "--stop", "a", "--stop", "b", "--stop", "c", "--stop", "d", "--stop", "e", "--stop", "f", "--stop", "g", "--stop", "h", "--stop", "i"}, "", 2, "", "stopcut: too many stop sequences (max 8)\n"},
    {{"check", "--stop", "ab", "--stop", "ab"}, "", 2, "", "stopcut: duplicate stop sequence\n"},
    {{"check", "--stop", ""}, "", 2, "", "stopcut: empty stop sequence\n"},
    {{"cut", "--stop", "ab", "--stop", "\\x61b"}, "", 2, "", "stopcut: duplicate stop sequence\n"},
};

int main() {
    return check_rows(rows);
}
