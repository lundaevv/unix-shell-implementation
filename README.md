# Minishell — Bash-like Shell Implementation

A Unix shell implementation in C with parsing, pipes, redirections, and built-in commands.

## Overview

Minishell is a Bash-like command-line interpreter written in C.  
It supports command execution, environment variables, pipes, redirections, and built-in commands, while focusing on low-level Unix system programming concepts such as processes, file descriptors, and signal handling.

## Features

- Interactive shell prompt
- Command parsing and tokenization
- Execution of external programs
- Built-in commands:
  - `echo`
  - `cd`
  - `pwd`
  - `export`
  - `unset`
  - `env`
  - `exit`
- Environment variable expansion
- Pipes (`|`)
- Input and output redirections (`<`, `>`, `>>`)
- Signal handling (`Ctrl+C`, `Ctrl+\`, `Ctrl+D`)

## Tech Stack

- C
- Unix system calls (`fork`, `execve`, `pipe`, `dup2`)
- - GNU Readline

## Core Concepts

- Process creation and management
- File descriptors and redirections
- Inter-process communication with pipes
- Parsing and tokenization
- Environment management
- Signal handling

## Architecture

The shell is organized into several main stages:

1. **Input handling**
   - Reads user input and displays the prompt

2. **Lexer / Tokenizer**
   - Splits input into meaningful tokens while handling quotes and operators

3. **Parser**
   - Transforms tokens into executable command structures

4. **Executor**
   - Launches external commands and built-ins

5. **Pipes and redirections**
   - Configures file descriptors for command chaining and input/output redirection
  
## Challenges

- Implementing a robust parser with support for quotes and environment variables
- Managing file descriptors across multiple processes
- Handling signals consistently with Bash behavior
- Avoiding memory leaks in a long-running process

## ⚙️ Build and Run

```bash
make
./minishell
```

## Requirements

- GCC or Clang compiler
- GNU Readline library
- Unix-like environment (macOS, Linux)

## Project Structure

```bash
minishell/
├── src/        # Source files
├── includes/   # Header files
├── libft/      # Custom libft library
├── Makefile
└── README.md
```

## Authors

- **Vitali Lund**  
  GitHub: https://github.com/lundaevv
- **GuljaWinchester**  
  GitHub: https://github.com/GuljaWinchester
