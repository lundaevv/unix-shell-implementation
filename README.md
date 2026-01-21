# Minishell (42 Project)

This project is part of the 42 curriculum and aims to recreate a simple shell (a minimal version of Bash), which is capable of interpreting and executing user commands.

## 🧠 Project Overview

Minishell is developed in C and reproduces the behavior of a Unix shell.  
It handles command parsing, environment variables, pipes, redirections, and built-in commands.

## 🧩 Features

- Prompt display and user input handling
- Command parsing and execution
- Built-in commands (`echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`)
- Environment variable management
- Support for quotes, pipes, and redirections
- Signal handling (`Ctrl+C`, `Ctrl+\`)

## 🧑‍💻 Team

- **Vitali Lund**  
  GitHub: https://github.com/lundaevv

- **GuljaWinchester**  
  GitHub: https://github.com/GuljaWinchester

## 🗂️ Project Structure

```bash
minishell/
├── src/        # Source files
├── includes/   # Header files
├── libft/      # Custom libft library
├── Makefile
└── README.md
```

## ⚙️ Compilation & Usage

```bash
make
./minishell
```

## 🧰 Requirements

- GCC or Clang compiler
- GNU Readline library
- Unix-like environment (macOS, Linux)

## 🚫 .gitignore

Recommended `.gitignore` for this project:

```gitignore
# build output
obj/
minishell

# macOS
.DS_Store

# editor settings
.vscode/

# logs
*.log
```
