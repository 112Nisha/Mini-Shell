# Mini Shell

This shell mirrors the functionalities of a standard bash shell. Here are its key features:

- Displays a prompt upon completion of a foreground process.
- Supports `;` and `&` to separate commands. Treats `;` as a new-line character and `&` as an indicator for a background process.
- Allows the usage of pipes and redirection arrows.
- Performs commands similar to terminal actions:
  - `warp`: akin to `cd`
  - `peek`: akin to `ls`, supporting the same flags and input format.
- Past Events:
  Tracks up to 15 past commands. 
  Functions (usage):
    - `pastevents`: Displays past commands in order of recent use.
    - `pastevents purge`: Clears all past command information.
    - `pastevents execute <num>`: Executes the `<num>`-th last command.
- Proclore Function:
  Displays the following information on a process: PID, Process status, Process Group, Virtual memory, Executable path.
- Seek Function:
  Searches for a file/directory in a specified or current directory and returns relative paths of matching files/directories.
  Flags:
    - `-d`: Look for directories (ignore files even if the name matches).
    - `-f`: Look for files (ignore directories even if the name matches).
    - `-e`: Execute/change directory based on a single match.
      - This flag is effective only when a single file or a single directory with the name is found.
      - If only one file (and no directories) is found, then prints its output.
      - If only one directory (and no files) is found, then changes the current working directory to it.
      - Otherwise, the flag has no effect.
      - Works in conjunction with `-d` and `-f` flags.
      - If `-e` flag is enabled but the directory lacks access permission (execute) or the file lacks read permission, it outputs "Missing permissions for the task!".
- Activities Function:
  Lists processes spawned by the shell in lexicographic order.
  Information includes Command Name, PID, and state (running or stopped).
- Neonate Function:
  Prints the PID of the most recently created process.
  Continuously prints every `[time_arg]` seconds until the 'x' key is pressed.
  Usage: `neonate` or `neonate -n <time_arg>`.
- iMan Command:
  Fetches the man page of a specified command.
  Usage: `iMan <command_name>`.

## Concepts and Ideas Utilized

- `fork`, `wait`
- System calls
- Networking with TCP
- Raw and cooked mode
- Knowledge of Linux

**Note:** Ensure proper permissions for execution and file access to avoid errors.
