# C-Shell 
Operating Systems Assignment 1, Part 2 was to create a shell that mimics some features of the linux system shell to learn the fork() feature of C.


## Compiling Instructions 
Copy all files into a location including my_shell.c which contains the shell, proc_parse.c which performs the parsing, and Makefile. Within linux, cd into the location of these files, type "make" in the command console, which creates the executable "shell". 

## Deployment
After compiling, to run type ./shell 

## Work flow of the shell: 
* Ask user for command input
* Parse input strings into tokens 
* Check for the internal commands "exit" (which would close exit the program) and "cd" which changes to the desired file location. 
* If an internal command, perform the internal command, re-start main loop. 
* If not, parse the tokensand check for the pipe symbol "|" in case piping needs to be performed. 
* Call Fork(), start child process which executes the user command, merge with parent when completed, exit main loop and go back to start. 


## Example tests: 
* cd Documents -> Enters into the documents folder if you have one. 
* cat <textfile name> -> runs the program "cat" to show a textfile. 


## Author 
Ka Shun (Jason) Chan, ksc19@sfu.ca
