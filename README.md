# Custom Shell in C
![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)

A simple, interactive shell written in C that supports basic command parsing and execution of common Linux commands, both built-in and external.

## Features

- Custom prompt: `irtaza's-shell>>`
- Built-in command support:
  - `cd`, `pwd`, `ls`
  - `mkdir`, `rmdir`, `rm`, `touch`
  - `cp`, `mv`, `grep`
- Executes other Linux commands using `execvp()`
- Parses user input into tokens
- Keeps running until `exit` is entered

## Technologies Used

- **C (GCC)**
- **Linux/POSIX system calls**
- **Standard I/O and file operations**

## How to Compile and Run

### Compile
```bash
gcc -o my_shell shell.c
```

### Run
```bash
./my_shell
```

### Clean (Optional)
```bash
rm my_shell
```

## Supported Commands

| Command | Description |
|---------|-------------|
| `ls` | List files in current directory |
| `pwd` | Print working directory path |
| `cd <dir>` | Change to specified directory |
| `mkdir <dir>` | Create a new directory |
| `rmdir <dir>` | Remove an empty directory |
| `rm <file>` | Delete a file |
| `touch <file>` | Create a new empty file |
| `cp <src> <dst>` | Copy source file to destination |
| `mv <src> <dst>` | Move or rename a file |
| `grep <pat> <f>` | Search for a pattern inside a file |
| `exit` | Exit the shell |
| (others) | Executed using `execvp()` (e.g. `echo`) |

## Example Usage

```bash
irtaza's-shell>> mkdir test
irtaza's-shell>> cd test
irtaza's-shell>> touch file.txt
irtaza's-shell>> echo "Hello World" > file.txt
irtaza's-shell>> grep Hello file.txt
irtaza's-shell>> cd ..
irtaza's-shell>> rm -r test
irtaza's-shell>> exit
```

## Project Structure

```
.
├── shell.c      # Main shell implementation
├── README.md    # Project documentation
└── .gitignore   # Git ignore file
```

## Author

**Irtaza Shahzad**
