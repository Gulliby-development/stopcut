#ifndef STOPCUT_HPP
#define STOPCUT_HPP

#include <cstddef>
#include <string>
#include <vector>

namespace stopcut {

// What one command line produced: the exit status and the text for standard output and standard error.
struct Result {
    int status;
    std::string out;
    std::string err;
};

// Thrown by a command or the chunk reader: the exit status and the message without "stopcut: " and the line feed.
struct Failure {
    int status;
    std::string message;
};

// The options of a command line: the stop sequences with their escapes decoded, in the order given, and the --keep flag.
struct Options {
    std::vector<std::string> stops;
    bool keep;
};

using Handler = Result (*)(const Options &options, const std::string &input);

// A command: its name, whether it accepts --keep, whether it needs at least one --stop, and its handler (which may throw Failure).
struct Command {
    const char *name;
    bool allows_keep;
    bool needs_stop;
    Handler run;
};

// What the stream engine did with one chunk: the text it let through, the text it held back and whether a stop sequence ended the stream here.
struct Step {
    std::string emit;
    std::string hold;
    bool stopped;
};

// The whole play of a stream: one Step for every chunk that was read, the held text flushed at the end (empty after a stop),
// the bytes of the chunks that were read, the bytes let through (the flush included) and the largest held text.
struct Play {
    std::vector<Step> steps;
    std::string flush;
    std::size_t bytes;
    std::size_t total;
    std::size_t peak;
    bool stopped;
};

extern const char *const USAGE;
extern const std::vector<Command> COMMANDS;

// cli.cpp
Result run(const std::vector<std::string> &args, const std::string &input);
bool reads_stdin(const std::vector<std::string> &args);

// escape.cpp: decode \n \t \ and \xHH (ok is false for any other backslash use); encode bytes for display
std::string unescape(const std::string &text, bool &ok);
std::string escape(const std::string &bytes);

// chunks.cpp: the lines of the input, and the decoded chunk of one line (number counts from 1; throws Failure(1, "line N: ..."))
std::vector<std::string> split_lines(const std::string &input);
std::string read_chunk(const std::string &line, std::size_t number);

// engine.cpp: reads the chunks lazily (a line after the stop is never decoded) and plays them against the stop sequences
Play play(const std::string &input, const Options &options);

// cmd_cut.cpp, cmd_trace.cpp, cmd_check.cpp
Result cut_command(const Options &options, const std::string &input);
Result trace_command(const Options &options, const std::string &input);
Result stats_command(const Options &options, const std::string &input);
Result check_command(const Options &options, const std::string &input);

} // namespace stopcut

#endif
