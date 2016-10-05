#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>

#define tokenDelim " \n\t"
#define bufferSize 100

char* readCmd();
char* readCmd();
char** tokenizer(char*);
bool exitCheck(char**);
bool cdCheck(char**);
int cmdCount(char**);



int main(void){
  bool terminate = false;
  while(!terminate){
    char* cmd;
    char* cwd;
    char** cmdToken;
    int i = 0;
    int cmd_counter = 0; // count the number of pipes needed
    pid_t pid;


    cmd = readCmd();
    cmdToken = tokenizer(cmd);
    cmd_counter = cmdCount(cmdToken);
    //  for(i = 0; i<3; i++){
    //    printf("%s | ", cmdToken[i]);
    //  }

    if(exitCheck(cmdToken)==true){
      printf("exiting shell..\n");
      terminate = true;
      exit(0);
    }
    else if(cdCheck(cmdToken)==true){
      if(cmdToken[1]==NULL){
        perror("error: not enough arguement\f");
      }
      else{
        cwd = getcwd(NULL, 0);
        //printf("%s \n", cwd);
        strncat(cwd, "/", bufferSize);
        strncat(cwd, cmdToken[1], bufferSize);
        printf("%s \n", cwd);
        if(chdir(cwd) == -1){
          perror("error: directory not found\n");
        }
      }
    }
    else{
    /////////////////////////////////////////////
    /*forking if command not calling build-in's*/
    /////////////////////////////////////////////
    pid = fork();
    printf("pid = %d\n", pid);
    if(pid<0){
      perror("error");
      exit(1);
    }
    else if(pid==0){
      printf("------in child------\n");
      if(execvp(cmdToken[0],cmdToken)<0){
        perror("error: execution failed");
      }
    }
    else{
      printf("------parent waiting------\n");
      wait();
      printf("------parent finished waiting------\n");
      printf("parent exiting\n");
      //exit(0);
    }
  }
}
  return 0;
}


/// Print out a prompt then read a command from user
char* readCmd(){
  printf("Please enter a command:\n");
  char* line = NULL;
  ssize_t buf = 0;
  getline(&line, &buf, stdin);
  return line;
}

/// Split command arguement into tokens
char** tokenizer(char* line){
  char** tokens = malloc(bufferSize);
  char* token;
  int i = 0; //array position counter

  tokens[i] = strtok(line,tokenDelim);

  while(tokens[i]!=NULL)
  {
    tokens[++i] = strtok(NULL,tokenDelim);
  }
  return tokens;
}


/// Check for "exit" command, if so return true for main to exit
 bool exitCheck(char** cmdTok){
   if(strcmp("exit", cmdTok[0])==0){
     //printf("true \n");
     return true;
   }
   else{
     //printf("false \n");
     return false;
   }
 }

/// Check for "exit" command, if so return true for main to change directory
 bool cdCheck(char** cmdTok){
   if(strcmp("cd", cmdTok[0])==0){
     //printf("true \n");
     return true;
   }
   else{
     //printf("false \n");
     return false;
   }
 }

 int cmdCount(char** cmdTok){
   int n = sizeof(cmdTok)/sizeof(cmdTok[0]);
   int cmd_counter=1;
   int i = 0;//loop counter
   for(i; i<n; i++){
     if(strcmp("|", cmdTok[i])==0){
       cmd_counter++;
     }
   }
   printf("Number of commands: %d\n", cmd_counter);
   return cmd_counter;
 }
