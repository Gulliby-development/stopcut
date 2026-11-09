<p align="center"><img src="docs/logo.svg" width="96" alt="" /></p>

# stopcut

[![CI](https://github.com/Gulliby-development/stopcut/actions/workflows/ci.yml/badge.svg)](https://github.com/Gulliby-development/stopcut/actions/workflows/ci.yml) [![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

stopcut is a small, offline C++17 command-line tool for replaying a text stream as chunks and stopping at one of up to eight byte sequences. It is intended for people building chat clients, agents, and evaluation harnesses who want to inspect what a stop sequence does across chunk boundaries. It does not call a model or use a network, and it uses only the C++ standard library.

## Build and run

Requirements: CMake 3.16 or newer and a C++17 compiler (for example, Visual Studio on Windows, or Clang/GCC on macOS and Linux).

From the repository root:

```sh
cmake -S . -B build
cmake --build build
```

Run the executable from the build folder. In this guide, `stopcut` means the built program: use `build/stopcut` with a single-configuration generator, or `build/Debug/stopcut` with the Visual Studio generator on Windows (the executable may have the `.exe` suffix). The examples below use POSIX shell syntax, run at the repository root after building. Define this convenience function first:

```sh
stopcut() { ./build/stopcut "$@"; }
```

## Commands

```text
usage: stopcut cut --stop SEQ [--stop SEQ]... [--keep]
       stopcut trace --stop SEQ [--stop SEQ]... [--keep]
       stopcut stats --stop SEQ [--stop SEQ]... [--keep]
       stopcut check [--stop SEQ]...
```

All commands consume standard input. `cut`, `trace`, and `stats` require at least one `--stop`; `check` accepts none or more. The `--keep` option is available to the first three commands: when a stop is found, it includes the matched stop bytes in the emitted text. Without it, the match itself is removed.

`help`, `-h`, or `--help` as the first argument prints the usage and exits successfully; `--version` prints `stopcut 0.1.0`. These take precedence over any following arguments. With no command, usage is written to standard error and the status is 2. An unrecognized command also exits 2.

A successful command writes its result to standard output and nothing to standard error. A command-line error exits 2; a stream error exits 1. Both errors leave standard output empty and write one line to standard error beginning with `stopcut: `.

### Command-line rules and diagnostics

Arguments are examined from left to right. A word that is empty, exactly `-`, or does not begin with `-` is reported as `stopcut: unexpected argument WORD`. An unsupported option (including `--keep` for `check`) is `stopcut: unknown option WORD`. A final `--stop` with no following value reports `stopcut: missing value for --stop`. The next word is always consumed as the stop value, even if it looks like an option.

Stop values are decoded and validated as they are encountered, in this order: malformed escape gives `stopcut: bad escape in stop sequence`; zero decoded bytes gives `stopcut: empty stop sequence`; more than 16 decoded bytes gives `stopcut: stop sequence is too long (max 16 bytes)`; a decoded value already supplied gives `stopcut: duplicate stop sequence`; and a ninth stop gives `stopcut: too many stop sequences (max 8)`. Repeating `--keep` gives `stopcut: given twice: --keep`. Once all arguments have been parsed, a missing stop for `cut`, `trace`, or `stats` gives `stopcut: missing --stop`.

An unknown command is reported as `stopcut: unknown command WORD`; no-command usage and all command-line diagnostics end with a line feed on standard error. Stream errors include the one-based line number: `stopcut: line N: bad escape` or `stopcut: line N: chunk is too long (max 64 bytes)`. Escape validity is checked before the decoded chunk-size limit. There is no partial output on failure.

## Escapes and input format

A stop value and each stream line use the same byte escapes: `\\n` represents a line feed, `\\t` a tab, `\\\\` a backslash, and `\\xHH` a byte written with exactly two hexadecimal digits (upper- or lower-case digits are accepted). Every other byte represents itself. Any other backslash use, including a trailing backslash, is invalid. Stop sequences must contain 1–16 decoded bytes. A decoded stream chunk may contain 0–64 bytes.

The stream is UTF-8-independent byte-oriented text, with input lines separated by LF. One CR immediately before an LF, or at the end of the input, is removed. A final line without LF is still a chunk; an LF at the very end does not add a further chunk. Empty input has no chunks, while a single LF denotes one empty chunk. Empty chunks count as chunks but contribute no bytes.

Displayed bytes are rendered as plain ASCII: backslash, line feed, and tab are written as `\\\\`, `\\n`, and `\\t`; any other byte below 0x20 or above 0x7E is written as `\\x` followed by two lower-case hexadecimal digits. Other bytes are displayed unchanged.

## How matching works

The player carries a held tail that may still become the start of a stop sequence. For each chunk it joins the held tail and the new chunk, then looks for stops in that combined buffer. The match with the earliest starting byte wins; if multiple stops start at the same byte, the longest match wins. The output is the bytes before the match, or through its end with `--keep`. No later chunk is decoded after a match.

When there is no match yet, stopcut holds the longest suffix that is a proper prefix of any stop sequence and emits the rest. At end of stream it flushes the held tail. Chunk boundaries affect when a match becomes complete: with stops `abcd` and `bc`, chunks `ab` then `cd` match `abcd`, whereas chunks `abc` then `d` match `bc` first.

Valid command input is initially buffered by the program before command execution. During cut, trace, and stats playback, chunk escape and length validation is performed only as chunks are processed, so malformed text after a completed stop is not reported. The `check` command instead validates and joins the entire stream, including lines after any stop occurrence.

## Output

- **cut** emits the complete allowed byte text, including any final flush, encoded with the display rules above, followed by LF. It exits 0 whether or not a stop was found.
- **trace** prints one line per processed chunk: `N emit "TEXT" hold "HELD"`, or `N emit "TEXT" stop` for the chunk that completes a stop. It ends with `stopped T` after a match, or `flushed T "TEXT"` at end of input. `N` is the one-based chunk number, `TEXT` is emitted text, `HELD` is the held tail, and `T` is the total emitted byte count including a flush. An empty stream prints `flushed 0 ""`.
- **stats** prints `chunks=C bytes=B emitted=T held=H stopped=yes|no`. These are the number of processed chunks, their decoded byte count, total emitted bytes including flush, largest held-tail size after a chunk, and whether a stop was found.
- **check** examines the fully joined stream and prints one line per stop, in the order supplied: `stop N "SEQ": KIND`. `KIND` is `absent`, `inside OFFSET`, or `split OFFSET`. The offset is the first occurrence's zero-based byte offset; `inside` means the match lies in one non-empty chunk, and `split` means it crosses a chunk boundary. It concludes with `chunks C bytes B`. With no stops, only that summary is printed. Empty chunks count in C but do not affect offsets or B.

## Examples

```text
$ printf 'Hello, wor\nld!EN\nD more\n' | stopcut cut --stop END
Hello, world!
$ printf 'one EN\nD two\n' | stopcut trace --stop END
1 emit "one " hold "EN"
2 emit "" stop
stopped 4
$ printf 'xab\ncd\n' | stopcut cut --stop abcd --stop bc; printf 'xabc\nd\n' | stopcut cut --stop abcd --stop bc
x
xa
$ printf 'x END y\nEN\nD\n' | stopcut check --stop END --stop 'q r'
stop 1 "END": inside 2
stop 2 "q r": absent
chunks 3 bytes 10
```

## Development

The project builds a C++17 library and executable with CMake. Its tests are literal rows run through the command dispatcher: 103 rows in eight command-focused test files, plus the scaffold test. To format and check source layout, run `clang-format --dry-run --Werror` over the C++ source and test files (or use `clang-format -i` to format them). Then run the test suite:

```sh
ctest --test-dir build --output-on-failure -C Debug
```

The build enables strict warnings and treats them as errors. The command handlers return results rather than writing streams; the entry point handles standard input and output.

---

Built with [BAKED](https://github.com/Gulliby-development/baked), an open-source coding agent (bench round R91).
