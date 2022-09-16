# minsc

Immo Landwerth's Minsk in C. Hence the name.

This project is also home to many of my custom mini-libraries, which perform
small tasks in a robust, cross-platform way.

## The Language

Currently the language is a glorified calculator.
It supports the four functions you'd expect from
any calculator, as well as the unary `+` and `-` operators.
You may also parenthesize these expressions to specify order of evaluation.

There are two meta-commands:
 - `#cls` Clears the console window.
 - `#showTree` Debug command. Toggles showing the internal AST after each line input.

## Implementation

The implementation closely mirrors the original C# version (seen [here](https://github.com/terrajobst/minsk)).
 - Input is tokenized ([lexer.c](/src/code_analysis/syntax/lexer.c)).
 - An abstract syntax tree is built (the root node is in [expression_syntax.h](/include/minsc/code_analysis/syntax/expression_syntax.h), and the parser in [parser.c](/src/code_analysis/syntax/parser.c)).
 - Type checking occurs in [binder.c](/src/code_analysis/binding/binder.c) (though currently there is only one type).
 - The expression is evaluated ([evaluator.c](/src/code_analysis/evaluator.c)).

## Mini-Libraries

### buf

`buf` is a library for dynamic buffers, also known as vectors.
It is one header composed of macros.

Dependencies: None.

### c-argparser

This project has its own repo. See [README](https://github.com/Phytolizer/c-argparser#readme)

### file

Cross-platform file operations. Current support:
 - Reading a file into memory ("slurping")
 - Testing a file's existence
 - Testing a file's directory status

Dependencies:
 - [str](#str)
 - [sum](#sum)

### linenoise

This is not my repo, just a fork. See [the original](https://github.com/msteveb/linenoise).

### println

Extremely simple wrapper for the printf family.
Currently no support for s\*printf.

Dependencies:
 - [hedley]

### str

Unofficial fork of [maxim2266/str](https://github.com/maxim2266/str).
Main differences are I added a couple of extra helpful functions, and it compiles on Windows.

Dependencies:
  - [hedley]

### styler

Translation of [utilForever/styler](https://github.com/utilForever/styler).
It was originally C++, so I rewrote it in C for consistency.
It's quite unergonomic, and I hope to fix that eventually.

Dependencies: None.

### sum

Some extremely useful sum types, inspired by Haskell and Rust, though much less safe.
They are all implemented as macros, and I use them pervasively.

Dependencies: None.

## Mini-Programs

### varycase

I use this to convert X-macros in my source to UpperCamelCase, for presentation to the user.
It comes with a [CMake macro](/cmake/VaryCase.cmake).

Dependencies:
 - [buf](#buf)
 - [c-argparser](#c-argparser)
 - [file](#file)
 - [println](#println)
 - [str](#str)
 - [sum](#sum)

[hedley]: https://github.com/nemequ/hedley
