# Minishell

[![C](https://img.shields.io/badge/Language-C-555555?logo=c)](https://www.iso.org/standard/74528.html)
[![Build](https://img.shields.io/badge/Build-make-333333)](https://www.gnu.org/software/make/)
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS-blue)](https://github.com)

A minimal Unix shell built from scratch in C. Readline-driven prompt, pipelines, redirections, heredoc, logical operators, and a full set of built-ins — no `system()`, no `popen()`. Everything is wired directly on `fork`, `execve`, `pipe`, and `dup2`.

---

## Quick Start

Install **readline** for your platform, then build and run.

**macOS:**
```bash
brew install readline
```

**Linux:**
```bash
# Debian / Ubuntu
sudo apt install libreadline-dev

# Fedora / RHEL
sudo dnf install readline-devel

# Arch
sudo pacman -S readline
```

```bash
make && ./minishell
```

`make re` for a clean rebuild. Exit with `exit` or **Ctrl+D**.

---

## Features

Input is normalized (spaces added around `|`, `&&`, `||`, and redirection operators), then split by delimiter strings in precedence order: first `" || "`, then `" && "`, then `|`. Characters inside quotes are masked so delimiters are not split on mid-word. Syntax checks reject invalid or incomplete redirections and pipes before any execution. Each pipeline is represented as a linked list of commands (`t_cmd`). Execution runs a single pipeline, or loops over the `||` and `&&` segments and runs each pipeline in turn with short-circuit logic.

**Pipelines & logic** — `|` connects stdout of one command to stdin of the next. `&&` and `||` short-circuit on exit status. Precedence is pipe-before-logic, so `cmd1 && cmd2 | cmd3` parses as `cmd1 && (cmd2 | cmd3)`.

**Redirections** — `>` truncates, `>>` appends, `<` reads stdin from a file. **Heredoc** (`<< DELIM`) reads lines until the delimiter and expands `$VAR` / `$?` in the body before passing it to the command.

**Quoting** — single quotes are fully literal; double quotes allow `$VAR` and `$?` expansion. Unclosed quotes are rejected as syntax errors before any execution.

**Signals** — Ctrl+C at the prompt redraws the line (status 130). During a command, the child receives SIGINT and the shell reports 130. Ctrl+\ is ignored at the prompt; during a command the child is quit and the shell sets status 131. Heredoc has its own handler so Ctrl+C there aborts and returns to the prompt cleanly.

### Built-ins

| Command | Behavior |
|---------|----------|
| `echo` | Print arguments to stdout; `-n` suppresses the trailing newline. |
| `cd` | Change directory; updates `PWD` and `OLDPWD`. |
| `pwd` | Print the current working directory. |
| `env` | Print the environment. |
| `export` | Set or update environment variables (`export NAME=value`). |
| `unset` | Remove environment variables. |
| `exit` | Exit with optional status 0–255; non-numeric argument exits 255; extra arguments do not exit. |

---

## Internals

### Execution pipeline

```
Raw input string
      │
      ▼
   add_space() ──── add spaces around | && || < > << >>
      │
      ▼
   mask_quoted_chars() ──── mask bytes inside quotes (so delimiters aren't split on)
      │
      ▼
   syn_error() ──── reject leading/trailing |, invalid redirections
      │
      ▼
   split_by_delim(" || ") ──── then per segment split_by_delim(" && ")
      │
      ▼
   Per pipeline: ft_split(·, '|') → build_arr() → linked list of t_cmd
      │
      ▼
   Executor ──── for single pipeline: execute_pipeline(); for &&/||: loop over segments,
                run each via run_pipeline_string() (split by |, build_arr, expand, execute_pipeline), short-circuit on status
```

Operator precedence is encoded by the order of splitting: split on `" || "` first, then on `" && "`, then on `|`, so pipe binds tighter than logical operators. The executor either runs one pipeline (linked list of `t_cmd`) or iterates over the `||` and `&&` segments and runs each pipeline string in turn, forking and using `pipe()` / `dup2()` for stages and `execve()` for external commands.

---

## System Calls

### `fork(2)` — Process creation

Every external command runs in a child process. `fork()` duplicates the calling process — same memory, same open file descriptors, same signal disposition. The child gets return value `0`; the parent gets the child's PID. After forking, the child calls `execve()` to replace its image with the target program.

```c
pid_t pid = fork();
if (pid == 0) {
    execve(path, argv, envp); // child becomes the new program
    perror("execve");
    exit(127);
} else {
    waitpid(pid, &status, 0); // parent collects exit status
}
```

> *Reference: The Linux Programming Interface, Chapter 24 — Process Creation.*

---

### `execve(2)` — Program execution

`execve()` replaces the calling process's image with a new program. It never returns on success. Before calling it, the shell resolves the command name by walking each directory in `$PATH` and stat-ing candidates until it finds an executable. The resolved path, the argument vector, and the current environment are passed in.

> *Reference: The Linux Programming Interface, Chapter 27 — Program Execution.*

---

### `pipe(2)` — Inter-process communication

A pipeline like `cmd1 | cmd2` requires cmd1's stdout to feed directly into cmd2's stdin. `pipe()` creates two file descriptors — `fd[0]` for reading, `fd[1]` for writing — backed by a kernel buffer.

```
pipe(fd)
  ┌─────────────────────────┐
  │  fd[1]  →  kernel buf  →  fd[0]  │
  └─────────────────────────┘
     cmd1 stdout               cmd2 stdin
```

After forking, each child uses `dup2()` to point its stdin or stdout at the right end, then closes both original descriptors. Forgetting to close the write end in the reading process is a classic bug — `read()` never sees EOF and the pipeline hangs indefinitely.

> *Reference: The Linux Programming Interface, Chapter 44 — Pipes and FIFOs.*

---

### `dup2(2)` — File descriptor redirection

`dup2(oldfd, newfd)` makes `newfd` refer to the same open file description as `oldfd`, atomically closing `newfd` first if it was already open. This is the mechanism behind both pipe wiring and file redirections (`>`, `<`, `>>`). The executed program never knows its stdin or stdout was swapped — it just reads and writes file descriptors 0 and 1 as usual.

```c
// redirect stdout to a file before execve
int fd = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
dup2(fd, STDOUT_FILENO);
close(fd);
```

> *Reference: The Linux Programming Interface, Chapter 5 — File I/O: Further Details.*

---

### `waitpid(2)` — Reaping children

After forking, the parent must call `waitpid()` to collect the child's exit status. A child that exits before being waited on becomes a **zombie** — it remains in the process table until the parent collects it. In a multi-stage pipeline, the shell forks one child per stage and must wait for all of them; `$?` is set to the exit status of the last command in the pipeline.

Exit status is encoded in an integer and decoded with macros:

```c
if (WIFEXITED(status))
    code = WEXITSTATUS(status);          // normal exit
else if (WIFSIGNALED(status))
    code = 128 + WTERMSIG(status);       // killed by signal (e.g. 130 for SIGINT)
```

> *Reference: The Linux Programming Interface, Chapter 26 — Monitoring Child Processes.*

---

### Signals — `SIGINT`, `SIGQUIT`

Signal handling depends on context: prompt, running command, or heredoc. The code uses `signal()` to install handlers. At the start of each forked command, the child resets SIGINT and SIGQUIT to `SIG_DFL` so that Ctrl+C and Ctrl+\ are delivered to the child, not swallowed by the shell.

**SIGINT (Ctrl+C)** — At the prompt: print a newline and redisplay the prompt; the shell does not exit (exit status 130 is set). During a command: the child receives SIGINT and exits; the shell collects status 130.

**SIGQUIT (Ctrl+\\)** — Ignored at the prompt. During a command: delivered to the child; the shell prints `Quit: 3` and collects status 131.

> *Reference: The Linux Programming Interface, Chapters 20–22 — Signals.*

---

### Heredoc — `<< DELIM`

Heredoc reads lines from the shell (via readline) until a line matching the delimiter is seen. The body is written to a temporary file in `/tmp` (e.g. `/tmp/.heredoc1`); that file is then opened as stdin for the target command. `$VAR` and `$?` expansion is applied to each line before it is written. A dedicated SIGINT handler is installed during heredoc collection so that Ctrl+C aborts it and returns control to the prompt.

---

## Contributors

- **Mouad Kimdil**
- **Aboubakr Boukdidi**