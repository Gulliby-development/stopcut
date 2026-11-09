#include "harness.hpp"

static const std::vector<Row> rows = {
    {{}, "", 2, "", "usage: stopcut cut --stop SEQ [--stop SEQ]... [--keep]\n       stopcut trace --stop SEQ [--stop SEQ]... [--keep]\n       stopcut stats --stop SEQ [--stop SEQ]... [--keep]\n       stopcut check [--stop SEQ]...\n"},
    {{"help"}, "", 0, "usage: stopcut cut --stop SEQ [--stop SEQ]... [--keep]\n       stopcut trace --stop SEQ [--stop SEQ]... [--keep]\n       stopcut stats --stop SEQ [--stop SEQ]... [--keep]\n       stopcut check [--stop SEQ]...\n", ""},
    {{"-h", "extra"}, "", 0, "usage: stopcut cut --stop SEQ [--stop SEQ]... [--keep]\n       stopcut trace --stop SEQ [--stop SEQ]... [--keep]\n       stopcut stats --stop SEQ [--stop SEQ]... [--keep]\n       stopcut check [--stop SEQ]...\n", ""},
    {{"--help"}, "", 0, "usage: stopcut cut --stop SEQ [--stop SEQ]... [--keep]\n       stopcut trace --stop SEQ [--stop SEQ]... [--keep]\n       stopcut stats --stop SEQ [--stop SEQ]... [--keep]\n       stopcut check [--stop SEQ]...\n", ""},
    {{"--version"}, "", 0, "stopcut 0.1.0\n", ""},
    {{"--version", "x"}, "", 0, "stopcut 0.1.0\n", ""},
    {{"frob"}, "", 2, "", "stopcut: unknown command frob\n"},
    {{""}, "", 2, "", "stopcut: unknown command \n"},
    {{"Cut"}, "", 2, "", "stopcut: unknown command Cut\n"},
    {{"--stop"}, "", 2, "", "stopcut: unknown command --stop\n"},
    {{"--keep", "cut"}, "", 2, "", "stopcut: unknown command --keep\n"},
    {{"check", "--keep"}, "", 2, "", "stopcut: unknown option --keep\n"},
};

int main() {
    return check_rows(rows);
}
