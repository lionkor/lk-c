# `lk` C library

## The quick version

- C17
- No 3rd party dependencies
- Channel library inspired by Go(-lang)'s `chan`
- Cross-platform threads library with some synchronisation builtin
- Input library with features like non-buffering getchar
- MIT license
- Meson build system

## Examples

All examples can be found in [src/examples](https://github.com/lionkor/lk/tree/master/src/examples).

- [threaded unbuffered user input directly to file](https://github.com/lionkor/lk/blob/master/src/examples/type_to_file.c) - In this example we're making use of LKThread, LKChannel and the lk input library. The main thread takes user input via an unbuffered getchar(), then writes it to a channel. This channel is waited on in another thread, which then writes the character to 'outfile.txt'. Inspecting the program's CPU usage, you will see that there is a 0% usage while input from the user is waited on. This demonstrates the use-case of channels, which is for one side to wait for data without spinning, and the other to be able to wait for the data to be processed before adding more, also without spinning.
