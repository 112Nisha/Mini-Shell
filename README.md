# Description
- The prompt is colored in the same way an actual terminal is colored.
- Errors are colored red
- Multiple tabs and spaces are handled
- files, folders and executables are colored too
- No pastevents commands are stored in the file

# Assumptions

Part A

- bg process works only on non-user defined functions
- For the same command, seek cannot open files sometimes.
- Pastevents does not store full commands. It stores every smallest segment input that can be executed.
- If we have one file and one folder, -e flag will not have it's condition satisfied
- When Pastevents execute is performed on an "exit" command, it will execute whatever was executed the last.
- If two commands are entered with ';', and one of them is wrong, the wrong one alone is not entered in the file.
- For seek, if flags d and e are used simultanously, I am assuming that the user wants to go into a directory and not list all of the folders. Similarly if f and e are used, I am assuming that the user wants to print the contents of a file and not list all of the files.
- For seek I am asuuming that parent directories cannot be accessed.
- Not more than 1000 input words with 1000 characters each are input.
- If there is an erroenous input given to a background process, it will not be executed at all.

Part B

- Assuming no () are used while giving inputs.
- Token hierarchy : ";" > "&" > "|" > ">" = "<" = ">>". <!-- cause apparently it changes with context and (), and I am not using () or ''-->
- Tokenising with " " sometimes, so if commands have " " in between them, it will not work.
- While piping, for now, I am assuming that you do not enter &, although I am tokenizing with & and |, I am not separating the processes to ensure they work fine with that.
- ls -l | grep in works and not for ls -l | grep "in"
- spec 12: Assuming I need to show only the procs that are not exited for activities
- Using ctrl + z or ctrl + c after a user defined function such as neonate gives undefined behaviour and I am assuming it is not necessary for me to handle those.
- fg bg processes work only for processes spawned by my shell.
