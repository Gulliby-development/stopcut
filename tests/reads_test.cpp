#include "harness.hpp"

static const std::vector<Row> rows = {
    {{"cut", "--stop", "a"}, "", 1, "", ""},
    {{"check"}, "", 1, "", ""},
    {{"trace", "--stop", "a", "--keep"}, "", 1, "", ""},
    {{"help"}, "", 0, "", ""},
    {{}, "", 0, "", ""},
    {{"frob"}, "", 0, "", ""},
    {{"cut"}, "", 0, "", ""},
    {{"cut", "--stop", ""}, "", 0, "", ""},
    {{"check", "--keep"}, "", 0, "", ""},
    {{"stats", "--stop", "a", "x"}, "", 0, "", ""},
};

int main() {
    return check_reads(rows);
}
